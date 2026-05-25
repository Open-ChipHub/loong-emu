# LoongArch Emulator 项目分析总结

这个项目是一个独立的 LoongArch 模拟器，主体是 C 写的解释执行器，不是完整 QEMU fork。它复用了不少 QEMU 风格的结构、CSR 位域宏、softfloat、gdbstub 思路，但核心执行路径是自己的 `fetch -> interpreter -> trans_*`。

## 整体结构

- 构建入口：`Makefile`
- 主程序、启动、ELF 加载、执行循环：`src/core/main.c`
- 指令解释实现：`src/core/interpreter.c`
- CPU 状态、CSR、TLB、指令缓存结构：`include/cpu.h`
- MMU/TLB 翻译：`src/core/cpu_helper.c`、`src/mmu/tlb_helper.c`
- 用户态 syscall：`src/syscall/syscall.c`
- 插件：`plugins/`
- 项目内已有详细报告：`ANALYSIS_REPORT.md`

## 执行模型

主循环在 `exec_env()`。每轮会处理 CLI、GDB、IRQ，然后 `fetch()` 取指，调用 `interpreter()` 解码执行，最后更新 `icount`。异常通过 `do_raise_exception()` 触发 `siglongjmp`，回到主循环外层后进入 `loongarch_cpu_do_interrupt()` 处理。

取指路径有两级缓存：

- `tc_fetch/tc_load/tc_store`：按页缓存 VA 到 PA，减少 MMU 翻译开销。
- `inscache[16K]`：按 PC 索引缓存已解码的 `trans_func + arg[4]`，命中时跳过译码，直接执行函数指针。

## 支持能力

README 标称支持：

- LoongArch64 base / privilege
- FP / LSX / LASX
- Timer / Serial
- GDB server
- deterministic events
- SPEC CPU
- user mode 和 kernel mode

从源码看，kernel mode 是主路线；user mode 有 ELF loader、栈构造和 syscall 转发，但 README 也明确动态 ELF、信号、多线程不支持。

## CPU/架构配置

默认 `CORE := openc910`，不是 README 里写的默认 `la464`。代码里支持 `la464`、`centaur320`、`openc910`，还有 `loongarch32r/loongarch32s` 的痕迹。CPU 配置主要写在 `src/core/cpu.c`。

## 当前明显问题

1. `make -j2` 当前会失败，不是主程序失败，而是插件失败。
   顶层 Makefile 总是执行 `make -C plugins -j`。插件源码包含 `../plugin.h`，但实际头文件在 `include/plugin.h`，所以干净构建会卡住。

2. 参数解析有 fallthrough，看起来像 bug。
   `-v` 会继续落到 `-b`，再落到 `-w`；`-b` 也会落到 `-w`。结果是开启 verbose 会顺带打开 BIOS 模式和硬件页表遍历。

3. 部分指令未实现或正确性有风险。
   项目自己的 `docs/INSN_ANALYSIS.md` 已指出边界检查访存类指令未实现、div/mod 除零/溢出处理风险、FPU 异常标志可能不完整。

4. user mode 能跑静态 ELF 的可能性较高，但动态 ELF 明确不支持。
   `PT_INTERP` 会直接打印 unsupported dynamic elf 并退出。

## 验证结果

运行 `make -j2` 时，已有 `build/la_emu_kernel`，但完整构建失败在插件阶段，错误是 `../plugin.h: No such file or directory`。所以当前仓库处于“主二进制已存在，但从源码完整构建不干净”的状态。

## 总体判断

这是一个偏研究/验证用途的 LoongArch64 解释器，核心路径清晰，kernel mode、MMU、CSR、GDB/difftest 都有框架；但工程化质量还有明显缺口，尤其是构建、参数解析、未实现指令和边界行为。下一步最值得先修的是插件构建和 `getopt` fallthrough，这两个属于低成本高收益问题。
