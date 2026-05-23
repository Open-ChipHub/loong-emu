# Test: basic ALU operations (ADD, SUB, MUL, DIV, bitwise ops)
# All instructions use direct addressing (DA=1, no TLB).
# Final result stored at 0x1c000100 for external verification.

.globl _start
_start:
    # Test 1: addi.w — immediate add word (12-bit signed imm)
    addi.w  $t0, $zero, 42          # t0 = 42
    addi.w  $t1, $zero, 58          # t1 = 58

    # Test 2: add.w — register add word
    add.w   $t2, $t0, $t1           # t2 = 100

    # Test 3: sub.w — register sub word
    sub.w   $t3, $t1, $t0           # t3 = 58 - 42 = 16

    # Test 4: ori — 12-bit unsigned immediate
    ori     $t4, $zero, 0x555       # t4 = 0x555
    ori     $t5, $zero, 0x777       # t5 = 0x777

    # Test 5: mul.w — multiply word
    mul.w   $t6, $t0, $t1           # t6 = 42 * 58 = 2436

    # Test 6: div.w — signed divide word
    div.w   $t7, $t1, $t0           # t7 = 58 / 42 = 1

    # Test 7: and / or / xor / nor
    and     $t8, $t4, $t5           # t8 = 0x555 & 0x777 = 0x555
    or      $a0, $t4, $t5           # a0 = 0x555 | 0x777 = 0x777
    xor     $a1, $t4, $t5           # a1 = 0x555 ^ 0x777 = 0x222
    nor     $a2, $t4, $t5           # a2 = ~(0x555 | 0x777)

    # Test 8: slli.d / srli.d — shifts
    slli.d  $a3, $t0, 4             # a3 = 42 << 4 = 672
    srli.d  $a4, $a3, 2             # a4 = 672 >> 2 = 168

    # Store result (t2 = 100) at output area
    lu12i.w $s0, 0x1c000            # s0 = 0x1c000 << 12 = 0x1c000000
    ori     $s0, $s0, 0x100         # s0 = 0x1c000100
    stptr.w $t2, $s0, 0             # *(0x1c000100) = 100

    # Loop forever
1:  b       1b
