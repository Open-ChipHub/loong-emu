# LoongArch 模拟器

English: [README.en.md](README.en.md)

## 概述

`loong-emu` 是一个面向 LoongArch 的解释执行模拟器，主要用于内核启动、
裸机程序执行、指令实现验证、QEMU 差分测试和指令统计。当前主线入口是
kernel/bare-metal 模式的 `build/la_emu_kernel`；代码中也保留了 user-mode
ELF 加载与系统调用模拟路径，可按需手动构建 `build/la_emu_user`。

核心能力：

- LoongArch64 基础整数、特权架构、CSR、异常/中断、TLB/MMU
- 浮点、LSX、LASX 向量寄存器与指令解释
- kernel/bare-metal 程序加载，支持 ELF、raw binary、checkpoint 目录
- GDB server、CLI 调试、PC trace、指令统计报告
- Diffnet：通过 QEMU GDB RSP 做寄存器/状态差分
- 插件接口：执行日志、basic block、BBV、SimPoint checkpoint 等

## 目录结构

- `src/core/`：CPU 状态、主循环、指令译码/解释、指令统计
- `src/mmu/`：TLB/MMU 和地址翻译辅助逻辑
- `src/fpu/`、`src/vec/`：浮点和向量辅助函数
- `src/devices/`：串口、FIFO 等简单设备模型
- `src/gdbstub/`：GDB Remote Serial Protocol server
- `src/difftest/`：difftest 动态库接口和 Diffnet 集成
- `src/checkpoint/`：checkpoint 保存/恢复
- `plugins/`：动态插件示例和插件构建规则
- `tests/`：LoongArch 汇编 smoke/diffnet 测试
- `scripts/`：Diffnet、指令统计和辅助脚本
- `docs/`：Diffnet、指令统计和架构差异说明

## 依赖

构建主程序需要 GCC、make、GLib 开发头文件和库。构建测试程序需要
LoongArch64 交叉工具链。

```bash
# Debian/Ubuntu 示例
sudo apt-get install build-essential libglib2.0-dev

# tests/Makefile 默认使用该前缀，可按本机路径覆盖
export CROSS_PREFIX=/path/to/loongarch64-linux-gnu-
```

Diffnet 和部分脚本需要 `qemu-system-loongarch64`。当前代码/脚本中有本地
QEMU 默认路径，移植到新机器时通常需要调整 `include/diffnet.h` 中的
`DIFFNET_QEMU_PATH`，或使用脚本参数 `--qemu` 指定 QEMU。

## 构建

```bash
# 默认构建 kernel/bare-metal 模拟器和插件
make -j

# 优化构建；Makefile 当前默认 DEBUG=1，因此 DEBUG=0 才使用 -O3/LTO
make DEBUG=0 -j

# 启用 GDB server 支持
make GDB=1 -j

# 启用交互式 CLI 调试
make CLI=1 -j

# 启用插件加载接口 (-p)
make PLUGIN=1 -j

# 启用网络差分 Diffnet (-N)
make DIFF_NET=1 -j

# 生成外部 difftest reference 动态库
make DIFF=1 -j

# 手动构建 user-mode 模拟器
make build/la_emu_user -j

# 构建汇编测试程序
make -C tests

# 清理
make clean
make -C tests clean
make -C plugins clean
```

默认编译期 core 为 `openc910`。可用 `CORE=<name>` 覆盖，例如：

```bash
make CORE=la464 -j
make CORE=centaur320 -j
```

运行时也可以通过 `-A` 选择 CPU 模型，支持 `la464`、`loongarch64`、
`openc910`、`swiftcore`、`loongarch32r`、`loongarch32s`。

## 基本运行

运行 LoongArch kernel ELF：

```bash
./build/la_emu_kernel -m 16 -k /path/to/vmlinux
```

运行 raw binary/bare-metal 程序：

```bash
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000
```

运行 checkpoint 目录：

```bash
./build/la_emu_kernel -z -k /path/to/checkpoint_dir
```

运行 user-mode ELF：

```bash
make build/la_emu_user -j
./build/la_emu_user /path/to/static_program arg0 arg1
```

注意：user-mode 当前不支持动态 ELF；检测到 `PT_INTERP` 时会退出。

## 常用参数

| 参数 | 说明 |
| --- | --- |
| `-m N` | kernel 模式内存大小，单位 GiB；`-m 16` 表示 16 GiB |
| `-k FILE_OR_DIR` | kernel ELF、raw binary 或 checkpoint 目录 |
| `-b` | 按 raw binary/BIOS 方式加载 `-k` 指定文件 |
| `-E ADDR` | raw binary 加载和入口地址 |
| `-A ARCH` | 运行时 CPU 模型：`la464`、`loongarch64`、`openc910`、`swiftcore`、`loongarch32r`、`loongarch32s` |
| `--smp N` | 虚拟 CPU 数量 |
| `-z` | 启用 deterministic events |
| `-w` | 强制启用硬件页表遍历模型，调试 MMU 时建议使用 |
| `-s` | 启用 `serial_plus` 串口模型 |
| `-d MASK` | 打开日志，常用 mask：`exec,cpu,fpu,int,mmu,page,strace,timer,ptw,vpu,plugin,unimp` |
| `-D FILE` | 日志输出文件 |
| `-c MASK` | 打开检查项，例如 `tlb_mhit` |
| `-g` | 启用 GDB server，默认监听端口 `1239`，需 `make GDB=1` |
| `-v` | GDB verbose 日志 |
| `-C PC` | 在指定 PC 触发 checkpoint |
| `-R FILE` | 指令统计 Markdown 报告文件，默认 `report_instruction.md` |
| `-N PORT` | Diffnet spawn 模式：自动启动 QEMU 并连接端口 |
| `-N HOST:PORT` | Diffnet connect 模式：连接已有 QEMU GDB stub |
| `-p PLUGIN[,ARGS]` | 加载插件，需 `make PLUGIN=1` |
| `--trace` | 输出 PC trace，默认文件 `trace_pc.log` |
| `--trance_log_name FILE` | 设置 PC trace 文件名；参数名当前保留源码中的拼写 |

## GDB 调试

```bash
make GDB=1 -j
./build/la_emu_kernel -m 16 -k /path/to/vmlinux -g -w

# 另一个终端
gdb-multiarch /path/to/vmlinux
(gdb) target remote :1239
```

## Diffnet 差分

Diffnet 通过 GDB RSP 连接 QEMU，在 emu 执行过程中按批次比较 GPR/PC，
并可选比较 FPR、LSX、LASX、CSR dump 区和测试输出内存。

```bash
make DIFF_NET=1 -j
make -C tests

# emu 自动启动 QEMU
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000 -N 1234

# 或使用脚本入口
python3 scripts/diffnet.py emu \
  --binary tests/test_add.bin \
  --max-steps 10000 \
  --batch-size 500 \
  --ctxcmp gpr
```

常用环境变量：

| 变量 | 默认值 | 说明 |
| --- | --- | --- |
| `DIFFNET_MAX_STEPS` | `70000` | 最大比较指令数 |
| `DIFFNET_BATCH_SIZE` | `2000` | 批量比较间隔；`1` 表示逐条比较 |
| `DIFFNET_CMP_MASK` | `gpr` | 比较类别：`gpr,fpr,lsx,lasx,csr,mem,all` |
| `DIFFNET_QEMU_CPU` | 由脚本/架构推导 | QEMU CPU 名称 |
| `DIFFNET_CSR_DUMP_ADDR` | 脚本默认 `0x1c000300` | CSR dump trampoline 基地址 |

更多细节见 [docs/diffnet.md](docs/diffnet.md)。

## 指令统计

模拟器退出时会生成 `-R` 指定的 Markdown 指令统计报告：

```bash
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000 -R add_report.md
```

也可以使用脚本限制执行步数并生成报告：

```bash
python3 scripts/insn_report.py \
  --binary tests/test_add.bin \
  --bios \
  --addr 0x1c000000 \
  --max-cycles 50000 \
  --output add_report.md
```

更多细节见 [docs/insn_report.md](docs/insn_report.md)。

## 插件

插件需要用 `make PLUGIN=1 -j` 启用主程序的 `dlopen` 支持。插件库在
`plugins/` 目录下构建。

```bash
make PLUGIN=1 -j
make -C plugins

./build/la_emu_kernel \
  -z -k tests/test_add.bin -b -E 0x1c000000 \
  -p plugins/liblog_exec.so
```

已有插件：

- `liblog_exec.so`：打印每条指令的 PC 和 instruction word
- `libbblk.so`：收集 basic block
- `libbbv.so` / `libbbv_naive.so`：生成 basic block vector
- `libsimpoint.so`：按 SimPoint/weight 保存 checkpoint

## 当前限制

- user-mode 动态 ELF 不支持
- user-mode signal、多线程语义不完整
- block device 尚未建模
- Diffnet 依赖 QEMU GDB stub；TLB 内容无法通过 GDB RSP 直接比较
- CSR 比较依赖测试程序把 CSR dump 到约定内存区域，不是实时 CSR 读取
- 部分脚本和默认 QEMU 路径带有本地环境假设，迁移机器时需要调整
