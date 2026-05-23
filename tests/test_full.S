# Full difftest: integer + CSR dump + memory output
# CSR dump at 0x1c000200 (22 x uint64_t = 176 bytes)

.globl _start
_start:
    # ── Integer ALU loop ──
    ori     $t0, $zero, 0
    ori     $t1, $zero, 0x64        # 100 iterations
    ori     $t2, $zero, 0

1:  addi.w  $t3, $t0, 1
    add.w   $t4, $t2, $t3
    sub.w   $t5, $t4, $t0
    and     $t6, $t5, $t3
    or      $t7, $t6, $t0
    xor     $t8, $t7, $t5
    slli.w  $t2, $t8, 1
    srli.w  $t2, $t2, 1
    add.w   $t2, $t2, $t6
    addi.w  $t0, $t0, 1
    blt     $t0, $t1, 1b

    # ── CSR dump to 0x1c000200 ──
    lu12i.w $s0, 0x1c000
    ori     $s0, $s0, 0x200

    csrrd   $t0, 0x0                # CRMD
    st.d    $t0, $s0, 0
    csrrd   $t0, 0x1                # PRMD
    st.d    $t0, $s0, 8
    csrrd   $t0, 0x2                # EUEN
    st.d    $t0, $s0, 16
    csrrd   $t0, 0x4                # ECFG
    st.d    $t0, $s0, 24
    csrrd   $t0, 0x5                # ESTAT
    st.d    $t0, $s0, 32
    csrrd   $t0, 0x6                # ERA
    st.d    $t0, $s0, 40
    csrrd   $t0, 0x7                # BADV
    st.d    $t0, $s0, 48
    csrrd   $t0, 0xc                # EENTRY
    st.d    $t0, $s0, 56
    csrrd   $t0, 0x10               # TLBIDX
    st.d    $t0, $s0, 64
    csrrd   $t0, 0x11               # TLBEHI
    st.d    $t0, $s0, 72
    csrrd   $t0, 0x12               # TLBELO0
    st.d    $t0, $s0, 80
    csrrd   $t0, 0x13               # TLBELO1
    st.d    $t0, $s0, 88
    csrrd   $t0, 0x18               # ASID
    st.d    $t0, $s0, 96
    csrrd   $t0, 0x19               # PGDL
    st.d    $t0, $s0, 104
    csrrd   $t0, 0x1a               # PGDH
    st.d    $t0, $s0, 112
    csrrd   $t0, 0x1e               # STLBPS
    st.d    $t0, $s0, 120
    csrrd   $t0, 0x41               # TCFG
    st.d    $t0, $s0, 128
    csrrd   $t0, 0x42               # TVAL
    st.d    $t0, $s0, 136
    csrrd   $t0, 0x44               # TICLR
    st.d    $t0, $s0, 144
    csrrd   $t0, 0x60               # LLBCTL
    st.d    $t0, $s0, 152
    csrrd   $t0, 0x180              # DMW0
    st.d    $t0, $s0, 160
    csrrd   $t0, 0x181              # DMW1
    st.d    $t0, $s0, 168

    # ── Test output at 0x1c000100 ──
    lu12i.w $s1, 0x1c000
    ori     $s1, $s1, 0x100
    stptr.w $t2, $s1, 0

done:
    addi.w  $zero, $zero, 0
    b       done
