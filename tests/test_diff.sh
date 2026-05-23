#!/bin/bash
# Quick test: start QEMU, run emu with diffnet, verify no mismatches
set -e

KILL() { kill -9 $(pgrep -f "qemu-system-loongarch") 2>/dev/null; true; }
KILL

QEMU=/home/airxs/user/qemu/qemu-10.1.2/build/qemu-system-loongarch64
EMU=./build/la_emu_kernel
BIN=tests/test_add.bin

echo "Starting QEMU..."
$QEMU -M virt -device loader,file=$BIN,addr=0x1c000000 -s -S \
  -nographic -monitor none -display none &
QPID=$!
sleep 3

echo "QEMU PID=$QPID"
if ! kill -0 $QPID 2>/dev/null; then
  echo "FAIL: QEMU failed to start"
  exit 1
fi

echo "Running emu with diffnet..."
timeout 10 $EMU -z -k $BIN -b -E 0x1c000000 -N 127.0.0.1:1234 2>&1
RET=$?

kill $QPID 2>/dev/null; wait $QPID 2>/dev/null; true

if [ $RET -eq 0 ]; then
  echo "PASS: no difftest mismatches"
else
  echo "FAIL: difftest errors (exit $RET)"
  exit 1
fi
