# Instruction Statistics Reporter (`insn_report.py`)

## 概述

生成 LoongArch 模拟器指令执行统计的 Markdown 报告，包含：

- 异常/中断类型及次数
- ISA 章节分类统计 (算术/分支/访存/浮点/向量/特权等)
- 每条已执行指令的 hex 编码及执行次数、百分比
- 完整 ISA 指令参考表 (1690 条，来自 `trans_la.c.inc`)

## 快速开始

```bash
# 编译
make DIFF_NET=1 -j && make -C tests

# 运行 (单命令: QEMU 自动启停)
python3 scripts/insn_report.py --binary tests/test_add.bin --max-cycles 10000
```

## 参数

| 参数 | 简写 | 默认值 | 说明 |
|------|------|--------|------|
| `--binary` | `-k` | (必需) | 测试程序 (`.bin`) 或内核 (`vmlinux`) |
| `--output` | `-o` | `report_instruction.md` | 输出 Markdown 文件 |
| `--max-cycles` | `-M` | 100000 | 最大执行指令数 |
| `--addr` | `-E` | `0x1c000000` | 程序加载地址 (BIOS 模式) |
| `--memory` | `-m` | — | 内存大小, 如 `16G` (内核模式) |
| `--port` | `-p` | 1234 | QEMU GDB 端口 |
| `--emu` | — | `build/la_emu_kernel` | 模拟器路径 |

## 使用示例

### 测试程序统计

```bash
python3 scripts/insn_report.py \
  --binary tests/test_add.bin \
  --max-cycles 50000 \
  --output add_stats.md
```

### 内核启动统计

```bash
python3 scripts/insn_report.py \
  --kernel vmlinux \
  --memory 16G \
  --max-cycles 2000000 \
  --output kernel_boot.md
```

### 快速默认

```bash
# 使用所有默认值, 生成 report_instruction.md
python3 scripts/insn_report.py -k tests/test_add.bin
```

## 报告结构

生成的 Markdown 文件包含以下章节：

### 1. Exceptions & Interrupts

```
| Type | Count |
|------|-------|
| IRQ (interrupts raised) | 0 |
| TLB refill events | 0 |
| Syscall count | 0 |
| Exception entries (ecount) | 0 |
```

若触发了异常, 还会按异常码逐条列出明细。

### 2. By Category

按 ISA 手册章节分类:

| Category | Count | % |
|----------|-------|----|
| Arithmetic/Shift (Ch3.1) | 45000 | 90.0% |
| Branch/Jump (Ch3.3) | 5000 | 10.0% |

分类对照:

| 枚举 | ISA 章节 | 内容 |
|------|---------|------|
| `CAT_ARITH_SHIFT` | Ch3.1 | 算术/移位 |
| `CAT_BIT_MANIP` | Ch3.2 | 位操作 |
| `CAT_BRANCH` | Ch3.3 | 分支/跳转 |
| `CAT_LOAD_STORE` | Ch3.4 | 访存 |
| `CAT_ATOMIC` | Ch3.5 | 原子指令 |
| `CAT_BARRIER` | Ch3.6 | 屏障 |
| `CAT_OTHER_BASIC` | Ch3.7 | 其他基础 |
| `CAT_FLOAT` | Ch4 | 浮点 |
| `CAT_VECTOR` | Ch5 | LSX/LASX 向量 |
| `CAT_PRIVILEGED` | Ch6 | 特权指令 |

### 3. Executed Instructions

每条被执行的指令, 按次数降序排列:

| # | Instruction | Category | Count | % |
|---|------------|----------|-------|----|
| 1 | `02800000` | Ch3.1 | 7497 | 10.71% |
| 2 | `00103dd0` | Ch3.1 | 5000 | 7.14% |

指令以 32 位 hex 编码表示 (如 `02800000`), 与 `trans_la.c.inc` 中的编码对应。

### 4. Complete ISA Reference

全部 1690 条静态 ISA 指令参考表:

| # | Instruction | Category | Count |
|---|------------|----------|-------|
| 1 | `add.d` | Ch3.1 | 0 |
| 2 | `add.w` | Ch3.1 | 0 |
| ... | ... | ... | ... |

> 注: 此表指令名与第 3 节的 hex key 之间尚未建立双向映射。
> 执行过的指令在 hex-key 表中可看到计数。

## 实现

- 分类器: `src/core/insn_stats.c` — 按 major opcode (bits 31:26) 分类
- 主表: `src/core/insn_table.inc` — 从 `trans_la.c.inc` 自动提取的 1690 条指令
- 注入点: `interpreter()` 每条指令调用 `insn_stats_classify_and_record()`
- 输出: `laemu_exit()` 时自动生成 Markdown 文件

## 依赖

- `la_emu_kernel` 已编译 (`make DIFF_NET=1 -j`)
- `qemu-system-loongarch64` (用于步数限制, 自动启停)

## 相关脚本

| 脚本 | 功能 |
|------|------|
| `scripts/insn_report.py` | 独立指令统计报告 |
| `scripts/diffnet.py emu` | 差分比对 + 报告 (推荐) |
| `scripts/diffnet.py stats` | 同上, 快捷方式 |
