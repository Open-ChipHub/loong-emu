# Diffnet 开发记录

## 2026-05-23 — 初始实现

### QRSP GDB RSP 客户端库 (`tools/qrsp/`)

- `qrsp.h` / `qrsp.c` — 零依赖 GDB RSP 协议实现
- 支持 `$...#XX` 包格式, ACK/NAK 握手, 校验和
- 高级 API: `qrsp_connect/disconnect`, `qrsp_step`, `qrsp_continue`,
  `qrsp_read_g_packet`, `qrsp_read_register`, `qrsp_read_memory`,
  `qrsp_write_memory`, `qrsp_set/remove_breakpoint`

### 网络差分测试 (`src/difftest/diffnet.c`, `include/diffnet.h`)

- `diffnet_init(host, port, env)` — 连接已有 QEMU, 同步初始 GPR
- `diffnet_spawn(qemu_path, kernel, addr, port, env)` — fork 子进程跑 QEMU, 轮询等待 GDB 就绪, 连接
- `diffnet_check(env)` — 每条指令后调用, batch 边界时 step QEMU + 比对
- `diffnet_step_exception(env)` — 异常路径同步
- `diffnet_cleanup()` — kill 子进程 + 断开

### main.c 集成 (`src/core/main.c`)

- `-N PORT` — spawn 模式 (自动启动 QEMU)
- `-N HOST:PORT` — connect 模式 (连接已有 QEMU)
- 环境变量: `DIFFNET_MAX_STEPS`, `DIFFNET_BATCH_SIZE`, `DIFFNET_CMP_MASK`,
  `DIFFNET_CSR_DUMP_ADDR`

### Python 脚本 (`scripts/diffnet.py`)

- `emu` 模式 — 运行 C 模拟器 (推荐)
- `step` 模式 — 纯 Python GDB RSP 逐条步进
- `batch` 模式 — 纯 Python GDB RSP 批量步进

### 扩展比对范围

| 类别 | 掩码 | 访问方式 | 状态 |
|------|------|---------|------|
| GPR r0-r31 + PC | CMP_GPR | `g` packet | 始终 |
| FPR f0-f31 + FCC + FCSR | CMP_FPR | `p35`-`p75` | cpucfg.FP=1 |
| LSX vr0-31 | CMP_LSX | `p76`-`p107` | cpucfg.LSX=1 |
| LASX xr0-31 | CMP_LASX | `p108`-`p139` | cpucfg.LASX=1 |
| CSR (22个) | CMP_CSR | 跳板 + `m` packet | --ctxcmp csr |
| 内存输出 | CMP_MEM | `m` packet | --ctxcmp mem |

### CSR 跳板机制

预编码的 LoongArch CSR dump blob (180 字节, 45 条指令):
- 来源: `tests/dump_csr.S` → 交叉汇编 → `xxd -i` → 嵌入 `diffnet.c`
- 基地址: `--csr-dump-addr` / `DIFFNET_CSR_DUMP_ADDR` (默认 0x1c000300)
- 代码 + 数据紧邻: [0..179] 代码, [180..355] 数据
- emu 侧: memcpy 保存→注入→interpreter 执行→恢复
- QEMU 侧: 'm' 读→'M' 写→'P' 设寄存器→44 次 's'→'m' 读→恢复

### 测试程序 (`tests/`)

- `test_add.S` — ALU 循环 (5000 迭代 × 11 指令)
- `test_branch.S` — 分支指令
- `test_memory.S` — 访存指令
- `test_full.S` — 整数 + CSR dump
- `dump_csr.S` — CSR dump 跳板源码 (用于生成 hex blob)
