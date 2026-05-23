#include "qemu/osdep.h"
#include "gdbstub/gdbstub.h"

const GDBFeature gdb_static_features[] = {
    {
        "loongarch-base32.xml",
        "<?xml version=\"1.0\"?>\n"
        "<!-- Copyright (C) 2022 Free Software Foundation, Inc.\n"
        "\n"
        "     Copying and distribution of this file, with or without modification,\n"
        "     are permitted in any medium without royalty provided the copyright\n"
        "     notice and this notice are preserved.  -->\n"
        "\n"
        "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n"
        "<feature name=\"org.gnu.gdb.loongarch.base\">\n"
        "  <reg name=\"r0\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r1\" bitsize=\"32\" type=\"code_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r2\" bitsize=\"32\" type=\"data_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r3\" bitsize=\"32\" type=\"data_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r4\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r5\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r6\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r7\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r8\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r9\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r10\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r11\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r12\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r13\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r14\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r15\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r16\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r17\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r18\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r19\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r20\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r21\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r22\" bitsize=\"32\" type=\"data_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r23\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r24\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r25\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r26\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r27\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r28\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r29\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r30\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"r31\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"orig_a0\" bitsize=\"32\" type=\"uint32\" group=\"general\"/>\n"
        "  <reg name=\"pc\" bitsize=\"32\" type=\"code_ptr\" group=\"general\"/>\n"
        "  <reg name=\"badv\" bitsize=\"32\" type=\"code_ptr\" group=\"general\"/>\n"
        "</feature>\n",
        "org.gnu.gdb.loongarch.base",
        (const char * const []) {
            [0] =
                "r0",
            [1] =
                "r1",
            [2] =
                "r2",
            [3] =
                "r3",
            [4] =
                "r4",
            [5] =
                "r5",
            [6] =
                "r6",
            [7] =
                "r7",
            [8] =
                "r8",
            [9] =
                "r9",
            [10] =
                "r10",
            [11] =
                "r11",
            [12] =
                "r12",
            [13] =
                "r13",
            [14] =
                "r14",
            [15] =
                "r15",
            [16] =
                "r16",
            [17] =
                "r17",
            [18] =
                "r18",
            [19] =
                "r19",
            [20] =
                "r20",
            [21] =
                "r21",
            [22] =
                "r22",
            [23] =
                "r23",
            [24] =
                "r24",
            [25] =
                "r25",
            [26] =
                "r26",
            [27] =
                "r27",
            [28] =
                "r28",
            [29] =
                "r29",
            [30] =
                "r30",
            [31] =
                "r31",
            [32] =
                "orig_a0",
            [33] =
                "pc",
            [34] =
                "badv",
        },
        35,
    },
    {
        "loongarch-base64.xml",
        "<?xml version=\"1.0\"?>\n"
        "<!-- Copyright (C) 2022 Free Software Foundation, Inc.\n"
        "\n"
        "     Copying and distribution of this file, with or without modification,\n"
        "     are permitted in any medium without royalty provided the copyright\n"
        "     notice and this notice are preserved.  -->\n"
        "\n"
        "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n"
        "<feature name=\"org.gnu.gdb.loongarch.base\">\n"
        "  <reg name=\"r0\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r1\" bitsize=\"64\" type=\"code_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r2\" bitsize=\"64\" type=\"data_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r3\" bitsize=\"64\" type=\"data_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r4\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r5\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r6\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r7\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r8\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r9\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r10\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r11\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r12\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r13\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r14\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r15\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r16\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r17\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r18\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r19\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r20\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r21\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r22\" bitsize=\"64\" type=\"data_ptr\" group=\"general\"/>\n"
        "  <reg name=\"r23\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r24\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r25\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r26\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r27\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r28\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r29\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r30\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"r31\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"orig_a0\" bitsize=\"64\" type=\"uint64\" group=\"general\"/>\n"
        "  <reg name=\"pc\" bitsize=\"64\" type=\"code_ptr\" group=\"general\"/>\n"
        "  <reg name=\"badv\" bitsize=\"64\" type=\"code_ptr\" group=\"general\"/>\n"
        "</feature>\n",
        "org.gnu.gdb.loongarch.base",
        (const char * const []) {
            [0] =
                "r0",
            [1] =
                "r1",
            [2] =
                "r2",
            [3] =
                "r3",
            [4] =
                "r4",
            [5] =
                "r5",
            [6] =
                "r6",
            [7] =
                "r7",
            [8] =
                "r8",
            [9] =
                "r9",
            [10] =
                "r10",
            [11] =
                "r11",
            [12] =
                "r12",
            [13] =
                "r13",
            [14] =
                "r14",
            [15] =
                "r15",
            [16] =
                "r16",
            [17] =
                "r17",
            [18] =
                "r18",
            [19] =
                "r19",
            [20] =
                "r20",
            [21] =
                "r21",
            [22] =
                "r22",
            [23] =
                "r23",
            [24] =
                "r24",
            [25] =
                "r25",
            [26] =
                "r26",
            [27] =
                "r27",
            [28] =
                "r28",
            [29] =
                "r29",
            [30] =
                "r30",
            [31] =
                "r31",
            [32] =
                "orig_a0",
            [33] =
                "pc",
            [34] =
                "badv",
        },
        35,
    },
    {
        "loongarch-fpu.xml",
        "<?xml version=\"1.0\"?>\n"
        "<!-- Copyright (C) 2021 Free Software Foundation, Inc.\n"
        "\n"
        "     Copying and distribution of this file, with or without modification,\n"
        "     are permitted in any medium without royalty provided the copyright\n"
        "     notice and this notice are preserved.  -->\n"
        "\n"
        "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n"
        "<feature name=\"org.gnu.gdb.loongarch.fpu\">\n"
        "\n"
        "  <union id=\"fputype\">\n"
        "    <field name=\"f\" type=\"ieee_single\"/>\n"
        "    <field name=\"d\" type=\"ieee_double\"/>\n"
        "  </union>\n"
        "\n"
        "  <reg name=\"f0\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f1\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f2\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f3\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f4\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f5\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f6\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f7\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f8\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f9\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f10\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f11\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f12\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f13\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f14\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f15\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f16\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f17\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f18\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f19\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f20\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f21\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f22\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f23\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f24\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f25\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f26\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f27\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f28\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f29\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f30\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"f31\" bitsize=\"64\" type=\"fputype\" group=\"float\"/>\n"
        "  <reg name=\"fcc0\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcc1\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcc2\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcc3\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcc4\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcc5\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcc6\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcc7\" bitsize=\"8\" type=\"uint8\" group=\"float\"/>\n"
        "  <reg name=\"fcsr\" bitsize=\"32\" type=\"uint32\" group=\"float\"/>\n"
        "</feature>\n",
        "org.gnu.gdb.loongarch.fpu",
        (const char * const []) {
            [0] =
                "f0",
            [1] =
                "f1",
            [2] =
                "f2",
            [3] =
                "f3",
            [4] =
                "f4",
            [5] =
                "f5",
            [6] =
                "f6",
            [7] =
                "f7",
            [8] =
                "f8",
            [9] =
                "f9",
            [10] =
                "f10",
            [11] =
                "f11",
            [12] =
                "f12",
            [13] =
                "f13",
            [14] =
                "f14",
            [15] =
                "f15",
            [16] =
                "f16",
            [17] =
                "f17",
            [18] =
                "f18",
            [19] =
                "f19",
            [20] =
                "f20",
            [21] =
                "f21",
            [22] =
                "f22",
            [23] =
                "f23",
            [24] =
                "f24",
            [25] =
                "f25",
            [26] =
                "f26",
            [27] =
                "f27",
            [28] =
                "f28",
            [29] =
                "f29",
            [30] =
                "f30",
            [31] =
                "f31",
            [32] =
                "fcc0",
            [33] =
                "fcc1",
            [34] =
                "fcc2",
            [35] =
                "fcc3",
            [36] =
                "fcc4",
            [37] =
                "fcc5",
            [38] =
                "fcc6",
            [39] =
                "fcc7",
            [40] =
                "fcsr",
        },
        41,
    },
    {
        "loongarch-lsx.xml",
        "<?xml version=\"1.0\"?>\n"
        "<!-- Copyright (C) 2022-2024 Free Software Foundation, Inc.\n"
        "\n"
        "     Copying and distribution of this file, with or without modification,\n"
        "     are permitted in any medium without royalty provided the copyright\n"
        "     notice and this notice are preserved.  -->\n"
        "\n"
        "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n"
        "<feature name=\"org.gnu.gdb.loongarch.lsx\">\n"
        "  <vector id=\"v4f32\" type=\"ieee_single\" count=\"4\"/>\n"
        "  <vector id=\"v2f64\" type=\"ieee_double\" count=\"2\"/>\n"
        "  <vector id=\"v16i8\" type=\"int8\" count=\"16\"/>\n"
        "  <vector id=\"v8i16\" type=\"int16\" count=\"8\"/>\n"
        "  <vector id=\"v4i32\" type=\"int32\" count=\"4\"/>\n"
        "  <vector id=\"v2i64\" type=\"int64\" count=\"2\"/>\n"
        "\n"
        "  <union id=\"lsxv\">\n"
        "    <field name=\"v4_float\" type=\"v4f32\"/>\n"
        "    <field name=\"v2_double\" type=\"v2f64\"/>\n"
        "    <field name=\"v16_int8\" type=\"v16i8\"/>\n"
        "    <field name=\"v8_int16\" type=\"v8i16\"/>\n"
        "    <field name=\"v4_int32\" type=\"v4i32\"/>\n"
        "    <field name=\"v2_int64\" type=\"v2i64\"/>\n"
        "    <field name=\"uint128\" type=\"uint128\"/>\n"
        "  </union>\n"
        "\n"
        "  <reg name=\"vr0\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr1\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr2\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr3\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr4\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr5\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr6\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr7\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr8\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr9\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr10\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr11\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr12\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr13\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr14\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr15\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr16\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr17\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr18\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr19\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr20\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr21\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr22\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr23\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr26\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr25\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr26\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr27\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr28\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr29\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr30\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "  <reg name=\"vr31\" bitsize=\"128\" type=\"lsxv\" group=\"lsx\"/>\n"
        "</feature>\n",
        "org.gnu.gdb.loongarch.lsx",
        (const char * const []) {
            [0] =
                "vr0",
            [1] =
                "vr1",
            [2] =
                "vr2",
            [3] =
                "vr3",
            [4] =
                "vr4",
            [5] =
                "vr5",
            [6] =
                "vr6",
            [7] =
                "vr7",
            [8] =
                "vr8",
            [9] =
                "vr9",
            [10] =
                "vr10",
            [11] =
                "vr11",
            [12] =
                "vr12",
            [13] =
                "vr13",
            [14] =
                "vr14",
            [15] =
                "vr15",
            [16] =
                "vr16",
            [17] =
                "vr17",
            [18] =
                "vr18",
            [19] =
                "vr19",
            [20] =
                "vr20",
            [21] =
                "vr21",
            [22] =
                "vr22",
            [23] =
                "vr23",
            [24] =
                "vr26",
            [25] =
                "vr25",
            [26] =
                "vr26",
            [27] =
                "vr27",
            [28] =
                "vr28",
            [29] =
                "vr29",
            [30] =
                "vr30",
            [31] =
                "vr31",
        },
        32,
    },
    {
        "loongarch-lasx.xml",
        "<?xml version=\"1.0\"?>\n"
        "<!-- Copyright (C) 2022-2024 Free Software Foundation, Inc.\n"
        "\n"
        "     Copying and distribution of this file, with or without modification,\n"
        "     are permitted in any medium without royalty provided the copyright\n"
        "     notice and this notice are preserved.  -->\n"
        "\n"
        "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n"
        "<feature name=\"org.gnu.gdb.loongarch.lasx\">\n"
        "  <vector id=\"v8f32\" type=\"ieee_single\" count=\"8\"/>\n"
        "  <vector id=\"v4f64\" type=\"ieee_double\" count=\"4\"/>\n"
        "  <vector id=\"v32i8\" type=\"int8\" count=\"32\"/>\n"
        "  <vector id=\"v16i16\" type=\"int16\" count=\"16\"/>\n"
        "  <vector id=\"v8i32\" type=\"int32\" count=\"8\"/>\n"
        "  <vector id=\"v4i64\" type=\"int64\" count=\"4\"/>\n"
        "  <vector id=\"v2ui128\" type=\"uint128\" count=\"2\"/>\n"
        "\n"
        "  <union id=\"lasxv\">\n"
        "    <field name=\"v8_float\" type=\"v8f32\"/>\n"
        "    <field name=\"v4_double\" type=\"v4f64\"/>\n"
        "    <field name=\"v32_int8\" type=\"v32i8\"/>\n"
        "    <field name=\"v16_int16\" type=\"v16i16\"/>\n"
        "    <field name=\"v8_int32\" type=\"v8i32\"/>\n"
        "    <field name=\"v4_int64\" type=\"v4i64\"/>\n"
        "    <field name=\"v2_uint128\" type=\"v2ui128\"/>\n"
        "  </union>\n"
        "\n"
        "  <reg name=\"xr0\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr1\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr2\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr3\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr4\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr5\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr6\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr7\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr8\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr9\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr10\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr11\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr12\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr13\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr14\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr15\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr16\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr17\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr18\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr19\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr20\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr21\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr22\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr23\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr24\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr25\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr26\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr27\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr28\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr29\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr30\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "  <reg name=\"xr31\" bitsize=\"256\" type=\"lasxv\" group=\"lasx\"/>\n"
        "</feature>\n",
        "org.gnu.gdb.loongarch.lasx",
        (const char * const []) {
            [0] =
                "xr0",
            [1] =
                "xr1",
            [2] =
                "xr2",
            [3] =
                "xr3",
            [4] =
                "xr4",
            [5] =
                "xr5",
            [6] =
                "xr6",
            [7] =
                "xr7",
            [8] =
                "xr8",
            [9] =
                "xr9",
            [10] =
                "xr10",
            [11] =
                "xr11",
            [12] =
                "xr12",
            [13] =
                "xr13",
            [14] =
                "xr14",
            [15] =
                "xr15",
            [16] =
                "xr16",
            [17] =
                "xr17",
            [18] =
                "xr18",
            [19] =
                "xr19",
            [20] =
                "xr20",
            [21] =
                "xr21",
            [22] =
                "xr22",
            [23] =
                "xr23",
            [24] =
                "xr24",
            [25] =
                "xr25",
            [26] =
                "xr26",
            [27] =
                "xr27",
            [28] =
                "xr28",
            [29] =
                "xr29",
            [30] =
                "xr30",
            [31] =
                "xr31",
        },
        32,
    },
    { NULL }
};
