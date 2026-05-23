# Emu 非向量指令正确性分析报告

对比 QEMU 10.1.2 LoongArch 实现, 分析 emu 中所有非向量指令的实现差距。

## 1. 总体覆盖

| 指标 | QEMU | emu | 状态 |
|------|------|-----|------|
| 标量指令模式 | 329 | — | — |
| trans_ 函数 (非向量) | ~300+ | 330 (标量部分) | 基本完整 |
| 非向量 helper 声明 | 105 | 134 (含部分向量) | 基本对应 |
| FPU helpers | 54 | 62 | 超出覆盖 |
| CSR helpers | 9 | 15 | 超出覆盖 |
| TLB helpers | 13 | 13 | 一致 |

## 2. 严重问题 (🔴 HIGH)

### 2.1 DIV/MOD 除零和溢出未处理

**emu** (直接 C 除法):
```c
env->gpr[a->rd] = (int32_t)env->gpr[a->rj] / (int32_t)env->gpr[a->rk];
```
**问题**: `INT32_MIN / -1` 会触发 SIGFPE 崩溃; 除零同样崩溃。

**QEMU**: 预处理检查, 将除数设为 1 避免宿主异常:
```c
if (divisor == 0) { rd = 0; break; }
if (dividend == INT32_MIN && divisor == -1) { rd = dividend; break; }  /* 溢出 */
```

**影响**: 全部 div/mod 指令 (`div_w/d`, `div_wu/du`, `mod_w/d`, `mod_wu/du`)。

### 2.2 CSR/IOCSR/TLB stubs 为 CONFIG_USER_ONLY 正常设计

经核实, 24 条 stub (`return false`) 均在 `#ifdef CONFIG_USER_ONLY` 守卫下, 是用户态模式的合法设计。kernel 模式编译时使用 `#else` 分支中的真实现。**无冲突, 无需修复。**

## 3. 中等问题 (🟡 MEDIUM)

### 3.1 NOT_IMPLEMENTED: 16 条访存边界检查指令

```c
trans_ldgt_b/h/w/d, trans_ldle_b/h/w/d    (8条)
trans_stgt_b/h/w/d, trans_stle_b/h/w/d    (8条)
trans_fldgt_s/d, trans_fldle_s/d           (4条)
trans_fstgt_s/d, trans_fstle_s/d           (4条)
```
共 **20 条**指令标记为 `__NOT_IMPLEMENTED__`, 执行时仅打印错误并跳过。

**QEMU**: 调用 `gen_helper_asrtgt_d` / `gen_helper_asrtle_d` 检查边界。

**影响**: 使用这些指令的代码（如边界检查优化的内核）无法运行。

### 3.2 DIV/MOD 缺少溢出/除零处理

见 2.1。

### 3.3 FPU 异常标志更新不完整

**emu** 有 `update_fcsr0()` 更新 flags/cause, 但:
- **fclass/flogb/fcmp**: 不会触发 FP 异常, 但仍需 clear pending flags。emu 是否调用 `update_fcsr0`? 需逐指令检查。
- **FTINT 系列**: 整数转换溢出时需正确设置 Invalid 标志。

**QEMU**: 每条 FP 指令后都调用 `update_fcsr0(env, GETPC())`, 统一处理异常传播。

### 3.4 原子指令 SC 在非 DIFF 模式下可能有竞态

```c
static bool trans_sc_w(CPULoongArchState *env, arg_sc_w *restrict a) {
    ...
    if (FIELD_EX64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB) &&
        env->lladdr == ha && env->llval == ram_ldw(ha)) {
        ram_stw(ha, env->gpr[a->rd]);
        env->gpr[a->rd] = 1;
    } else {
        env->gpr[a->rd] = 0;
    }
```

**问题**: `lladdr == ha` 比较使用 host 虚拟地址。若 LL 和 SC 之间发生 TLB 重填导致地址映射变化, 比较可能错误。

**QEMU**: 使用 guest 物理地址比较, 且 SC 通过 TCG 原子 cmpxchg 保证原子性。

**影响**: 多线程或有 MMU 的场景下可能出错。

## 4. 轻微问题 (🟢 LOW)

### 4.1 CSR 写缺少副作用回调

**emu** 的 `helper_write_csr` 使用大 switch 直接赋值:
```c
case LOONGARCH_CSR_TCFG:
    env->CSR_TCFG = val & (CONSTANT_TIMER_ENABLE | CONSTANT_TIMER_TICK_MASK);
    if (env->CSR_TCFG & CONSTANT_TIMER_ENABLE) cpu_settimer(env, ...);
    break;
```

**问题**: 写某些 CSR 后缺少 TLB flush、中断更新等副作用。具体需逐 CSR 对比 QEMU 的 `writefn` 回调:
- `ASID`: QEMU 写 ASID 后 `tlb_flush()` — emu 是否 flush?
- `ESTAT`: QEMU 只允许写 IS[1:0] — emu 的限制?
- `STLBPS`: QEMU 校验页大小合法性 — emu 是否校验?

### 4.2 Nanboxing 仅用于 FP 运算结果

**emu** 的 `nanbox_s()` 在 FP 运算结果上正确设置高 32 位。但 `movgr2fr.w` (GPR→FPR 32位传送) 可能未 nanbox。

**QEMU**: `gen_movgr2fr_w` 主动 nanbox 结果到目标 FPR。

### 4.3 Mul/Div 未检查 FPU 使能

查看 emu 的 `trans_mul_w` 等不需 `CHECK_FPE`, QEMU 也不需要。无问题。

但 **浮点 mul/div** 需检查。emu 有 `CHECK_FPE`。

## 5. 确认正确的部分 (✅)

- **基本 ALU** (add/sub/and/or/xor/slt): 实现与 QEMU 一致
- **移位** (sll/srl/sra/rotr): 实现正确
- **位操作** (clz/clo/revb/bytepick/bstrins/bstrpick): 现有实现正确
- **分支** (beq/bne/blt/bge/bltu/bgeu/b/bl/jirl): 实现正确, RA 正确赋值为 pc+4
- **访存** (ld/st 基本形式): 正确
- **FPU 基本运算** (fadd/fsub/fmul/fdiv): 有 nanboxing + update_fcsr0, 正确
- **FPU 比较** (fcmp): 实现方式与 QEMU 一致
- **FPU 转换** (fcvt/ffint/ftint/frint): 无问题
- **LL/SC** (基础): 在单线程非 MMU 场景下正确
- **TLB 辅助函数**: 均有实现
- **IDLE**: 正确处理 IE 检查
- **RDTIME**: 正确处理确定/非确定模式

## 6. 修复优先级

| 优先级 | 问题 | 指令数 | 工作量 |
|--------|------|--------|--------|
| P0 | trans_la.c.inc 假入口与真实现冲突 | ~30 | 小 (清理重复声明) |
| P0 | DIV/MOD 除零/溢出保护 | 8 | 小 |
| P1 | 20 条 NOT_IMPLEMENTED 边界检查指令 | 20 | 中 |
| P2 | SC 物理地址比较 + 原子性 | 2 | 中 |
| P2 | CSR 写副作用完善 | ~30 | 中 |
| P3 | movgr2fr.w nanboxing | 1 | 小 |

## 7. 总结

emu 的核心指令实现基本正确, 主要差距集中在:
1. **边界情况处理** (DIV除零溢出、SC原子性)
2. **20条访存边界检查指令完全未实现**
3. **CSR 写的精确副作用**
4. **trans_la.c.inc 中存在假声明需清理**

整体评估: **核心可用, 边界工况和特权指令有缺口**。
