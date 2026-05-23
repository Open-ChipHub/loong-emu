## 介绍

## 运行方式

### 编译
make DIFF_NET=1 -j

### 终端1: 启动QEMU (GDB stub监听1234)
qemu-system-loongarch64 -M virt -device loader,file=tests/test_add.bin,addr=0x1c000000 -s -S

### 终端2: 运行emu (逐指令与QEMU比对)
./build/la_emu_kernel -z -k tests/test_add.bin -b -E 0x1c000000 -N 127.0.0.1:1234

### 期望输出: 0 mismatches, emu正常退出

验证结果

DIFFNET: Connected to QEMU at 127.0.0.1:1234
DIFFNET: synced GPRs from QEMU, QEMU PC=0x000000001c000000 emu PC=0x000000001c000000
(执行17+条指令, 0 mismatches)
EXIT: 0


