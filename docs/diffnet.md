# Diffnet — 网络差分测试系统

## 概述

Diffnet 通过 GDB Remote Serial Protocol (RSP) 连接 QEMU，在 emu 执行每条（或每批）指令后比对寄存器与内存状态，验证 emu 指令实现的正确性。

```
emu (la_emu_kernel)                    QEMU (qemu-system-loongarch64)
─────────────────────                   ─────────────────────────────────
exec_env() 循环:                          GDB stub @ 127.0.0.1:1234
  interpreter(env, insn)                    ▲
  diffnet_check(env) ────TCP/RSP──────────┘
    ├─ qrsp_step()       ──"s"──▶    单步执行
    ├─ qrsp_read_g_packet() ─"g"──▶  读寄存器 (g packet)
    ├─ qrsp_read_register() ─"pN"──▶ 读单个寄存器 (p packet)
    ├─ qrsp_read_memory()   ─"m"──▶  读内存 (m packet)
    └─ diffnet_compare()           比对 GPR/PC/FPR/CSR/Memory
```

## 快速开始

```bash
# 编译
make DIFF_NET=1 -j
make -C tests

# 运行 (单终端, QEMU 自动启停)
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000 -N 1234

# 或使用 Python 脚本
python3 scripts/diffnet.py emu --binary tests/test_add.bin --max-steps 10000 --batch-size 500
```

## 命令行参数

| 参数 | 说明 |
|------|------|
| `-N PORT` | spawn 模式: 自动启动 QEMU 子进程, 比对端口 PORT |
| `-N HOST:PORT` | connect 模式: 连接已有 QEMU |

环境变量 (spawn 模式下自动传递):

| 变量 | 默认值 | 说明 |
|------|--------|------|
| `DIFFNET_MAX_STEPS` | 70000 | 总指令数上限 |
| `DIFFNET_BATCH_SIZE` | 2000 | 批量比对间隔 (条数) |

## 比对模式

### 逐条比对 (batch_size=1)

每执行 1 条指令就 `s`(step) + `g`(read regs) + compare。最精确但最慢 (~2 RTT/指令)。

### 批量比对 (batch_size>1, 默认)

emu 先跑 batch_size 条指令, 然后快速 step QEMU 同样次数, 最后读一次 `g` packet 比对。

| batch_size | 10万指令耗时 | 网络开销 |
|-----------|-------------|---------|
| 1 (逐条) | ~100s | 200K RTT |
| 2000 | ~35s | 50K RTT |

每 10 批输出一次进度: `DIFFNET: batch 20000 ok (icount=19999)`

## 比对范围

### GDB RSP 寄存器布局 (LoongArch64)

| GDB regnum | 内容 | 大小 | 访问 |
|-----------|------|------|------|
| 0-31 | GPR r0-r31 | 8B | `g` packet |
| 32 | orig_a0 | 8B | `g` packet |
| **33** | **PC** | 8B | `g` packet |
| 34 | BADV | 8B | `g` packet |
| 35-66 | FPR f0-f31 | 8B | `pN` |
| 67-74 | FCC fcc0-7 | 1B | `pN` |
| 75 | FCSR | 4B | `pN` |
| 76-107 | LSX vr0-31 | 16B | `pN` |
| 108-139 | LASX xr0-31 | 32B | `pN` |

### 比对项详情

| 类别 | 内容 | 条件 | 实现 |
|------|------|------|------|
| **GPR** | r0-r31 (64位×32) | 始终 | `g` packet → `env->gpr[i]` |
| **PC** | 程序计数器 | 始终 | `g` packet reg 33 → `env->pc` |
| **FPR** | f0-f31 (低64位) | cpucfg.FP=1 | `p35`-`p66` → `env->fpr[i].vreg.D[0]` |
| **FCC** | fcc0-7 (布尔) | cpucfg.FP=1 | `p67`-`p74` → `env->cf[i]` |
| **FCSR** | 浮点控制状态 | cpucfg.FP=1 | `p75` → `env->fcsr0` |
| **LSX** | vr0-31 (128位) | cpucfg.LSX=1 | `p76`-`p107` → `env->fpr[i].vreg.D[0..1]` |
| **LASX** | xr0-31 (256位) | cpucfg.LASX=1 | `p108`-`p139` → `env->fpr[i].vreg.D[0..3]` |
| **CSR** | 22个关键CSR | dump区非空 | `m` packet → `env->CSR_*` |
| **内存** | 测试输出区 4B | 输出非零 | `m` packet → `ram_lduw()` |
| **TLB** | — | 不可行 | GDB 不暴露 TLB |

### CSR 比对方式

QEMU GDB stub 不暴露 CSR 寄存器。变通方案: 测试程序用 `csrrd` 指令将 CSR 值写入内存 dump 区 (0x1c000200), diffnet 通过 `m` packet 读出 QEMU 侧的 dump 区, 与 emu 直接读取的 `env->CSR_*` 比对。

dumped CSR 列表 (22个):
```
CRMD PRMD EUEN ECFG ESTAT ERA BADV EENTRY
TLBIDX TLBEHI TLBELO0 TLBELO1 ASID PGDL PGDH
STLBPS TCFG TVAL TICLR LLBCTL DMW0 DMW1
```

## 架构

```
src/core/main.c
  ├── -N PORT         → diffnet_spawn()    (fork QEMU + connect)
  ├── -N HOST:PORT    → diffnet_init()     (connect to existing QEMU)
  └── exec_env()
        └── diffnet_check(env)             (每条指令后调用)
              ├── batch边界? → step QEMU batch_size次
              ├── qrsp_read_g_packet()     (读 GPRs + PC)
              └── diffnet_compare()        (全量比对)

tools/qrsp/qrsp.c         GDB RSP 客户端库 (零 emu 依赖)
src/difftest/diffnet.c    集成模块 (spawn, compare, cleanup)
include/diffnet.h         公开接口
scripts/diffnet.py         Python 测试脚本 (emu/step/batch 三种模式)
tests/                     LoongArch 汇编测试程序
```

## 输出示例

正常退出 (0 mismatches):
```
DIFFNET: spawning QEMU: /.../qemu-system-loongarch64
DIFFNET: Connected to QEMU at 127.0.0.1:1234
DIFFNET: synced GPRs from QEMU, QEMU PC=0x1c000000 emu PC=0x1c000000
DIFFNET: max_steps=70000 batch_size=2000
DIFFNET: batch 20000 ok (icount=19999)
DIFFNET: batch 40000 ok (icount=39999)
DIFFNET: reached step limit 70000, exiting
DIFFNET: QEMU child process terminated
EXIT: 0
```

Mismatch 输出:
```
=== DIFFNET MISMATCH batch=1000 icount=999 prev_pc=0x1c000028 qemu_pc=0x1c00002c ===
  gpr r12 : emu=0x000000000000005a  qemu=0x0000000000000000
  fpr f0  : emu=0x4040000000000000  qemu=0x0000000000000000
  csr CRMD: emu=0x0000000000000008  qemu=0x0000000000000000
  PC  : emu=0x000000001c000028  qemu=0x000000001c00002c
  (prev instruction at pc=0x000000001c000028: 0x0280a80c)
```

## 限制

- **TLB**: GDB RSP 不暴露 TLB 内容, 无法比对
- **CSR**: 依赖测试程序内嵌 csrrd 指令 dump 到内存, 不能实时比对
- **内存**: 仅比对测试输出区 (0x1c000100), 非全量内存比对
- **LASX**: 需 cpucfg.LASX=1, 当前 emu 默认不启用
- **QEMU 路径**: 硬编码在 `include/diffnet.h` 的 `DIFFNET_QEMU_PATH` 宏中
