# LoongArch 架构底层差异分析

## LA64 / LA32S / LA32R 对比

| 特性 | LA64 | LA32S | LA32R |
|------|------|-------|-------|
| `CPUCFG1.ARCH` | 2 | 1 | 0 |
| GPR 宽度 | 64 位 | 64 位 (内部) / 32 位 (可见) | 同 LA32S |
| PC 宽度 | 64 位 | 32 位 (`set_pc` 截断) | 同 LA32S |
| 虚拟地址宽度 | 48 位 (VALEN=0x2f) | 32 位 (VALEN=0x1f) | 同 LA32S |
| 物理地址宽度 | 36 位 (PALEN=0x2f) | 32 位 (PALEN=0x1f) | 同 LA32S |
| FPU | 有 | 有 | 有 |
| LSX/LASX | 有 | 无 | 无 |
| IOCSR | 有 | 有 | 无 |
| UAL | 有 | 有 | 无 |
| HP (Huge Page) | 有 | 有 | 无 |

---

## 1. CSR 差异

### 1.1 TLB Entry 格式

LA32 和 LA64 共用 CSR 寄存器编号，但位域宽度不同：

```
TLBEHI VPPN:
  LA64: CSR_TLBEHI_64.VPPN = bits[47:13] (35 bits)
  LA32: CSR_TLBEHI_32.VPPN = bits[31:13] (19 bits)

TLB Entry PPN:
  LA64: TLBENTRY_64.PPN = bits[47:12] (36 bits)
        TLBENTRY_64.NR/NX/RPLV = bits[61:63] (权限增强位)
  LA32: TLBENTRY_32.PPN = bits[31:8]  (24 bits)
        无 NR/NX/RPLV 位

DMW:
  LA64: CSR_DMW_64.VSEG = bits[63:60] (4 bits)
  LA32: CSR_DMW_32.VSEG = bits[31:29] (3 bits)
        CSR_DMW_32.PSEG = bits[27:25] (3 bits)
```

**emu 状态**: ✅ 已处理 — `fill_tlb_entry()` 和 `raise_mmu_exception()` 均已通过 `is_la64()` 选择正确的位宽。

### 1.2 CSR 权限差异

| CSR | LA64 | LA32S | LA32R |
|-----|------|-------|-------|
| EUEN.FPE | R/W | R/W | R/O |
| EUEN.SXE | R/W | — | — |
| EUEN.ASXE | R/W | — | — |
| MISC.VA32[3:0] | R/W (可按 PLV 设 32 位地址) | — | — |

**emu 状态**: ⚠️ EUEN.SXE/ASXE 在 LA32 下应始终为 0 且写忽略。

### 1.3 CSR 写入掩码差异

```
LA64 TLBELO 写掩码:
  TLBELO_64_WMASK = V | D | PLV | MAT | G | PPN[47:12] | NR | NX | RPLV

LA32 TLBELO 写掩码:
  TLBELO_32_WMASK = V | D | PLV | MAT | G | PPN[31:8]
  (无 NR/NX/RPLV)
```

**emu 状态**: ✅ 已定义在 `cpu-csr.h`。

---

## 2. DMW (Direct Mapping Window) 差异

### 2.1 地址计算

**LA64**: `PA = VA & TARGET_PHYS_MASK` (直接截断到 36/48 位)
**LA32**: `PA = (VA[28:0]) | (PSEG << 29)` (使用 PSEG 段寄存器)

```c
// cpu_helper.c: dmw_va2pa()
if (is_la64(env)) {
    return va & TARGET_VIRT_MASK;          // 高位截断
} else {
    uint32_t pseg = FIELD_EX32(dmw, CSR_DMW_32, PSEG);
    return (va & 0x1FFFFFFF) | (pseg << 29); // 段式映射
}
```

### 2.2 DMW 匹配

**LA64**: VSEG = VA[63:60], 匹配 DMW.VSEG[3:0]
**LA32**: VSEG = VA[31:29], 匹配 DMW.VSEG[2:0]

```c
if (is_la64(env))
    base_v = address >> 60;   // 4-bit VSEG
else
    base_v = address >> 29;   // 3-bit VSEG
```

**emu 状态**: ✅ 已处理。

---

## 3. TLB / MMU 差异

### 3.1 页表层级

| 层级 | LA64 | LA32S | LA32R |
|------|------|-------|-------|
| PGD (Level 4) | PWCH.DIR4_BASE/WIDTH | 通常为 0 | 同 LA32S |
| PUD (Level 3) | PWCH.DIR3_BASE/WIDTH | 通常为 0 | 同 LA32S |
| PMD (Level 2) | PWCL.DIR2_BASE/WIDTH | PWCL.DIR2_BASE/WIDTH | 同 LA32S |
| PTE (Level 1) | PWCL.DIR1_BASE/WIDTH | PWCL.DIR1_BASE/WIDTH | 同 LA32S |

LA32 典型配置: 2 级页表 (DIR2 + DIR1)，DIR3/DIR4_WIDTH = 0。

**emu 状态**: ✅ 已处理 — `get_dir_base_width()` 在 `dir_width == 0` 时跳过该级。

### 3.2 TLB 查找

VPPN 匹配宽度不同（LA32: 19 bits, LA64: 35 bits）。`fill_tlb_entry()` 已处理。

### 3.3 TLB 条目数

LA32R 和 LA32S 的 STLB/MTLB 条目数可能与 LA64 不同（取决于 CPU 模型）。当前 emu 使用相同大小 (STLB=2048, MTLB=64)。

**emu 状态**: ⚠️ LA32R 可能应有更小的 TLB。

---

## 4. 异常处理差异

### 4.1 异常向量计算

**LA64**: `EENTRY + (EXCCODE & 0x3f) * VS` (VS 可为 0/4/8/16)
**LA32**: 相同公式，但 VS 取值范围可能不同。

### 4.2 异常类型

LA32 不支持的指令类型在 LA32 下触发 INE (Instruction Non-Existent) 而非 IPE (特权错误)：
- 所有 `*_d` 双字指令
- `lu32i_d`, `lu52i_d`, `addu16i_d`
- 64-bit load/store
- LSX/LASX 向量指令
- 64-bit 原子指令

**emu 状态**: ✅ `CHECK_LA64()` 宏已添加到 126 个 LA64 专属函数。

### 4.3 ERA/BADV 保存宽度

**LA64**: 64 位完整保存
**LA32**: 仅低 32 位有效

`loongarch_cpu_do_interrupt()` 中:
```c
env->CSR_ERA = env->pc;    // LA32 下 PC 已被截断到 32 位
env->CSR_BADV = address;   // LA32 下 address 为 32 位
```

**emu 状态**: ✅ `set_pc()` 已做截断，CSR 赋值自动保留 32 位。

### 4.4 ERTN 恢复

`helper_ertn()` 从 CSR_ERA 恢复 PC 和特权级。LA32 下 PC 会被 `set_pc()` 自动截断。

**emu 状态**: ✅ 已处理。

---

## 5. 指令集差异

### 5.1 LA64 专属指令 (LA32 下触发 INE)

| 类别 | 指令 |
|------|------|
| 算术双字 | `add.d`, `sub.d`, `mul.d`, `div.d`, `mod.d`, `div.du`, `mod.du`, `mulh.d`, `mulh.du`, `mulw.d.w`, `mulw.d.wu`, `alsl.d` |
| 立即数双字 | `addi.d`, `addu16i.d`, `lu32i.d`, `lu52i.d` |
| 移位双字 | `sll.d`, `srl.d`, `sra.d`, `rotr.d`, `slli.d`, `srli.d`, `srai.d`, `rotri.d` |
| 位操作双字 | `clo.d`, `clz.d`, `cto.d`, `ctz.d`, `revb.d`, `revh.d`, `bitrev.d`, `bytepick.d`, `bstrins.d`, `bstrpick.d` |
| 访存双字 | `ld.d`, `st.d`, `ldx.d`, `stx.d`, `ldptr.d`, `stptr.d`, `ldgt.d`, `stgt.d`, `ldle.d`, `stle.d`, `ld.wu` |
| 原子双字 | `am*_d`, `am*_du`, `am*_db.d`, `am*_db.du`, `ll.d`, `sc.d` |
| CRC 双字 | `crc.w.d.w`, `crcc.w.d.w` |
| 断言双字 | `asrtle.d`, `asrtgt.d` |
| 定时器双字 | `rdtime.d` |
| FP 高级 | `fscaleb.s/d`, `flogb.s/d`, `frint.s/d` |
| 向量 | LSX/LASX 全部 |

### 5.2 LA32R 简化的指令

LA32R 还额外移除：
- `cpucfg` 指令 (无 CPUCFG)
- IOCSR 访问指令
- CRC 指令

**emu 状态**: ⚠️ LA32R 的 cpucfg/IOCSR/CRC 移除需要添加 `CHECK_LA32S()` 宏。

---

## 6. 总结

| 组件 | LA32 适配状态 |
|------|--------------|
| CSR 位宽 (VPPN/PPN/DMW) | ✅ 通过 `is_la64()` 分支 |
| DMW 地址计算 | ✅ `dmw_va2pa()` 双路径 |
| DMW 匹配 | ✅ VSEG 宽度自动适配 |
| 页表层级 | ✅ `dir_width==0` 自动跳过 |
| TLB 条目大小 | ⚠️ 可能需缩减 |
| 异常向量 | ✅ 公式相同 |
| PC 截断 | ✅ `set_pc()` |
| 指令门控 | ✅ `CHECK_LA64()` |
| EUEN 权限控制 | ⚠️ SXE/ASXE 应写忽略 |
| LA32R cpucfg 移除 | ⚠️ 未实现 |
| LA32R IOCSR 移除 | ⚠️ 未实现 |
