#!/bin/bash
# Run a LoongArch test binary through both QEMU and emu with diffnet.
# Usage: ./run.sh <test.bin>
#
# Requires: qemu-system-loongarch64, la_emu_kernel (built with DIFF_NET=1)

set -e

QEMU="/home/airxs/user/qemu/qemu-10.1.2/build/qemu-system-loongarch64"
EMU="../build/la_emu_kernel"
TEST_BIN="$1"

if [ -z "$TEST_BIN" ]; then
    echo "Usage: $0 <test.bin>"
    exit 1
fi

if [ ! -f "$QEMU" ]; then
    echo "QEMU not found at $QEMU"
    exit 1
fi

if [ ! -f "$EMU" ]; then
    echo "emu not found at $EMU — build with: make DIFF_NET=1 -j"
    exit 1
fi

echo "=== QRSP Diffnet Test ==="
echo "Test binary: $TEST_BIN"
echo ""

# Kill any leftover QEMU from previous runs
pkill -f "qemu-system-loongarch64" 2>/dev/null || true
sleep 0.5

# Start QEMU in background with GDB server, stopped at first instruction
echo "[1/3] Starting QEMU with GDB server on :1234 ..."
$QEMU -M virt -bios "$TEST_BIN" -s -S -nographic -monitor none &
QEMU_PID=$!
sleep 2

# Verify QEMU is listening
if ! kill -0 $QEMU_PID 2>/dev/null; then
    echo "ERROR: QEMU failed to start"
    exit 1
fi

echo "[2/3] Running emu with diffnet..."
set +e
$EMU -z -k "$TEST_BIN" -b -E 0x1c000000 -N localhost:1234
EMU_RET=$?
set -e

echo ""
echo "[3/3] Cleaning up..."
kill $QEMU_PID 2>/dev/null
wait $QEMU_PID 2>/dev/null

if [ $EMU_RET -eq 0 ]; then
    echo "=== PASS: No difftest mismatches ==="
    exit 0
else
    echo "=== FAIL: difftest reported errors (exit code $EMU_RET) ==="
    exit 1
fi
