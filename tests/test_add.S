# Test: 100K instructions — ALU ops in a tight loop
# Optimized for batch difftest: has a unique "done" PC for breakpoint.

.globl _start
_start:
    ori     $t0, $zero, 0           # loop counter
    lu12i.w $t1, 0x1                # t1 = 0x1000
    ori     $t1, $t1, 0x388          # t1 = 0x1388 = 5000
    ori     $t2, $zero, 0           # accumulator

loop:
    addi.w  $t3, $t0, 1
    add.w   $t4, $t2, $t3
    sub.w   $t5, $t4, $t0
    and     $t6, $t5, $t3
    or      $t7, $t6, $t0
    xor     $t8, $t7, $t5
    slli.w  $t2, $t8, 1
    srli.w  $t2, $t2, 1
    add.w   $t2, $t2, $t6
    addi.w  $t0, $t0, 1
    blt     $t0, $t1, loop

    # Store result and signal completion
    lu12i.w $s0, 0x1c000
    ori     $s0, $s0, 0x100
    stptr.w $t2, $s0, 0

done:
    # Unique PC — breakpoint target. Keep spinning here.
    addi.w  $zero, $zero, 0
    b       done
