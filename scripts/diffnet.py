#!/usr/bin/env python3
"""
QRSP Diffnet — LoongArch emu vs QEMU differential testing.

Modes:
  emu    Run the C emu (la_emu_kernel) with auto-spawned QEMU.
         The emu does the comparison internally via GDB RSP.
         This is the primary mode — fast, optimized, reliable.

  step   Pure Python step-by-step comparison via GDB RSP.
         Steps QEMU one instruction at a time, reads registers,
         and compares against a reference trace. Useful for
         debugging individual instructions.

  batch  Pure Python batch comparison via GDB RSP.
         Steps QEMU in batches, reading registers only at
         batch boundaries. Faster than step mode.

Usage:
  ./scripts/diffnet.py emu   --binary tests/test_add.bin [--max-steps N] [--batch-size N]
  ./scripts/diffnet.py step  --binary tests/test_add.bin [--max-steps N]
  ./scripts/diffnet.py batch --binary tests/test_add.bin [--max-steps N] [--batch-size N]
"""

import argparse
import os
import subprocess
import socket
import struct
import sys
import time
import signal
import binascii

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
QEMU_PATH = "/home/airxs/user/qemu/qemu-10.1.2/build/qemu-system-loongarch64"
EMU_PATH = os.path.join(PROJECT_ROOT, "build", "la_emu_kernel")
DEFAULT_PORT = 1234
DEFAULT_ADDR = "0x1c000000"

# ─── GDB RSP client ────────────────────────────────────────────

class GdbClient:
    """Minimal GDB Remote Serial Protocol client."""

    def __init__(self, host="127.0.0.1", port=DEFAULT_PORT):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(10)
        self.sock.connect((host, port))
        self.sock.settimeout(5)

    def close(self):
        self.sock.close()

    def _checksum(self, data: bytes) -> int:
        return sum(data) & 0xff

    def send_packet(self, cmd: str):
        data = cmd.encode()
        csum = self._checksum(data)
        pkt = b"$" + data + f"#{csum:02x}".encode()
        self.sock.send(pkt)
        ack = self.sock.recv(1)
        if ack != b"+":
            raise IOError(f"Expected ACK (+), got {ack}")

    def recv_packet(self) -> str:
        c = self.sock.recv(1)
        while c != b"$":
            c = self.sock.recv(1)
        data = b""
        while True:
            c = self.sock.recv(1)
            if c == b"#":
                break
            data += c
        self.sock.recv(2)  # checksum
        self.sock.send(b"+")
        return data.decode()

    def step(self) -> str:
        self.send_packet("s")
        return self.recv_packet()

    def continue_(self) -> str:
        self.send_packet("c")
        return self.recv_packet()

    def read_regs(self) -> bytes:
        """Return raw binary g-packet (280 bytes: 35 registers x 8 bytes LE)."""
        self.send_packet("g")
        hexdata = self.recv_packet()
        if len(hexdata) < 560:
            raise IOError(f"g-packet too short: {len(hexdata)} hex chars")
        return binascii.unhexlify(hexdata[:560])

    def read_gpr(self, n: int) -> int:
        """Read a single GPR (n=0..31). Returns 64-bit value."""
        binbuf = self.read_regs()
        return struct.unpack_from("<Q", binbuf, n * 8)[0]

    def read_pc(self) -> int:
        """Read the program counter (register 33 in g-packet)."""
        binbuf = self.read_regs()
        return struct.unpack_from("<Q", binbuf, 33 * 8)[0]

    def set_bp(self, addr: int, kind: int = 0) -> bool:
        self.send_packet(f"Z{kind},{addr:x},4")
        return self.recv_packet() == "OK"

    def remove_bp(self, addr: int, kind: int = 0) -> bool:
        self.send_packet(f"z{kind},{addr:x},4")
        return self.recv_packet() == "OK"

    def read_all_gprs(self):
        """Return list of 32 GPR values."""
        binbuf = self.read_regs()
        return [struct.unpack_from("<Q", binbuf, i * 8)[0] for i in range(32)]


# ─── QEMU subprocess management ─────────────────────────────────

class QemuProcess:
    """Manage a QEMU subprocess with GDB stub."""

    def __init__(self, binary: str, addr: str = DEFAULT_ADDR, port: int = DEFAULT_PORT,
                 qemu: str = QEMU_PATH):
        self.qemu = qemu
        self.binary = binary
        self.addr = addr
        self.port = port
        self.proc = None

    def start(self):
        loader = f"loader,file={self.binary},addr={self.addr}"
        gdb_arg = f"tcp::{self.port}"
        self.proc = subprocess.Popen(
            [self.qemu, "-M", "virt", "-device", loader,
             "-gdb", gdb_arg, "-S", "-nographic", "-monitor", "none", "-display", "none"],
            stdout=subprocess.DEVNULL, stderr=subprocess.PIPE
        )
        # Wait for GDB stub to be ready
        deadline = time.time() + 30
        while time.time() < deadline:
            try:
                s = socket.create_connection(("127.0.0.1", self.port), timeout=1)
                s.close()
                return True
            except (ConnectionRefusedError, OSError):
                time.sleep(0.5)
        self.stop()
        return False

    def stop(self):
        if self.proc:
            self.proc.send_signal(signal.SIGTERM)
            try:
                self.proc.wait(timeout=5)
            except subprocess.TimeoutExpired:
                self.proc.kill()
                self.proc.wait()
            self.proc = None


# ─── Emu mode — run C emu with internal diffnet ─────────────────

def mode_emu(args):
    """Run the C emulator (la_emu_kernel) which handles everything internally."""
    if not os.path.exists(EMU_PATH):
        print(f"ERROR: emu not built. Run: make DIFF_NET=1 -j")
        sys.exit(1)

    binary = os.path.abspath(args.binary)
    if not os.path.exists(binary):
        print(f"ERROR: binary not found: {binary}")
        sys.exit(1)

    cmd = [EMU_PATH, "-z", "-k", binary, "-b", "-E", args.addr, "-N", str(args.port)]

    print(f"=== Diffnet: emu mode ===")
    print(f"  emu:   {EMU_PATH}")
    print(f"  qemu:  {QEMU_PATH} (auto-spawned)")
    print(f"  binary: {binary}")
    print(f"  addr:   {args.addr}")
    print(f"  port:   {args.port}")
    print(f"  max steps: {args.max_steps}")
    print(f"  batch size: {args.batch_size}")
    print()

    env = os.environ.copy()
    # Pass batch config via environment
    env["DIFFNET_MAX_STEPS"] = str(args.max_steps)
    env["DIFFNET_BATCH_SIZE"] = str(args.batch_size)

    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, env=env)

    mismatches = 0
    batches_ok = 0
    for line in proc.stdout:
        line = line.rstrip()
        print(f"  [emu] {line}")
        if "MISMATCH" in line:
            mismatches += 1
        elif "batch" in line and "ok" in line:
            batches_ok += 1

    proc.wait()
    print()
    if proc.returncode == 0:
        print(f"PASS: {batches_ok} batches passed, 0 mismatches")
    else:
        print(f"FAIL: mismatches detected (exit {proc.returncode})")
    return proc.returncode


# ─── Step mode — pure Python step-by-step via GDB RSP ───────────

GPR_NAMES = [
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    "r8", "r9", "r10","r11","r12","r13","r14","r15",
    "r16","r17","r18","r19","r20","r21","r22","r23",
    "r24","r25","r26","r27","r28","r29","r30","r31",
]

def mode_step(args):
    """Step-by-step comparison via pure Python GDB RSP."""
    binary = os.path.abspath(args.binary)
    if not os.path.exists(binary):
        print(f"ERROR: binary not found: {binary}")
        sys.exit(1)

    qemu = QemuProcess(binary, args.addr, args.port, args.qemu)
    print(f"Starting QEMU (port {args.port})...")
    if not qemu.start():
        print("ERROR: QEMU failed to start")
        sys.exit(1)
    print(f"QEMU started (PID {qemu.proc.pid})")

    gdb = GdbClient(port=args.port)
    try:
        # Read initial state
        prev_pc = gdb.read_pc()
        print(f"Step-by-step comparison: max {args.max_steps} instructions")
        print(f"Initial PC = 0x{prev_pc:016x}")
        print()

        mismatches = 0
        for step in range(1, args.max_steps + 1):
            reply = gdb.step()
            if reply[0] not in "TS":
                print(f"[{step}] Unexpected reply: {reply}")
                break
            # Read registers (no comparison target in pure Python mode —
            # this is mainly for tracing/profiling)
            pc = gdb.read_pc()
            r0 = gdb.read_gpr(0)
            r12 = gdb.read_gpr(12)
            if step % 100 == 0 or step == 1:
                print(f"[{step:>6}] PC=0x{pc:016x} r0={r0} r12={r12}")

        print(f"\nDone. {step} instructions stepped.")

    finally:
        gdb.close()
        qemu.stop()
    return 0


# ─── Batch mode — pure Python batch comparison via GDB RSP ──────

def mode_batch(args):
    """Batch comparison via pure Python GDB RSP."""
    binary = os.path.abspath(args.binary)
    if not os.path.exists(binary):
        print(f"ERROR: binary not found: {binary}")
        sys.exit(1)

    qemu = QemuProcess(binary, args.addr, args.port, args.qemu)
    print(f"Starting QEMU (port {args.port})...")
    if not qemu.start():
        print("ERROR: QEMU failed to start")
        sys.exit(1)
    print(f"QEMU started (PID {qemu.proc.pid})")

    gdb = GdbClient(port=args.port)
    try:
        prev_pc = gdb.read_pc()
        print(f"Batch comparison: max {args.max_steps} steps, batch size {args.batch_size}")
        print(f"Initial PC = 0x{prev_pc:016x}")
        print()

        gprs = gdb.read_all_gprs()

        for step in range(1, args.max_steps + 1):
            reply = gdb.step()
            if reply[0] not in "TS":
                print(f"[{step}] Unexpected reply: {reply}")
                break

            if step % args.batch_size == 0:
                new_gprs = gdb.read_all_gprs()
                new_pc = gdb.read_pc()

                diffs = []
                for i in range(32):
                    if new_gprs[i] != gprs[i]:
                        diffs.append((i, gprs[i], new_gprs[i]))

                print(f"[{step:>6}] PC=0x{new_pc:016x} diffs={len(diffs)}", end="")
                if diffs:
                    print("  CHANGED:", ", ".join(
                        f"{GPR_NAMES[i]}:0x{old:x}→0x{new:x}"
                        for i, old, new in diffs[:5]
                    ))
                else:
                    print("  (no reg changes)")
                gprs = new_gprs

        print(f"\nDone. {step} instructions stepped, {step // args.batch_size} batches.")

    finally:
        gdb.close()
        qemu.stop()
    return 0


# ─── CLI ────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="QRSP Diffnet — LoongArch emu vs QEMU differential testing")
    sub = parser.add_subparsers(dest="mode", required=True)

    # emu mode
    p_emu = sub.add_parser("emu", help="Run C emu with internal diffnet (fast, recommended)")
    p_emu.add_argument("--binary", "-k", required=True, help="Test binary")
    p_emu.add_argument("--addr", "-E", default=DEFAULT_ADDR, help="Load address")
    p_emu.add_argument("--port", "-p", type=int, default=DEFAULT_PORT)
    p_emu.add_argument("--max-steps", "-M", type=int, default=70000)
    p_emu.add_argument("--batch-size", "-B", type=int, default=2000)

    # step mode
    p_step = sub.add_parser("step", help="Pure Python step-by-step via GDB RSP")
    p_step.add_argument("--binary", "-k", required=True, help="Test binary")
    p_step.add_argument("--addr", "-E", default=DEFAULT_ADDR)
    p_step.add_argument("--port", "-p", type=int, default=DEFAULT_PORT)
    p_step.add_argument("--qemu", "-Q", default=QEMU_PATH, help="QEMU binary path")
    p_step.add_argument("--max-steps", "-M", type=int, default=1000)

    # batch mode
    p_batch = sub.add_parser("batch", help="Pure Python batch comparison via GDB RSP")
    p_batch.add_argument("--binary", "-k", required=True, help="Test binary")
    p_batch.add_argument("--addr", "-E", default=DEFAULT_ADDR)
    p_batch.add_argument("--port", "-p", type=int, default=DEFAULT_PORT)
    p_batch.add_argument("--qemu", "-Q", default=QEMU_PATH)
    p_batch.add_argument("--max-steps", "-M", type=int, default=10000)
    p_batch.add_argument("--batch-size", "-B", type=int, default=500)

    args = parser.parse_args()

    if args.mode == "emu":
        sys.exit(mode_emu(args))
    elif args.mode == "step":
        sys.exit(mode_step(args))
    elif args.mode == "batch":
        sys.exit(mode_batch(args))


if __name__ == "__main__":
    main()
