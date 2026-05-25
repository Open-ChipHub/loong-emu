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

## LA32R 内核启动到 clocksource 后无输出的问题记录

目标内核源码路径：

- `/home/airxs/user/loongsonedu/linux/linux-6.11.0-la32r-emu`

可用启动命令：

```bash
./build/la_emu_kernel -z -m 16 -A loongarch32r -k /home/airxs/user/loongsonedu/linux/linux-6.11.0-la32r-emu/vmlinux
```

注意：不要加 `-s`。当前 `serial_plus` 路径会在 `serial_check_io()` 中 abort；普通串口路径可以输出内核日志。

### 现象

未开启 initcall 调试时，日志最后常见输出是：

```text
clocksource: Switched to clocksource Constant
```

看起来像卡在 clocksource 切换，但这是误判。给内核 bootargs 加上 `initcall_debug` 后可以看到：

```text
calling  clocksource_done_booting+0x0/0x50 @ 1
clocksource: Switched to clocksource Constant
initcall clocksource_done_booting+0x0/0x50 returned 0 after 75 usecs
...
calling  jent_mod_init+0x0/0x114 @ 1
random: crng init done
```

也就是说，`clocksource_done_booting()` 已经正常返回，真正长时间不返回的是后面的 `jent_mod_init()`。

### 根因判断

`jent_mod_init()` 来自内核 Jitter RNG：

- `crypto/jitterentropy-kcapi.c`
- 其初始化调用 `jent_entropy_init(CONFIG_CRYPTO_JITTERENTROPY_OSR, 0, desc, NULL)`

Jitter RNG 依赖 CPU/timer 的微小时间抖动作为熵源。当前模拟器的时间源和执行节奏过于确定，缺少真实硬件上的自然 jitter，导致 Jitter RNG 初始化/健康检测在模拟器中长时间无法完成。因此它表现为启动卡住。

配置上也不能简单只关 `CONFIG_CRYPTO_JITTERENTROPY`，因为 Kconfig 依赖会重新选中它：

```text
CRYPTO_RNG_DEFAULT -> CRYPTO_DRBG_MENU -> CRYPTO_DRBG -> CRYPTO_JITTERENTROPY
```

对应位置：

- `crypto/Kconfig:112`：`CRYPTO_RNG_DEFAULT` select `CRYPTO_DRBG_MENU`
- `crypto/Kconfig:1237`：`CRYPTO_DRBG`
- `crypto/Kconfig:1241`：`CRYPTO_DRBG` select `CRYPTO_JITTERENTROPY`

### 当前验证修复

在内核 DTS 中给 bootargs 添加 initcall 黑名单，跳过 `jent_mod_init`：

文件：

- `/home/airxs/user/loongsonedu/linux/linux-6.11.0-la32r-emu/arch/loongarch/boot/dts/loongson32_emu.dts`

当前 bootargs：

```dts
bootargs = "console=ttyS0,115200 init=/init loglevel=8 initcall_debug initcall_blacklist=jent_mod_init swiotlb=64 dhash_entries=16384 ihash_entries=16384";
```

重新编译内核：

```bash
cd /home/airxs/user/loongsonedu/linux/linux-6.11.0-la32r-emu
make -j2
```

验证日志：

```text
blacklisting initcall jent_mod_init
...
clocksource: Switched to clocksource Constant
initcall clocksource_done_booting+0x0/0x50 returned 0 after 75 usecs
...
initcall jent_mod_init blacklisted
calling  io_uring_init+0x0/0x118 @ 1
```

这证明 `clocksource` 不是卡点，跳过 `jent_mod_init` 后内核能继续执行后续 initcall。

### 后续建议

短期为了启动内核，可以继续使用：

```text
initcall_blacklist=jent_mod_init
```

长期更合理的方向有三个：

1. 在模拟器中提供可用随机源，例如 virtio-rng/hwrng 或宿主随机数注入。
2. 让模拟器 timer/rdtime 行为更接近真实硬件，至少能满足 Jitter RNG 的熵源检测。
3. 为 emu 内核配置裁剪 crypto RNG 依赖，避免内建 Jitter RNG；但需要处理 `CRYPTO_RNG_DEFAULT` 和 `CRYPTO_DRBG` 的依赖链。

跳过 `jent_mod_init` 后，90 秒限时测试已经能执行到 late initcall 后段，最后无输出位置已经不是本次 `clocksource/Jitter RNG` 问题，需要后续单独定位是否卡在 `async_synchronize_full()`、释放 initmem、rootfs 挂载或进入 `/init` 后无输出。
