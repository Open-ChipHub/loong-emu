# LoongArch Emulator

中文: [README.md](README.md)

## Overview

`loong-emu` is an interpreter-style LoongArch emulator used for kernel boot,
bare-metal execution, instruction validation, QEMU differential testing, and
instruction statistics. The primary target today is the kernel/bare-metal
binary `build/la_emu_kernel`. The source tree also keeps a user-mode ELF loader
and syscall emulation path, which can be built explicitly as
`build/la_emu_user`.

Main capabilities:

- LoongArch64 base integer ISA, privileged architecture, CSRs, exceptions,
  interrupts, TLB, and MMU behavior
- Floating-point, LSX, and LASX state and instruction helpers
- Kernel/bare-metal loading for ELF files, raw binaries, and checkpoint
  directories
- GDB server, CLI debugging, PC tracing, and instruction statistics reports
- Diffnet differential testing against QEMU through GDB RSP
- Dynamic plugin hooks for execution logging, basic blocks, BBV, and SimPoint
  checkpoint workflows

## Layout

- `src/core/`: CPU state, main execution loop, decode/interpreter code, stats
- `src/mmu/`: TLB/MMU and address translation helpers
- `src/fpu/`, `src/vec/`: floating-point and vector helpers
- `src/devices/`: serial, FIFO, and simple device models
- `src/gdbstub/`: GDB Remote Serial Protocol server
- `src/difftest/`: external difftest reference API and Diffnet integration
- `src/checkpoint/`: checkpoint save/restore support
- `plugins/`: plugin examples and plugin build rules
- `tests/`: LoongArch assembly smoke/diffnet tests
- `scripts/`: Diffnet, instruction report, and helper scripts
- `docs/`: Diffnet, instruction report, and architecture notes

## Dependencies

The emulator build needs GCC, make, and GLib headers/libraries. Test binaries
need a LoongArch64 cross toolchain.

```bash
# Debian/Ubuntu example
sudo apt-get install build-essential libglib2.0-dev

# Override the default prefix used by tests/Makefile
export CROSS_PREFIX=/path/to/loongarch64-linux-gnu-
```

Diffnet and some scripts require `qemu-system-loongarch64`. Some defaults in
the C code and scripts point to local QEMU paths; on a new machine, update
`DIFFNET_QEMU_PATH` in `include/diffnet.h` or pass `--qemu` to the Python
scripts when available.

## Build

```bash
# Default kernel/bare-metal emulator plus plugins
make -j

# Optimized build. The current Makefile defaults DEBUG=1, so use DEBUG=0
# to select -O3/LTO.
make DEBUG=0 -j

# Enable the GDB server path
make GDB=1 -j

# Enable the interactive CLI debugger
make CLI=1 -j

# Enable dynamic plugin loading (-p)
make PLUGIN=1 -j

# Enable network differential testing (-N)
make DIFF_NET=1 -j

# Build the external difftest reference shared library
make DIFF=1 -j

# Build the user-mode emulator explicitly
make build/la_emu_user -j

# Build assembly test binaries
make -C tests

# Clean
make clean
make -C tests clean
make -C plugins clean
```

The compile-time default core is `openc910`. Override it with `CORE=<name>`:

```bash
make CORE=la464 -j
make CORE=centaur320 -j
```

At runtime, `-A` can select `la464`, `loongarch64`, `openc910`, `swiftcore`,
`loongarch32r`, or `loongarch32s`.

## Basic Usage

Run a LoongArch kernel ELF:

```bash
./build/la_emu_kernel -m 16 -k /path/to/vmlinux
```

Run a raw bare-metal binary:

```bash
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000
```

Run from a checkpoint directory:

```bash
./build/la_emu_kernel -z -k /path/to/checkpoint_dir
```

Run a user-mode ELF:

```bash
make build/la_emu_user -j
./build/la_emu_user /path/to/static_program arg0 arg1
```

User mode does not support dynamic ELF binaries; binaries with `PT_INTERP`
exit early.

## Common Options

| Option | Meaning |
| --- | --- |
| `-m N` | Kernel-mode memory size in GiB; `-m 16` means 16 GiB |
| `-k FILE_OR_DIR` | Kernel ELF, raw binary, or checkpoint directory |
| `-b` | Load `-k` as raw binary/BIOS image |
| `-E ADDR` | Raw binary load address and entry PC |
| `-A ARCH` | Runtime CPU model: `la464`, `loongarch64`, `openc910`, `swiftcore`, `loongarch32r`, `loongarch32s` |
| `--smp N` | Number of virtual CPUs |
| `-z` | Enable deterministic events |
| `-w` | Force hardware page-table-walker modeling; useful for MMU debugging |
| `-s` | Use the `serial_plus` device model |
| `-d MASK` | Enable logs. Common masks: `exec,cpu,fpu,int,mmu,page,strace,timer,ptw,vpu,plugin,unimp` |
| `-D FILE` | Log output file |
| `-c MASK` | Enable checks such as `tlb_mhit` |
| `-g` | Enable the GDB server on port `1239`; requires `make GDB=1` |
| `-v` | Verbose GDB logging |
| `-C PC` | Trigger a checkpoint at the given PC |
| `-R FILE` | Instruction statistics Markdown report; default `report_instruction.md` |
| `-N PORT` | Diffnet spawn mode: launch QEMU and connect to this port |
| `-N HOST:PORT` | Diffnet connect mode: connect to an existing QEMU GDB stub |
| `-p PLUGIN[,ARGS]` | Load a dynamic plugin; requires `make PLUGIN=1` |
| `--trace` | Write a PC trace, default `trace_pc.log` |
| `--trance_log_name FILE` | Set the PC trace filename; spelling follows the current source |

## GDB Debugging

```bash
make GDB=1 -j
./build/la_emu_kernel -m 16 -k /path/to/vmlinux -g -w

# Another terminal
gdb-multiarch /path/to/vmlinux
(gdb) target remote :1239
```

## Diffnet

Diffnet connects the emulator to QEMU through GDB RSP. It compares GPR/PC by
default and can optionally compare FPR, LSX, LASX, CSR dump memory, and test
output memory.

```bash
make DIFF_NET=1 -j
make -C tests

# The emulator auto-spawns QEMU.
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000 -N 1234

# Or use the Python wrapper.
python3 scripts/diffnet.py emu \
  --binary tests/test_add.bin \
  --max-steps 10000 \
  --batch-size 500 \
  --ctxcmp gpr
```

Useful environment variables:

| Variable | Default | Meaning |
| --- | --- | --- |
| `DIFFNET_MAX_STEPS` | `70000` | Maximum compared instructions |
| `DIFFNET_BATCH_SIZE` | `2000` | Compare interval; `1` means per-instruction comparison |
| `DIFFNET_CMP_MASK` | `gpr` | Compare categories: `gpr,fpr,lsx,lasx,csr,mem,all` |
| `DIFFNET_QEMU_CPU` | derived by script/arch | QEMU CPU model |
| `DIFFNET_CSR_DUMP_ADDR` | script default `0x1c000300` | CSR dump trampoline base address |

See [docs/diffnet.md](docs/diffnet.md) for details.

## Instruction Statistics

The emulator writes a Markdown instruction report on exit. Use `-R` to choose
the output file:

```bash
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000 -R add_report.md
```

The helper script can also limit execution and generate a report:

```bash
python3 scripts/insn_report.py \
  --binary tests/test_add.bin \
  --bios \
  --addr 0x1c000000 \
  --max-cycles 50000 \
  --output add_report.md
```

See [docs/insn_report.md](docs/insn_report.md) for details.

## Plugins

Build the emulator with `PLUGIN=1` so `-p` can `dlopen` plugin libraries.

```bash
make PLUGIN=1 -j
make -C plugins

./build/la_emu_kernel \
  -z -k tests/test_add.bin -b -E 0x1c000000 \
  -p plugins/liblog_exec.so
```

Available plugins:

- `liblog_exec.so`: print PC and instruction word for every instruction
- `libbblk.so`: collect basic blocks
- `libbbv.so` / `libbbv_naive.so`: generate basic block vectors
- `libsimpoint.so`: save checkpoints from SimPoint/weight input

## Current Limitations

- Dynamic ELF binaries are not supported in user mode
- User-mode signal and multithread semantics are incomplete
- Block devices are not modeled
- Diffnet depends on QEMU's GDB stub; TLB contents cannot be compared directly
  through GDB RSP
- CSR comparison depends on test code dumping CSRs into agreed memory locations
- Some scripts and default QEMU paths are local-environment assumptions and may
  need adjustment on a new machine
