# Test: load/store instructions
# Uses only basic LD/ST variants

.globl _start
_start:
    # Set up base address = 0x1c000000
    lu12i.w $t0, 0x1c000

    # Test 1: ST.B + LD.B — byte store/load
    ori     $t1, $zero, 0xAB
    st.b    $t1, $t0, 0
    ld.b    $t2, $t0, 0
    xor     $a0, $t2, $t1           # should be 0

    # Test 2: ST.H + LD.H — halfword
    ori     $t1, $zero, 0x789
    st.h    $t1, $t0, 0x10
    ld.h    $t2, $t0, 0x10
    xor     $a1, $t2, $t1           # should be 0

    # Test 3: ST.W + LD.W — word
    lu12i.w $t1, 0x1234
    ori     $t1, $t1, 0x567
    st.w    $t1, $t0, 0x20
    ld.w    $t2, $t0, 0x20
    # compare: (t1 xor t2)[31:0] should be 0
    # use slli.d + srli.d to clear upper bits
    slli.d  $t1, $t1, 32
    srli.d  $t1, $t1, 32
    slli.d  $t2, $t2, 32
    srli.d  $t2, $t2, 32
    xor     $a2, $t1, $t2           # should be 0

    # Test 4: ST.D + LD.D — doubleword
    lu12i.w $t1, 0xAABB
    ori     $t1, $t1, 0xCCD
    st.d    $t1, $t0, 0x30
    ld.d    $t2, $t0, 0x30
    xor     $a3, $t1, $t2           # should be 0

    # Test 5: LD.BU — unsigned byte load
    ori     $t1, $zero, 0xFF
    st.b    $t1, $t0, 0x40
    ld.bu   $t2, $t0, 0x40
    xor     $a4, $t2, $t1           # should be 0

    # Test 6: STPTR.W + LDPTR.W — pre-indexed
    lu12i.w $t1, 0xCAFE
    ori     $t1, $t1, 0x999
    stptr.w $t1, $t0, 0x50
    ldptr.w $t2, $t0, 0x50
    slli.d  $t1, $t1, 32
    srli.d  $t1, $t1, 32
    slli.d  $t2, $t2, 32
    srli.d  $t2, $t2, 32
    xor     $a5, $t1, $t2           # should be 0

    # Sum = 0 means pass
    add.w   $a6, $a0, $a1
    add.w   $a6, $a6, $a2
    add.w   $a6, $a6, $a3
    add.w   $a6, $a6, $a4
    add.w   $a6, $a6, $a5

    # Store result at 0x1c000100
    lu12i.w $s0, 0x1c000
    ori     $s0, $s0, 0x100
    stptr.w $a6, $s0, 0

1:  b       1b
