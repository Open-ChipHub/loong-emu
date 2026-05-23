# Vector instruction diffnet test.
# All vectors start as zero (same in emu and QEMU).
# Each test block: 100 iterations × 10 vector ops = 1000 instructions.
# Stores result vr2.D[0] to memory after each block.

.globl _start
_start:
    ori     $t0, $zero, 0
    ori     $t1, $zero, 100

    lu12i.w $s0, 0x1c000
    ori     $s0, $s0, 0x100

    # ── Test 1: vadd.b (100×10) ──
1:  vadd.b  $vr2, $vr0, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    vadd.b  $vr2, $vr2, $vr1
    addi.w  $t0, $t0, 1
    blt     $t0, $t1, 1b
    movfr2gr.d $t2, $f2
    st.d    $t2, $s0, 0
    ori     $t0, $zero, 0

    # ── Test 2: vsub.h (100×10) ──
2:  vsub.h  $vr2, $vr0, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    vsub.h  $vr2, $vr2, $vr1
    addi.w  $t0, $t0, 1
    blt     $t0, $t1, 2b
    movfr2gr.d $t2, $f2
    st.d    $t2, $s0, 8
    ori     $t0, $zero, 0

    # ── Test 3: vadd.w (100×10) ──
3:  vadd.w  $vr2, $vr0, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    vadd.w  $vr2, $vr2, $vr1
    addi.w  $t0, $t0, 1
    blt     $t0, $t1, 3b
    movfr2gr.d $t2, $f2
    st.d    $t2, $s0, 16
    ori     $t0, $zero, 0

    # ── Test 4: vadd.d (100×10) ──
4:  vadd.d  $vr2, $vr0, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    vadd.d  $vr2, $vr2, $vr1
    addi.w  $t0, $t0, 1
    blt     $t0, $t1, 4b
    movfr2gr.d $t2, $f2
    st.d    $t2, $s0, 24

done:
    addi.w  $zero, $zero, 0
    b       done
