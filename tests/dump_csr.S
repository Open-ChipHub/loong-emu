# Minimal CSR dump trampoline — assembled to extract raw machine code.
# Input:  $t0 = dump base address (passed from caller)
# Clobbers: $t1
# Output: 30 CSR values written to [$t0 .. $t0+239]
# End:    b 0 (infinite loop = done marker)

.globl _start
_start:
    csrrd   $t1, 0x0
    st.d    $t1, $t0, 0
    csrrd   $t1, 0x1
    st.d    $t1, $t0, 8
    csrrd   $t1, 0x2
    st.d    $t1, $t0, 16
    csrrd   $t1, 0x4
    st.d    $t1, $t0, 24
    csrrd   $t1, 0x5
    st.d    $t1, $t0, 32
    csrrd   $t1, 0x6
    st.d    $t1, $t0, 40
    csrrd   $t1, 0x7
    st.d    $t1, $t0, 48
    csrrd   $t1, 0xc
    st.d    $t1, $t0, 56
    csrrd   $t1, 0x10
    st.d    $t1, $t0, 64
    csrrd   $t1, 0x11
    st.d    $t1, $t0, 72
    csrrd   $t1, 0x12
    st.d    $t1, $t0, 80
    csrrd   $t1, 0x13
    st.d    $t1, $t0, 88
    csrrd   $t1, 0x18
    st.d    $t1, $t0, 96
    csrrd   $t1, 0x19
    st.d    $t1, $t0, 104
    csrrd   $t1, 0x1a
    st.d    $t1, $t0, 112
    csrrd   $t1, 0x1e
    st.d    $t1, $t0, 120
    csrrd   $t1, 0x41
    st.d    $t1, $t0, 128
    csrrd   $t1, 0x42
    st.d    $t1, $t0, 136
    csrrd   $t1, 0x44
    st.d    $t1, $t0, 144
    csrrd   $t1, 0x60
    st.d    $t1, $t0, 152
    csrrd   $t1, 0x180
    st.d    $t1, $t0, 160
    csrrd   $t1, 0x181
    st.d    $t1, $t0, 168
    csrrd   $t1, 0x88
    st.d    $t1, $t0, 176
    csrrd   $t1, 0x89
    st.d    $t1, $t0, 184
    csrrd   $t1, 0x8a
    st.d    $t1, $t0, 192
    csrrd   $t1, 0x8b
    st.d    $t1, $t0, 200
    csrrd   $t1, 0x8c
    st.d    $t1, $t0, 208
    csrrd   $t1, 0x8d
    st.d    $t1, $t0, 216
    csrrd   $t1, 0x8e
    st.d    $t1, $t0, 224
    csrrd   $t1, 0x8f
    st.d    $t1, $t0, 232
1:  b       1b
