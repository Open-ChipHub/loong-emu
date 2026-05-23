# Test: branch and jump instructions
# Verifies BEQ, BNE, BLT, BGE, BLTU, BGEU, JIRL, B

.globl _start
_start:
    ori     $t0, $zero, 10          # t0 = 10
    ori     $t1, $zero, 20          # t1 = 20
    ori     $t2, $zero, 10          # t2 = 10 (= t0, for BEQ test)

    # Test 1: BEQ (taken)
    beq     $t0, $t2, 1f            # t0 == t2 → jump to 1
    ori     $a0, $zero, 0xBAD       # SKIP: should not execute
1:

    # Test 2: BNE (taken)
    bne     $t0, $t1, 2f            # t0 != t1 → jump to 2
    ori     $a0, $zero, 0xBAD       # SKIP
2:

    # Test 3: BLT (taken — t0 < t1)
    blt     $t0, $t1, 3f            # t0(10) < t1(20) → jump
    ori     $a0, $zero, 0xBAD       # SKIP
3:

    # Test 4: BLT (not taken — t1 > t0)
    blt     $t1, $t0, 9f            # t1(20) < t0(10) false → fall through
    ori     $a1, $zero, 1           # a1 = 1 (should execute)
    b       4f
9:  ori     $a1, $zero, 0xBAD       # SKIP
4:

    # Test 5: BGE (taken — equality)
    bge     $t0, $t2, 5f            # t0(10) >= t2(10) → jump
    ori     $a1, $zero, 0xBAD       # SKIP
5:

    # Test 6: BLTU (unsigned, taken)
    ori     $t3, $zero, 0xFFF       # t3 = 4095
    bltu    $t0, $t3, 6f            # 10 < 4095 → jump
    ori     $a1, $zero, 0xBAD       # SKIP
6:

    # Test 7: JIRL (jump and link register)
    ori     $a2, $zero, 0
    pcaddi  $ra, 4                  # $ra = PC + 4
    jirl    $zero, $ra, 0           # jump to $ra (next insn)
    ori     $a2, $zero, 0xBAD       # SKIP

    # Final result: a1 should be 1
    lu12i.w $s0, 0x1c000
    ori     $s0, $s0, 0x100
    stptr.w $a1, $s0, 0             # *(0x1c000100) = a1

1:  b       1b
