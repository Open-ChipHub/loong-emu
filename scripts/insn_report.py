#!/usr/bin/env python3
"""
LoongArch Emu — Instruction Execution Statistics Reporter.

Generates a Markdown report with:
  - Exception / interrupt statistics
  - Per-category instruction counts (ISA manual chapters)
  - Per-instruction execution counts (hex encoding)
  - Complete ISA reference (all 1690 instructions from trans_la.c.inc)

Usage:
  ./scripts/insn_report.py -k tests/test_add.bin
  ./scripts/insn_report.py -k vmlinux -m 16G --max-cycles 500000
  ./scripts/insn_report.py -k test.elf -m 16G -o my_report.md
"""

import argparse, os, subprocess, sys, time

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EMU_PATH = os.path.join(PROJECT_ROOT, "build", "la_emu_kernel")
DEFAULT_ADDR = "0x1c000000"


def main():
    parser = argparse.ArgumentParser(
        description="LoongArch Emu — Instruction Execution Statistics Reporter")
    parser.add_argument("--binary", "-k", required=True, help="Binary/ELF/kernel to run")
    parser.add_argument("--addr", "-E", default=DEFAULT_ADDR, help="Load address for BIOS mode")
    parser.add_argument("--max-cycles", "-M", type=int, default=100000,
                        help="Max instructions (default: 100000)")
    parser.add_argument("--output", "-o", default="report_instruction.md",
                        help="Output Markdown file (default: report_instruction.md)")
    parser.add_argument("--bios", "-b", action="store_true",
                        help="Load as raw binary (BIOS mode), use --addr for entry")
    parser.add_argument("--memory", "-m", default=None, help="Memory size, e.g. 16G")
    parser.add_argument("--emu", default=EMU_PATH, help="Path to la_emu_kernel")

    args = parser.parse_args()

    binary = os.path.abspath(args.binary)
    if not os.path.exists(binary):
        print(f"ERROR: binary not found: {binary}")
        sys.exit(1)
    if not os.path.exists(args.emu):
        print(f"ERROR: emu not found at {args.emu}. Build with: make -j")
        sys.exit(1)

    # Build command line
    cmd = [args.emu, "-z", "-k", binary, "-R", args.output]
    if args.bios:
        cmd += ["-b", "-E", args.addr]
    if args.memory:
        cmd += ["-m", args.memory]

    env = os.environ.copy()
    env["EMU_MAX_INSTRS"] = str(args.max_cycles)

    print(f"=== Instruction Stats Reporter ===")
    print(f"  emu:     {args.emu}")
    print(f"  binary:  {binary}")
    print(f"  memory:  {args.memory or 'default (4G)'}")
    print(f"  mode:    {'BIOS' if args.bios else 'ELF/kernel'}")
    print(f"  max insn:{args.max_cycles}")
    print(f"  output:  {args.output}")
    print()

    start = time.time()
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, env=env)

    for line in proc.stdout:
        line = line.rstrip()
        if any(kw in line for kw in ("insn_stats", "ERROR", "FAIL", "assert")):
            print(f"  {line}")

    proc.wait()
    elapsed = time.time() - start

    if os.path.exists(args.output):
        with open(args.output) as f:
            for line in f:
                if line.startswith("**Total") or line.startswith("**Distinct"):
                    print(f"  {line.strip()}")
        size = os.path.getsize(args.output)
        print(f"\nReport: {args.output} ({size:,} bytes, {elapsed:.1f}s)")
        return 0
    else:
        print(f"\nERROR: report not generated (emu exit={proc.returncode})")
        return 1


if __name__ == "__main__":
    sys.exit(main())
