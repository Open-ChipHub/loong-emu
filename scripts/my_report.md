# Emu Instruction Execution Report

**Total instructions executed**: 100000  
**Distinct executed**: 214 / 1690 static ISA  

## Exceptions & Interrupts

| Type | Count |
|------|-------|
| IRQ (interrupts raised) | 0 |
| TLB refill events | 4980 |
| Syscall count | 0 |
| Exception entries (ecount) | 4980 |

*No exceptions triggered during execution.*

## By Category

| Category | Count | % |
|----------|-------|----|
| Arithmetic/Shift (Ch3.1) | 84919 | 84.92% |
| Float (Ch4) | 15081 | 15.08% |

## Executed Instructions

| # | Instruction | Category | Count | % |
|---|------------|----------|-------|----|
| 1 | `0014bdad` | Arithmetic/Shift (Ch3.1) | **14** | **0.01%** |
| 2 | `0014bdce` | Arithmetic/Shift (Ch3.1) | **14** | **0.01%** |
| 3 | `0014c18c` | Arithmetic/Shift (Ch3.1) | **2** | **0.00%** |
| 4 | `0014ea3c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 5 | `0015000c` | Arithmetic/Shift (Ch3.1) | **4980** | **4.98%** |
| 6 | `00153dce` | Arithmetic/Shift (Ch3.1) | **14** | **0.01%** |
| 7 | `0041318c` | Arithmetic/Shift (Ch3.1) | **24925** | **24.93%** |
| 8 | `00413210` | Arithmetic/Shift (Ch3.1) | **10** | **0.01%** |
| 9 | `004c823d` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 10 | `02603f29` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 11 | `02bffc0f` | Arithmetic/Shift (Ch3.1) | **14** | **0.01%** |
| 12 | `02c0000c` | Arithmetic/Shift (Ch3.1) | **4985** | **4.99%** |
| 13 | `02c00010` | Arithmetic/Shift (Ch3.1) | **2** | **0.00%** |
| 14 | `02c0018c` | Arithmetic/Shift (Ch3.1) | **3** | **0.00%** |
| 15 | `02c001ad` | Arithmetic/Shift (Ch3.1) | **2** | **0.00%** |
| 16 | `02c001ce` | Arithmetic/Shift (Ch3.1) | **14** | **0.01%** |
| 17 | `02c021ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 18 | `02c0418c` | Arithmetic/Shift (Ch3.1) | **4981** | **4.98%** |
| 19 | `02c041ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 20 | `02c061ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 21 | `02c081ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 22 | `02c0a1ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 23 | `02c0c1ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 24 | `02c0e1ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 25 | `02c101ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 26 | `02c121ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 27 | `02c1418c` | Arithmetic/Shift (Ch3.1) | **4980** | **4.98%** |
| 28 | `02c141ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 29 | `02c161ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 30 | `02c4018c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 31 | `02d001ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 32 | `02d89bdb` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 33 | `02e1c18c` | Arithmetic/Shift (Ch3.1) | **4980** | **4.98%** |
| 34 | `02ed518c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 35 | `02f0e063` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 36 | `02f102d6` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 37 | `0300958c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 38 | `03016d4a` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 39 | `030667bd` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 40 | `03068908` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 41 | `031108a5` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 42 | `03145129` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 43 | `03161eb5` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 44 | `0318f021` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 45 | `031aa35a` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 46 | `0321c631` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 47 | `03236294` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 48 | `0325d4e7` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 49 | `0326e042` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 50 | `032847de` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 51 | `0329916b` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 52 | `032b52d6` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 53 | `032bedce` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 54 | `032c8884` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 55 | `032e53ff` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 56 | `032e7610` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 57 | `0330b2f7` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 58 | `0330e252` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 59 | `03311863` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 60 | `033350c6` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 61 | `033865ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 62 | `033aa77b` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 63 | `033b079c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 64 | `033b4b39` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 65 | `033c1a73` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 66 | `033cf318` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 67 | `033e05ef` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 68 | `0380318c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 69 | `0381358c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 70 | `0382ab39` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 71 | `0382c18c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 72 | `03864c0f` | Arithmetic/Shift (Ch3.1) | **14** | **0.01%** |
| 73 | `03877ed6` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 74 | `0388798c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 75 | `0389798c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 76 | `03899d8c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 77 | `038a1842` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 78 | `038a5318` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 79 | `038a7b7b` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 80 | `038a8908` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 81 | `038cc3ff` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 82 | `038ce529` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 83 | `038ec821` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 84 | `039129ad` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 85 | `0393d610` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 86 | `0394b18c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 87 | `0394b4a5` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 88 | `03969884` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 89 | `039a8273` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 90 | `039d4fbd` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 91 | `039e1e31` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 92 | `03a2a6f7` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 93 | `03a3f652` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 94 | `03a3f8c6` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 95 | `03a4c4e7` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 96 | `03a5d75a` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 97 | `03a61463` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 98 | `03a73dce` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 99 | `03a91f9c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 100 | `03b0e6b5` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 101 | `03b45294` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 102 | `03b61fde` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 103 | `03bae56b` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 104 | `03bb714a` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 105 | `03bb7def` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 106 | `03bffe10` | Arithmetic/Shift (Ch3.1) | **8** | **0.01%** |
| 107 | `0400000c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 108 | `0400002c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 109 | `0400042c` | Arithmetic/Shift (Ch3.1) | **4980** | **4.98%** |
| 110 | `0400102c` | Arithmetic/Shift (Ch3.1) | **4981** | **4.98%** |
| 111 | `0400182c` | Arithmetic/Shift (Ch3.1) | **4980** | **4.98%** |
| 112 | `0400302c` | Arithmetic/Shift (Ch3.1) | **4980** | **4.98%** |
| 113 | `0400642c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 114 | `0400682c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 115 | `0400702c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 116 | `0400742c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 117 | `0400782c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 118 | `0402202c` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 119 | `06483800` | Arithmetic/Shift (Ch3.1) | **4980** | **4.98%** |
| 120 | `06498000` | Arithmetic/Shift (Ch3.1) | **2** | **0.00%** |
| 121 | `29c001ae` | Arithmetic/Shift (Ch3.1) | **14** | **0.01%** |
| 122 | `4c000180` | Arithmetic/Shift (Ch3.1) | **4981** | **4.98%** |
| 123 | `4c000181` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 124 | `50000400` | Arithmetic/Shift (Ch3.1) | **4981** | **4.98%** |
| 125 | `50055000` | Arithmetic/Shift (Ch3.1) | **1** | **0.00%** |
| 126 | `14011404` | Float (Ch4) | **1** | **0.00%** |
| 127 | `140b00ba` | Float (Ch4) | **1** | **0.00%** |
| 128 | `140d072b` | Float (Ch4) | **1** | **0.00%** |
| 129 | `140ede71` | Float (Ch4) | **1** | **0.00%** |
| 130 | `140fa96a` | Float (Ch4) | **1** | **0.00%** |
| 131 | `145f2f1e` | Float (Ch4) | **1** | **0.00%** |
| 132 | `146ee60f` | Float (Ch4) | **1** | **0.00%** |
| 133 | `14700417` | Float (Ch4) | **1** | **0.00%** |
| 134 | `14718192` | Float (Ch4) | **1** | **0.00%** |
| 135 | `14752047` | Float (Ch4) | **1** | **0.00%** |
| 136 | `14763436` | Float (Ch4) | **1** | **0.00%** |
| 137 | `147756d5` | Float (Ch4) | **1** | **0.00%** |
| 138 | `14945e59` | Float (Ch4) | **1** | **0.00%** |
| 139 | `14c32d2e` | Float (Ch4) | **1** | **0.00%** |
| 140 | `14d81fb0` | Float (Ch4) | **1** | **0.00%** |
| 141 | `14dd93a2` | Float (Ch4) | **1** | **0.00%** |
| 142 | `14e032d3` | Float (Ch4) | **1** | **0.00%** |
| 143 | `14e409c5` | Float (Ch4) | **1** | **0.00%** |
| 144 | `14eb5138` | Float (Ch4) | **1** | **0.00%** |
| 145 | `14fddb14` | Float (Ch4) | **1** | **0.00%** |
| 146 | `1515340c` | Float (Ch4) | **1** | **0.00%** |
| 147 | `151ce79c` | Float (Ch4) | **1** | **0.00%** |
| 148 | `1548dadd` | Float (Ch4) | **1** | **0.00%** |
| 149 | `15716da6` | Float (Ch4) | **1** | **0.00%** |
| 150 | `15864983` | Float (Ch4) | **1** | **0.00%** |
| 151 | `15abe961` | Float (Ch4) | **1** | **0.00%** |
| 152 | `15acd71b` | Float (Ch4) | **1** | **0.00%** |
| 153 | `15cd87e9` | Float (Ch4) | **1** | **0.00%** |
| 154 | `15e0d82d` | Float (Ch4) | **1** | **0.00%** |
| 155 | `15e3fa88` | Float (Ch4) | **1** | **0.00%** |
| 156 | `15fc2ebf` | Float (Ch4) | **1** | **0.00%** |
| 157 | `15ffffef` | Float (Ch4) | **14** | **0.01%** |
| 158 | `160cfdbf` | Float (Ch4) | **1** | **0.00%** |
| 159 | `16151e1b` | Float (Ch4) | **1** | **0.00%** |
| 160 | `161fffef` | Float (Ch4) | **28** | **0.03%** |
| 161 | `1621cec2` | Float (Ch4) | **1** | **0.00%** |
| 162 | `162f3c10` | Float (Ch4) | **1** | **0.00%** |
| 163 | `163d11f2` | Float (Ch4) | **1** | **0.00%** |
| 164 | `16434b04` | Float (Ch4) | **1** | **0.00%** |
| 165 | `16571791` | Float (Ch4) | **1** | **0.00%** |
| 166 | `1661884f` | Float (Ch4) | **1** | **0.00%** |
| 167 | `16623269` | Float (Ch4) | **1** | **0.00%** |
| 168 | `168e2b23` | Float (Ch4) | **1** | **0.00%** |
| 169 | `168fc6a6` | Float (Ch4) | **1** | **0.00%** |
| 170 | `1691969c` | Float (Ch4) | **1** | **0.00%** |
| 171 | `16b0d870` | Float (Ch4) | **1** | **0.00%** |
| 172 | `16ba9827` | Float (Ch4) | **1** | **0.00%** |
| 173 | `16be47b6` | Float (Ch4) | **1** | **0.00%** |
| 174 | `16c95ef8` | Float (Ch4) | **1** | **0.00%** |
| 175 | `16dcdc6a` | Float (Ch4) | **1** | **0.00%** |
| 176 | `17069154` | Float (Ch4) | **1** | **0.00%** |
| 177 | `17115b2d` | Float (Ch4) | **1** | **0.00%** |
| 178 | `1714fbab` | Float (Ch4) | **1** | **0.00%** |
| 179 | `1733c7fd` | Float (Ch4) | **1** | **0.00%** |
| 180 | `175051a1` | Float (Ch4) | **1** | **0.00%** |
| 181 | `1758975a` | Float (Ch4) | **1** | **0.00%** |
| 182 | `176827ec` | Float (Ch4) | **1** | **0.00%** |
| 183 | `1771e3e8` | Float (Ch4) | **1** | **0.00%** |
| 184 | `17a7fdf3` | Float (Ch4) | **1** | **0.00%** |
| 185 | `17a8b35e` | Float (Ch4) | **1** | **0.00%** |
| 186 | `17acff2e` | Float (Ch4) | **1** | **0.00%** |
| 187 | `17af1357` | Float (Ch4) | **1** | **0.00%** |
| 188 | `17c7a865` | Float (Ch4) | **1** | **0.00%** |
| 189 | `17f13b39` | Float (Ch4) | **1** | **0.00%** |
| 190 | `17fa5015` | Float (Ch4) | **1** | **0.00%** |
| 191 | `1a00000c` | Float (Ch4) | **9961** | **9.96%** |
| 192 | `1a00002c` | Float (Ch4) | **4981** | **4.98%** |
| 193 | `1a00758c` | Float (Ch4) | **1** | **0.00%** |
| 194 | `1a0080c3` | Float (Ch4) | **1** | **0.00%** |
| 195 | `1a008216` | Float (Ch4) | **1** | **0.00%** |
| 196 | `1a00842c` | Float (Ch4) | **2** | **0.00%** |
| 197 | `1a00842d` | Float (Ch4) | **1** | **0.00%** |
| 198 | `1a00844d` | Float (Ch4) | **1** | **0.00%** |
| 199 | `1a00844e` | Float (Ch4) | **1** | **0.00%** |
| 200 | `1a00846d` | Float (Ch4) | **12** | **0.01%** |
| 201 | `1a00846e` | Float (Ch4) | **1** | **0.00%** |
| 202 | `1a00848e` | Float (Ch4) | **1** | **0.00%** |
| 203 | `1a0084ae` | Float (Ch4) | **1** | **0.00%** |
| 204 | `1a0084ce` | Float (Ch4) | **1** | **0.00%** |
| 205 | `1a0084ee` | Float (Ch4) | **1** | **0.00%** |
| 206 | `1a00850e` | Float (Ch4) | **1** | **0.00%** |
| 207 | `1a00852e` | Float (Ch4) | **1** | **0.00%** |
| 208 | `1a00854e` | Float (Ch4) | **1** | **0.00%** |
| 209 | `1a00856e` | Float (Ch4) | **1** | **0.00%** |
| 210 | `1a00858e` | Float (Ch4) | **1** | **0.00%** |
| 211 | `1a0085ae` | Float (Ch4) | **1** | **0.00%** |
| 212 | `1a0085ce` | Float (Ch4) | **1** | **0.00%** |
| 213 | `1a0085ee` | Float (Ch4) | **1** | **0.00%** |
| 214 | `1e861706` | Float (Ch4) | **1** | **0.00%** |

*214 distinct instructions executed.*

## Complete ISA Reference

> All 1690 static ISA instructions from `trans_la.c.inc`.  
> **Bold** = executed. Plain = not executed in this run.

| # | Instruction | Category | Count |
|---|------------|----------|-------|
| 1 | `add.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 2 | `add.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 3 | `addi.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 4 | `addi.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 5 | `addu16i.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 6 | `alsl.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 7 | `alsl.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 8 | `alsl.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 9 | `amadd.d` | Atomic (Ch3.5) | 0 |
| 10 | `amadd.db.d` | Atomic (Ch3.5) | 0 |
| 11 | `amadd.db.w` | Atomic (Ch3.5) | 0 |
| 12 | `amadd.w` | Atomic (Ch3.5) | 0 |
| 13 | `amand.d` | Atomic (Ch3.5) | 0 |
| 14 | `amand.db.d` | Atomic (Ch3.5) | 0 |
| 15 | `amand.db.w` | Atomic (Ch3.5) | 0 |
| 16 | `amand.w` | Atomic (Ch3.5) | 0 |
| 17 | `ammax.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 18 | `ammax.db.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 19 | `ammax.db.du` | Arithmetic/Shift (Ch3.1) | 0 |
| 20 | `ammax.db.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 21 | `ammax.db.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 22 | `ammax.du` | Arithmetic/Shift (Ch3.1) | 0 |
| 23 | `ammax.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 24 | `ammax.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 25 | `ammin.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 26 | `ammin.db.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 27 | `ammin.db.du` | Arithmetic/Shift (Ch3.1) | 0 |
| 28 | `ammin.db.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 29 | `ammin.db.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 30 | `ammin.du` | Arithmetic/Shift (Ch3.1) | 0 |
| 31 | `ammin.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 32 | `ammin.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 33 | `amor.d` | Atomic (Ch3.5) | 0 |
| 34 | `amor.db.d` | Atomic (Ch3.5) | 0 |
| 35 | `amor.db.w` | Atomic (Ch3.5) | 0 |
| 36 | `amor.w` | Atomic (Ch3.5) | 0 |
| 37 | `amswap.d` | Atomic (Ch3.5) | 0 |
| 38 | `amswap.db.d` | Atomic (Ch3.5) | 0 |
| 39 | `amswap.db.w` | Atomic (Ch3.5) | 0 |
| 40 | `amswap.w` | Atomic (Ch3.5) | 0 |
| 41 | `amxor.d` | Atomic (Ch3.5) | 0 |
| 42 | `amxor.db.d` | Atomic (Ch3.5) | 0 |
| 43 | `amxor.db.w` | Atomic (Ch3.5) | 0 |
| 44 | `amxor.w` | Atomic (Ch3.5) | 0 |
| 45 | `and` | Arithmetic/Shift (Ch3.1) | 0 |
| 46 | `andi` | Arithmetic/Shift (Ch3.1) | 0 |
| 47 | `andn` | Arithmetic/Shift (Ch3.1) | 0 |
| 48 | `asrtgt.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 49 | `asrtle.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 50 | `b` | Branch/Jump (Ch3.3) | 0 |
| 51 | `bceqz` | Branch/Jump (Ch3.3) | 0 |
| 52 | `bcnez` | Branch/Jump (Ch3.3) | 0 |
| 53 | `beq` | Branch/Jump (Ch3.3) | 0 |
| 54 | `beqz` | Branch/Jump (Ch3.3) | 0 |
| 55 | `bge` | Branch/Jump (Ch3.3) | 0 |
| 56 | `bgeu` | Branch/Jump (Ch3.3) | 0 |
| 57 | `bitrev.4b` | Arithmetic/Shift (Ch3.1) | 0 |
| 58 | `bitrev.8b` | Arithmetic/Shift (Ch3.1) | 0 |
| 59 | `bitrev.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 60 | `bitrev.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 61 | `bl` | Branch/Jump (Ch3.3) | 0 |
| 62 | `blt` | Branch/Jump (Ch3.3) | 0 |
| 63 | `bltu` | Branch/Jump (Ch3.3) | 0 |
| 64 | `bne` | Branch/Jump (Ch3.3) | 0 |
| 65 | `bnez` | Branch/Jump (Ch3.3) | 0 |
| 66 | `break` | Arithmetic/Shift (Ch3.1) | 0 |
| 67 | `bstrins.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 68 | `bstrins.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 69 | `bstrpick.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 70 | `bstrpick.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 71 | `bytepick.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 72 | `bytepick.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 73 | `cacop` | Privileged (Ch6) | 0 |
| 74 | `clo.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 75 | `clo.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 76 | `clz.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 77 | `clz.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 78 | `cpucfg` | Privileged (Ch6) | 0 |
| 79 | `crc.w.b.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 80 | `crc.w.d.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 81 | `crc.w.h.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 82 | `crc.w.w.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 83 | `crcc.w.b.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 84 | `crcc.w.d.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 85 | `crcc.w.h.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 86 | `crcc.w.w.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 87 | `csrrd` | Privileged (Ch6) | 0 |
| 88 | `csrwr` | Privileged (Ch6) | 0 |
| 89 | `csrxchg` | Privileged (Ch6) | 0 |
| 90 | `cto.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 91 | `cto.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 92 | `ctz.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 93 | `ctz.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 94 | `dbar` | Barrier (Ch3.6) | 0 |
| 95 | `dbcl` | Arithmetic/Shift (Ch3.1) | 0 |
| 96 | `div.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 97 | `div.du` | Arithmetic/Shift (Ch3.1) | 0 |
| 98 | `div.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 99 | `div.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 100 | `ertn` | Privileged (Ch6) | 0 |
| 101 | `ext.w.b` | Arithmetic/Shift (Ch3.1) | 0 |
| 102 | `ext.w.h` | Arithmetic/Shift (Ch3.1) | 0 |
| 103 | `fabs.d` | Float (Ch4) | 0 |
| 104 | `fabs.s` | Float (Ch4) | 0 |
| 105 | `fadd.d` | Float (Ch4) | 0 |
| 106 | `fadd.s` | Float (Ch4) | 0 |
| 107 | `fclass.d` | Float (Ch4) | 0 |
| 108 | `fclass.s` | Float (Ch4) | 0 |
| 109 | `fcmp.cond.d` | Float (Ch4) | 0 |
| 110 | `fcmp.cond.s` | Float (Ch4) | 0 |
| 111 | `fcopysign.d` | Float (Ch4) | 0 |
| 112 | `fcopysign.s` | Float (Ch4) | 0 |
| 113 | `fcvt.d.s` | Float (Ch4) | 0 |
| 114 | `fcvt.s.d` | Float (Ch4) | 0 |
| 115 | `fdiv.d` | Float (Ch4) | 0 |
| 116 | `fdiv.s` | Float (Ch4) | 0 |
| 117 | `ffint.d.l` | Float (Ch4) | 0 |
| 118 | `ffint.d.w` | Float (Ch4) | 0 |
| 119 | `ffint.s.l` | Float (Ch4) | 0 |
| 120 | `ffint.s.w` | Float (Ch4) | 0 |
| 121 | `fld.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 122 | `fld.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 123 | `fldgt.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 124 | `fldgt.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 125 | `fldle.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 126 | `fldle.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 127 | `fldx.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 128 | `fldx.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 129 | `flogb.d` | Float (Ch4) | 0 |
| 130 | `flogb.s` | Float (Ch4) | 0 |
| 131 | `fmadd.d` | Float (Ch4) | 0 |
| 132 | `fmadd.s` | Float (Ch4) | 0 |
| 133 | `fmax.d` | Float (Ch4) | 0 |
| 134 | `fmax.s` | Float (Ch4) | 0 |
| 135 | `fmaxa.d` | Float (Ch4) | 0 |
| 136 | `fmaxa.s` | Float (Ch4) | 0 |
| 137 | `fmin.d` | Float (Ch4) | 0 |
| 138 | `fmin.s` | Float (Ch4) | 0 |
| 139 | `fmina.d` | Float (Ch4) | 0 |
| 140 | `fmina.s` | Float (Ch4) | 0 |
| 141 | `fmov.d` | Float (Ch4) | 0 |
| 142 | `fmov.s` | Float (Ch4) | 0 |
| 143 | `fmsub.d` | Float (Ch4) | 0 |
| 144 | `fmsub.s` | Float (Ch4) | 0 |
| 145 | `fmul.d` | Float (Ch4) | 0 |
| 146 | `fmul.s` | Float (Ch4) | 0 |
| 147 | `fneg.d` | Float (Ch4) | 0 |
| 148 | `fneg.s` | Float (Ch4) | 0 |
| 149 | `fnmadd.d` | Float (Ch4) | 0 |
| 150 | `fnmadd.s` | Float (Ch4) | 0 |
| 151 | `fnmsub.d` | Float (Ch4) | 0 |
| 152 | `fnmsub.s` | Float (Ch4) | 0 |
| 153 | `frecip.d` | Float (Ch4) | 0 |
| 154 | `frecip.s` | Float (Ch4) | 0 |
| 155 | `frint.d` | Float (Ch4) | 0 |
| 156 | `frint.s` | Float (Ch4) | 0 |
| 157 | `frsqrt.d` | Float (Ch4) | 0 |
| 158 | `frsqrt.s` | Float (Ch4) | 0 |
| 159 | `fscaleb.d` | Float (Ch4) | 0 |
| 160 | `fscaleb.s` | Float (Ch4) | 0 |
| 161 | `fsel` | Float (Ch4) | 0 |
| 162 | `fsqrt.d` | Float (Ch4) | 0 |
| 163 | `fsqrt.s` | Float (Ch4) | 0 |
| 164 | `fst.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 165 | `fst.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 166 | `fstgt.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 167 | `fstgt.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 168 | `fstle.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 169 | `fstle.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 170 | `fstx.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 171 | `fstx.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 172 | `fsub.d` | Float (Ch4) | 0 |
| 173 | `fsub.s` | Float (Ch4) | 0 |
| 174 | `ftint.l.d` | Float (Ch4) | 0 |
| 175 | `ftint.l.s` | Float (Ch4) | 0 |
| 176 | `ftint.w.d` | Float (Ch4) | 0 |
| 177 | `ftint.w.s` | Float (Ch4) | 0 |
| 178 | `ftintrm.l.d` | Float (Ch4) | 0 |
| 179 | `ftintrm.l.s` | Float (Ch4) | 0 |
| 180 | `ftintrm.w.d` | Float (Ch4) | 0 |
| 181 | `ftintrm.w.s` | Float (Ch4) | 0 |
| 182 | `ftintrne.l.d` | Float (Ch4) | 0 |
| 183 | `ftintrne.l.s` | Float (Ch4) | 0 |
| 184 | `ftintrne.w.d` | Float (Ch4) | 0 |
| 185 | `ftintrne.w.s` | Float (Ch4) | 0 |
| 186 | `ftintrp.l.d` | Float (Ch4) | 0 |
| 187 | `ftintrp.l.s` | Float (Ch4) | 0 |
| 188 | `ftintrp.w.d` | Float (Ch4) | 0 |
| 189 | `ftintrp.w.s` | Float (Ch4) | 0 |
| 190 | `ftintrz.l.d` | Float (Ch4) | 0 |
| 191 | `ftintrz.l.s` | Float (Ch4) | 0 |
| 192 | `ftintrz.w.d` | Float (Ch4) | 0 |
| 193 | `ftintrz.w.s` | Float (Ch4) | 0 |
| 194 | `ibar` | Barrier (Ch3.6) | 0 |
| 195 | `idle` | Privileged (Ch6) | 0 |
| 196 | `invtlb` | Privileged (Ch6) | 0 |
| 197 | `iocsrrd.b` | Privileged (Ch6) | 0 |
| 198 | `iocsrrd.d` | Privileged (Ch6) | 0 |
| 199 | `iocsrrd.h` | Privileged (Ch6) | 0 |
| 200 | `iocsrrd.w` | Privileged (Ch6) | 0 |
| 201 | `iocsrwr.b` | Privileged (Ch6) | 0 |
| 202 | `iocsrwr.d` | Privileged (Ch6) | 0 |
| 203 | `iocsrwr.h` | Privileged (Ch6) | 0 |
| 204 | `iocsrwr.w` | Privileged (Ch6) | 0 |
| 205 | `jirl` | Branch/Jump (Ch3.3) | 0 |
| 206 | `ld.b` | Load/Store (Ch3.4) | 0 |
| 207 | `ld.bu` | Load/Store (Ch3.4) | 0 |
| 208 | `ld.d` | Load/Store (Ch3.4) | 0 |
| 209 | `ld.h` | Load/Store (Ch3.4) | 0 |
| 210 | `ld.hu` | Load/Store (Ch3.4) | 0 |
| 211 | `ld.w` | Load/Store (Ch3.4) | 0 |
| 212 | `ld.wu` | Load/Store (Ch3.4) | 0 |
| 213 | `lddir` | Load/Store (Ch3.4) | 0 |
| 214 | `ldgt.b` | Load/Store (Ch3.4) | 0 |
| 215 | `ldgt.d` | Load/Store (Ch3.4) | 0 |
| 216 | `ldgt.h` | Load/Store (Ch3.4) | 0 |
| 217 | `ldgt.w` | Load/Store (Ch3.4) | 0 |
| 218 | `ldle.b` | Load/Store (Ch3.4) | 0 |
| 219 | `ldle.d` | Load/Store (Ch3.4) | 0 |
| 220 | `ldle.h` | Load/Store (Ch3.4) | 0 |
| 221 | `ldle.w` | Load/Store (Ch3.4) | 0 |
| 222 | `ldpte` | Load/Store (Ch3.4) | 0 |
| 223 | `ldptr.d` | Load/Store (Ch3.4) | 0 |
| 224 | `ldptr.w` | Load/Store (Ch3.4) | 0 |
| 225 | `ldx.b` | Load/Store (Ch3.4) | 0 |
| 226 | `ldx.bu` | Load/Store (Ch3.4) | 0 |
| 227 | `ldx.d` | Load/Store (Ch3.4) | 0 |
| 228 | `ldx.h` | Load/Store (Ch3.4) | 0 |
| 229 | `ldx.hu` | Load/Store (Ch3.4) | 0 |
| 230 | `ldx.w` | Load/Store (Ch3.4) | 0 |
| 231 | `ldx.wu` | Load/Store (Ch3.4) | 0 |
| 232 | `ll.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 233 | `ll.w` | Load/Store (Ch3.4) | 0 |
| 234 | `lu12i.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 235 | `lu32i.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 236 | `lu52i.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 237 | `maskeqz` | Arithmetic/Shift (Ch3.1) | 0 |
| 238 | `masknez` | Arithmetic/Shift (Ch3.1) | 0 |
| 239 | `mod.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 240 | `mod.du` | Arithmetic/Shift (Ch3.1) | 0 |
| 241 | `mod.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 242 | `mod.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 243 | `movcf2fr` | Arithmetic/Shift (Ch3.1) | 0 |
| 244 | `movcf2gr` | Arithmetic/Shift (Ch3.1) | 0 |
| 245 | `movfcsr2gr` | Arithmetic/Shift (Ch3.1) | 0 |
| 246 | `movfr2cf` | Arithmetic/Shift (Ch3.1) | 0 |
| 247 | `movfr2gr.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 248 | `movfr2gr.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 249 | `movfrh2gr.s` | Arithmetic/Shift (Ch3.1) | 0 |
| 250 | `movgr2cf` | Arithmetic/Shift (Ch3.1) | 0 |
| 251 | `movgr2fcsr` | Arithmetic/Shift (Ch3.1) | 0 |
| 252 | `movgr2fr.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 253 | `movgr2fr.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 254 | `movgr2frh.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 255 | `mul.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 256 | `mul.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 257 | `mulh.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 258 | `mulh.du` | Arithmetic/Shift (Ch3.1) | 0 |
| 259 | `mulh.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 260 | `mulh.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 261 | `mulw.d.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 262 | `mulw.d.wu` | Arithmetic/Shift (Ch3.1) | 0 |
| 263 | `nor` | Arithmetic/Shift (Ch3.1) | 0 |
| 264 | `or` | Arithmetic/Shift (Ch3.1) | 0 |
| 265 | `ori` | Arithmetic/Shift (Ch3.1) | 0 |
| 266 | `orn` | Arithmetic/Shift (Ch3.1) | 0 |
| 267 | `pcaddi` | Arithmetic/Shift (Ch3.1) | 0 |
| 268 | `pcaddu12i` | Arithmetic/Shift (Ch3.1) | 0 |
| 269 | `pcaddu18i` | Arithmetic/Shift (Ch3.1) | 0 |
| 270 | `pcalau12i` | Arithmetic/Shift (Ch3.1) | 0 |
| 271 | `preld` | Load/Store (Ch3.4) | 0 |
| 272 | `preldx` | Load/Store (Ch3.4) | 0 |
| 273 | `rdtime.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 274 | `rdtimeh.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 275 | `rdtimel.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 276 | `revb.2h` | Arithmetic/Shift (Ch3.1) | 0 |
| 277 | `revb.2w` | Arithmetic/Shift (Ch3.1) | 0 |
| 278 | `revb.4h` | Arithmetic/Shift (Ch3.1) | 0 |
| 279 | `revb.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 280 | `revh.2w` | Arithmetic/Shift (Ch3.1) | 0 |
| 281 | `revh.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 282 | `rotr.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 283 | `rotr.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 284 | `rotri.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 285 | `rotri.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 286 | `sc.d` | Load/Store (Ch3.4) | 0 |
| 287 | `sc.w` | Load/Store (Ch3.4) | 0 |
| 288 | `sll.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 289 | `sll.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 290 | `slli.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 291 | `slli.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 292 | `slt` | Arithmetic/Shift (Ch3.1) | 0 |
| 293 | `slti` | Arithmetic/Shift (Ch3.1) | 0 |
| 294 | `sltu` | Arithmetic/Shift (Ch3.1) | 0 |
| 295 | `sltui` | Arithmetic/Shift (Ch3.1) | 0 |
| 296 | `sra.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 297 | `sra.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 298 | `srai.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 299 | `srai.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 300 | `srl.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 301 | `srl.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 302 | `srli.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 303 | `srli.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 304 | `st.b` | Load/Store (Ch3.4) | 0 |
| 305 | `st.d` | Load/Store (Ch3.4) | 0 |
| 306 | `st.h` | Load/Store (Ch3.4) | 0 |
| 307 | `st.w` | Load/Store (Ch3.4) | 0 |
| 308 | `stgt.b` | Load/Store (Ch3.4) | 0 |
| 309 | `stgt.d` | Load/Store (Ch3.4) | 0 |
| 310 | `stgt.h` | Load/Store (Ch3.4) | 0 |
| 311 | `stgt.w` | Load/Store (Ch3.4) | 0 |
| 312 | `stle.b` | Load/Store (Ch3.4) | 0 |
| 313 | `stle.d` | Load/Store (Ch3.4) | 0 |
| 314 | `stle.h` | Load/Store (Ch3.4) | 0 |
| 315 | `stle.w` | Load/Store (Ch3.4) | 0 |
| 316 | `stptr.d` | Load/Store (Ch3.4) | 0 |
| 317 | `stptr.w` | Load/Store (Ch3.4) | 0 |
| 318 | `stx.b` | Load/Store (Ch3.4) | 0 |
| 319 | `stx.d` | Load/Store (Ch3.4) | 0 |
| 320 | `stx.h` | Load/Store (Ch3.4) | 0 |
| 321 | `stx.w` | Load/Store (Ch3.4) | 0 |
| 322 | `sub.d` | Arithmetic/Shift (Ch3.1) | 0 |
| 323 | `sub.w` | Arithmetic/Shift (Ch3.1) | 0 |
| 324 | `syscall` | Arithmetic/Shift (Ch3.1) | 0 |
| 325 | `tlbclr` | Privileged (Ch6) | 0 |
| 326 | `tlbfill` | Privileged (Ch6) | 0 |
| 327 | `tlbflush` | Privileged (Ch6) | 0 |
| 328 | `tlbrd` | Privileged (Ch6) | 0 |
| 329 | `tlbsrch` | Privileged (Ch6) | 0 |
| 330 | `tlbwr` | Privileged (Ch6) | 0 |
| 331 | `vabsd.b` | Vector LSX/LASX (Ch5) | 0 |
| 332 | `vabsd.bu` | Vector LSX/LASX (Ch5) | 0 |
| 333 | `vabsd.d` | Vector LSX/LASX (Ch5) | 0 |
| 334 | `vabsd.du` | Vector LSX/LASX (Ch5) | 0 |
| 335 | `vabsd.h` | Vector LSX/LASX (Ch5) | 0 |
| 336 | `vabsd.hu` | Vector LSX/LASX (Ch5) | 0 |
| 337 | `vabsd.w` | Vector LSX/LASX (Ch5) | 0 |
| 338 | `vabsd.wu` | Vector LSX/LASX (Ch5) | 0 |
| 339 | `vadd.b` | Vector LSX/LASX (Ch5) | 0 |
| 340 | `vadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 341 | `vadd.h` | Vector LSX/LASX (Ch5) | 0 |
| 342 | `vadd.q` | Vector LSX/LASX (Ch5) | 0 |
| 343 | `vadd.w` | Vector LSX/LASX (Ch5) | 0 |
| 344 | `vadda.b` | Vector LSX/LASX (Ch5) | 0 |
| 345 | `vadda.d` | Vector LSX/LASX (Ch5) | 0 |
| 346 | `vadda.h` | Vector LSX/LASX (Ch5) | 0 |
| 347 | `vadda.w` | Vector LSX/LASX (Ch5) | 0 |
| 348 | `vaddi.bu` | Vector LSX/LASX (Ch5) | 0 |
| 349 | `vaddi.du` | Vector LSX/LASX (Ch5) | 0 |
| 350 | `vaddi.hu` | Vector LSX/LASX (Ch5) | 0 |
| 351 | `vaddi.wu` | Vector LSX/LASX (Ch5) | 0 |
| 352 | `vaddwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 353 | `vaddwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 354 | `vaddwev.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 355 | `vaddwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 356 | `vaddwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 357 | `vaddwev.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 358 | `vaddwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 359 | `vaddwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 360 | `vaddwev.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 361 | `vaddwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 362 | `vaddwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 363 | `vaddwev.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 364 | `vaddwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 365 | `vaddwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 366 | `vaddwod.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 367 | `vaddwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 368 | `vaddwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 369 | `vaddwod.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 370 | `vaddwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 371 | `vaddwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 372 | `vaddwod.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 373 | `vaddwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 374 | `vaddwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 375 | `vaddwod.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 376 | `vand.v` | Vector LSX/LASX (Ch5) | 0 |
| 377 | `vandi.b` | Vector LSX/LASX (Ch5) | 0 |
| 378 | `vandn.v` | Vector LSX/LASX (Ch5) | 0 |
| 379 | `vavg.b` | Vector LSX/LASX (Ch5) | 0 |
| 380 | `vavg.bu` | Vector LSX/LASX (Ch5) | 0 |
| 381 | `vavg.d` | Vector LSX/LASX (Ch5) | 0 |
| 382 | `vavg.du` | Vector LSX/LASX (Ch5) | 0 |
| 383 | `vavg.h` | Vector LSX/LASX (Ch5) | 0 |
| 384 | `vavg.hu` | Vector LSX/LASX (Ch5) | 0 |
| 385 | `vavg.w` | Vector LSX/LASX (Ch5) | 0 |
| 386 | `vavg.wu` | Vector LSX/LASX (Ch5) | 0 |
| 387 | `vavgr.b` | Vector LSX/LASX (Ch5) | 0 |
| 388 | `vavgr.bu` | Vector LSX/LASX (Ch5) | 0 |
| 389 | `vavgr.d` | Vector LSX/LASX (Ch5) | 0 |
| 390 | `vavgr.du` | Vector LSX/LASX (Ch5) | 0 |
| 391 | `vavgr.h` | Vector LSX/LASX (Ch5) | 0 |
| 392 | `vavgr.hu` | Vector LSX/LASX (Ch5) | 0 |
| 393 | `vavgr.w` | Vector LSX/LASX (Ch5) | 0 |
| 394 | `vavgr.wu` | Vector LSX/LASX (Ch5) | 0 |
| 395 | `vbitclr.b` | Vector LSX/LASX (Ch5) | 0 |
| 396 | `vbitclr.d` | Vector LSX/LASX (Ch5) | 0 |
| 397 | `vbitclr.h` | Vector LSX/LASX (Ch5) | 0 |
| 398 | `vbitclr.w` | Vector LSX/LASX (Ch5) | 0 |
| 399 | `vbitclri.b` | Vector LSX/LASX (Ch5) | 0 |
| 400 | `vbitclri.d` | Vector LSX/LASX (Ch5) | 0 |
| 401 | `vbitclri.h` | Vector LSX/LASX (Ch5) | 0 |
| 402 | `vbitclri.w` | Vector LSX/LASX (Ch5) | 0 |
| 403 | `vbitrev.b` | Vector LSX/LASX (Ch5) | 0 |
| 404 | `vbitrev.d` | Vector LSX/LASX (Ch5) | 0 |
| 405 | `vbitrev.h` | Vector LSX/LASX (Ch5) | 0 |
| 406 | `vbitrev.w` | Vector LSX/LASX (Ch5) | 0 |
| 407 | `vbitrevi.b` | Vector LSX/LASX (Ch5) | 0 |
| 408 | `vbitrevi.d` | Vector LSX/LASX (Ch5) | 0 |
| 409 | `vbitrevi.h` | Vector LSX/LASX (Ch5) | 0 |
| 410 | `vbitrevi.w` | Vector LSX/LASX (Ch5) | 0 |
| 411 | `vbitsel.v` | Vector LSX/LASX (Ch5) | 0 |
| 412 | `vbitseli.b` | Vector LSX/LASX (Ch5) | 0 |
| 413 | `vbitset.b` | Vector LSX/LASX (Ch5) | 0 |
| 414 | `vbitset.d` | Vector LSX/LASX (Ch5) | 0 |
| 415 | `vbitset.h` | Vector LSX/LASX (Ch5) | 0 |
| 416 | `vbitset.w` | Vector LSX/LASX (Ch5) | 0 |
| 417 | `vbitseti.b` | Vector LSX/LASX (Ch5) | 0 |
| 418 | `vbitseti.d` | Vector LSX/LASX (Ch5) | 0 |
| 419 | `vbitseti.h` | Vector LSX/LASX (Ch5) | 0 |
| 420 | `vbitseti.w` | Vector LSX/LASX (Ch5) | 0 |
| 421 | `vbsll.v` | Vector LSX/LASX (Ch5) | 0 |
| 422 | `vbsrl.v` | Vector LSX/LASX (Ch5) | 0 |
| 423 | `vclo.b` | Vector LSX/LASX (Ch5) | 0 |
| 424 | `vclo.d` | Vector LSX/LASX (Ch5) | 0 |
| 425 | `vclo.h` | Vector LSX/LASX (Ch5) | 0 |
| 426 | `vclo.w` | Vector LSX/LASX (Ch5) | 0 |
| 427 | `vclz.b` | Vector LSX/LASX (Ch5) | 0 |
| 428 | `vclz.d` | Vector LSX/LASX (Ch5) | 0 |
| 429 | `vclz.h` | Vector LSX/LASX (Ch5) | 0 |
| 430 | `vclz.w` | Vector LSX/LASX (Ch5) | 0 |
| 431 | `vdiv.b` | Vector LSX/LASX (Ch5) | 0 |
| 432 | `vdiv.bu` | Vector LSX/LASX (Ch5) | 0 |
| 433 | `vdiv.d` | Vector LSX/LASX (Ch5) | 0 |
| 434 | `vdiv.du` | Vector LSX/LASX (Ch5) | 0 |
| 435 | `vdiv.h` | Vector LSX/LASX (Ch5) | 0 |
| 436 | `vdiv.hu` | Vector LSX/LASX (Ch5) | 0 |
| 437 | `vdiv.w` | Vector LSX/LASX (Ch5) | 0 |
| 438 | `vdiv.wu` | Vector LSX/LASX (Ch5) | 0 |
| 439 | `vext2xv.d.b` | Vector LSX/LASX (Ch5) | 0 |
| 440 | `vext2xv.d.h` | Vector LSX/LASX (Ch5) | 0 |
| 441 | `vext2xv.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 442 | `vext2xv.du.bu` | Vector LSX/LASX (Ch5) | 0 |
| 443 | `vext2xv.du.hu` | Vector LSX/LASX (Ch5) | 0 |
| 444 | `vext2xv.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 445 | `vext2xv.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 446 | `vext2xv.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 447 | `vext2xv.w.b` | Vector LSX/LASX (Ch5) | 0 |
| 448 | `vext2xv.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 449 | `vext2xv.wu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 450 | `vext2xv.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 451 | `vexth.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 452 | `vexth.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 453 | `vexth.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 454 | `vexth.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 455 | `vexth.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 456 | `vexth.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 457 | `vexth.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 458 | `vexth.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 459 | `vextl.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 460 | `vextl.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 461 | `vextrins.b` | Vector LSX/LASX (Ch5) | 0 |
| 462 | `vextrins.d` | Vector LSX/LASX (Ch5) | 0 |
| 463 | `vextrins.h` | Vector LSX/LASX (Ch5) | 0 |
| 464 | `vextrins.w` | Vector LSX/LASX (Ch5) | 0 |
| 465 | `vfadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 466 | `vfadd.s` | Vector LSX/LASX (Ch5) | 0 |
| 467 | `vfclass.d` | Vector LSX/LASX (Ch5) | 0 |
| 468 | `vfclass.s` | Vector LSX/LASX (Ch5) | 0 |
| 469 | `vfcmp.cond.d` | Vector LSX/LASX (Ch5) | 0 |
| 470 | `vfcmp.cond.s` | Vector LSX/LASX (Ch5) | 0 |
| 471 | `vfcvt.h.s` | Vector LSX/LASX (Ch5) | 0 |
| 472 | `vfcvt.s.d` | Vector LSX/LASX (Ch5) | 0 |
| 473 | `vfcvth.d.s` | Vector LSX/LASX (Ch5) | 0 |
| 474 | `vfcvth.s.h` | Vector LSX/LASX (Ch5) | 0 |
| 475 | `vfcvtl.d.s` | Vector LSX/LASX (Ch5) | 0 |
| 476 | `vfcvtl.s.h` | Vector LSX/LASX (Ch5) | 0 |
| 477 | `vfdiv.d` | Vector LSX/LASX (Ch5) | 0 |
| 478 | `vfdiv.s` | Vector LSX/LASX (Ch5) | 0 |
| 479 | `vffint.d.l` | Vector LSX/LASX (Ch5) | 0 |
| 480 | `vffint.d.lu` | Vector LSX/LASX (Ch5) | 0 |
| 481 | `vffint.s.l` | Vector LSX/LASX (Ch5) | 0 |
| 482 | `vffint.s.w` | Vector LSX/LASX (Ch5) | 0 |
| 483 | `vffint.s.wu` | Vector LSX/LASX (Ch5) | 0 |
| 484 | `vffinth.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 485 | `vffintl.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 486 | `vflogb.d` | Vector LSX/LASX (Ch5) | 0 |
| 487 | `vflogb.s` | Vector LSX/LASX (Ch5) | 0 |
| 488 | `vfmadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 489 | `vfmadd.s` | Vector LSX/LASX (Ch5) | 0 |
| 490 | `vfmax.d` | Vector LSX/LASX (Ch5) | 0 |
| 491 | `vfmax.s` | Vector LSX/LASX (Ch5) | 0 |
| 492 | `vfmaxa.d` | Vector LSX/LASX (Ch5) | 0 |
| 493 | `vfmaxa.s` | Vector LSX/LASX (Ch5) | 0 |
| 494 | `vfmin.d` | Vector LSX/LASX (Ch5) | 0 |
| 495 | `vfmin.s` | Vector LSX/LASX (Ch5) | 0 |
| 496 | `vfmina.d` | Vector LSX/LASX (Ch5) | 0 |
| 497 | `vfmina.s` | Vector LSX/LASX (Ch5) | 0 |
| 498 | `vfmsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 499 | `vfmsub.s` | Vector LSX/LASX (Ch5) | 0 |
| 500 | `vfmul.d` | Vector LSX/LASX (Ch5) | 0 |
| 501 | `vfmul.s` | Vector LSX/LASX (Ch5) | 0 |
| 502 | `vfnmadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 503 | `vfnmadd.s` | Vector LSX/LASX (Ch5) | 0 |
| 504 | `vfnmsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 505 | `vfnmsub.s` | Vector LSX/LASX (Ch5) | 0 |
| 506 | `vfrecip.d` | Vector LSX/LASX (Ch5) | 0 |
| 507 | `vfrecip.s` | Vector LSX/LASX (Ch5) | 0 |
| 508 | `vfrint.d` | Vector LSX/LASX (Ch5) | 0 |
| 509 | `vfrint.s` | Vector LSX/LASX (Ch5) | 0 |
| 510 | `vfrintrm.d` | Vector LSX/LASX (Ch5) | 0 |
| 511 | `vfrintrm.s` | Vector LSX/LASX (Ch5) | 0 |
| 512 | `vfrintrne.d` | Vector LSX/LASX (Ch5) | 0 |
| 513 | `vfrintrne.s` | Vector LSX/LASX (Ch5) | 0 |
| 514 | `vfrintrp.d` | Vector LSX/LASX (Ch5) | 0 |
| 515 | `vfrintrp.s` | Vector LSX/LASX (Ch5) | 0 |
| 516 | `vfrintrz.d` | Vector LSX/LASX (Ch5) | 0 |
| 517 | `vfrintrz.s` | Vector LSX/LASX (Ch5) | 0 |
| 518 | `vfrsqrt.d` | Vector LSX/LASX (Ch5) | 0 |
| 519 | `vfrsqrt.s` | Vector LSX/LASX (Ch5) | 0 |
| 520 | `vfrstp.b` | Vector LSX/LASX (Ch5) | 0 |
| 521 | `vfrstp.h` | Vector LSX/LASX (Ch5) | 0 |
| 522 | `vfrstpi.b` | Vector LSX/LASX (Ch5) | 0 |
| 523 | `vfrstpi.h` | Vector LSX/LASX (Ch5) | 0 |
| 524 | `vfsqrt.d` | Vector LSX/LASX (Ch5) | 0 |
| 525 | `vfsqrt.s` | Vector LSX/LASX (Ch5) | 0 |
| 526 | `vfsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 527 | `vfsub.s` | Vector LSX/LASX (Ch5) | 0 |
| 528 | `vftint.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 529 | `vftint.lu.d` | Vector LSX/LASX (Ch5) | 0 |
| 530 | `vftint.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 531 | `vftint.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 532 | `vftint.wu.s` | Vector LSX/LASX (Ch5) | 0 |
| 533 | `vftinth.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 534 | `vftintl.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 535 | `vftintrm.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 536 | `vftintrm.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 537 | `vftintrm.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 538 | `vftintrmh.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 539 | `vftintrml.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 540 | `vftintrne.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 541 | `vftintrne.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 542 | `vftintrne.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 543 | `vftintrneh.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 544 | `vftintrnel.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 545 | `vftintrp.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 546 | `vftintrp.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 547 | `vftintrp.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 548 | `vftintrph.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 549 | `vftintrpl.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 550 | `vftintrz.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 551 | `vftintrz.lu.d` | Vector LSX/LASX (Ch5) | 0 |
| 552 | `vftintrz.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 553 | `vftintrz.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 554 | `vftintrz.wu.s` | Vector LSX/LASX (Ch5) | 0 |
| 555 | `vftintrzh.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 556 | `vftintrzl.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 557 | `vhaddw.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 558 | `vhaddw.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 559 | `vhaddw.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 560 | `vhaddw.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 561 | `vhaddw.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 562 | `vhaddw.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 563 | `vhaddw.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 564 | `vhaddw.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 565 | `vhsubw.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 566 | `vhsubw.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 567 | `vhsubw.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 568 | `vhsubw.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 569 | `vhsubw.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 570 | `vhsubw.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 571 | `vhsubw.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 572 | `vhsubw.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 573 | `vilvh.b` | Vector LSX/LASX (Ch5) | 0 |
| 574 | `vilvh.d` | Vector LSX/LASX (Ch5) | 0 |
| 575 | `vilvh.h` | Vector LSX/LASX (Ch5) | 0 |
| 576 | `vilvh.w` | Vector LSX/LASX (Ch5) | 0 |
| 577 | `vilvl.b` | Vector LSX/LASX (Ch5) | 0 |
| 578 | `vilvl.d` | Vector LSX/LASX (Ch5) | 0 |
| 579 | `vilvl.h` | Vector LSX/LASX (Ch5) | 0 |
| 580 | `vilvl.w` | Vector LSX/LASX (Ch5) | 0 |
| 581 | `vinsgr2vr.b` | Vector LSX/LASX (Ch5) | 0 |
| 582 | `vinsgr2vr.d` | Vector LSX/LASX (Ch5) | 0 |
| 583 | `vinsgr2vr.h` | Vector LSX/LASX (Ch5) | 0 |
| 584 | `vinsgr2vr.w` | Vector LSX/LASX (Ch5) | 0 |
| 585 | `vld` | Vector LSX/LASX (Ch5) | 0 |
| 586 | `vldi` | Vector LSX/LASX (Ch5) | 0 |
| 587 | `vldrepl.b` | Vector LSX/LASX (Ch5) | 0 |
| 588 | `vldrepl.d` | Vector LSX/LASX (Ch5) | 0 |
| 589 | `vldrepl.h` | Vector LSX/LASX (Ch5) | 0 |
| 590 | `vldrepl.w` | Vector LSX/LASX (Ch5) | 0 |
| 591 | `vldx` | Vector LSX/LASX (Ch5) | 0 |
| 592 | `vmadd.b` | Vector LSX/LASX (Ch5) | 0 |
| 593 | `vmadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 594 | `vmadd.h` | Vector LSX/LASX (Ch5) | 0 |
| 595 | `vmadd.w` | Vector LSX/LASX (Ch5) | 0 |
| 596 | `vmaddwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 597 | `vmaddwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 598 | `vmaddwev.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 599 | `vmaddwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 600 | `vmaddwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 601 | `vmaddwev.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 602 | `vmaddwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 603 | `vmaddwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 604 | `vmaddwev.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 605 | `vmaddwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 606 | `vmaddwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 607 | `vmaddwev.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 608 | `vmaddwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 609 | `vmaddwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 610 | `vmaddwod.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 611 | `vmaddwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 612 | `vmaddwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 613 | `vmaddwod.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 614 | `vmaddwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 615 | `vmaddwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 616 | `vmaddwod.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 617 | `vmaddwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 618 | `vmaddwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 619 | `vmaddwod.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 620 | `vmax.b` | Vector LSX/LASX (Ch5) | 0 |
| 621 | `vmax.bu` | Vector LSX/LASX (Ch5) | 0 |
| 622 | `vmax.d` | Vector LSX/LASX (Ch5) | 0 |
| 623 | `vmax.du` | Vector LSX/LASX (Ch5) | 0 |
| 624 | `vmax.h` | Vector LSX/LASX (Ch5) | 0 |
| 625 | `vmax.hu` | Vector LSX/LASX (Ch5) | 0 |
| 626 | `vmax.w` | Vector LSX/LASX (Ch5) | 0 |
| 627 | `vmax.wu` | Vector LSX/LASX (Ch5) | 0 |
| 628 | `vmaxi.b` | Vector LSX/LASX (Ch5) | 0 |
| 629 | `vmaxi.bu` | Vector LSX/LASX (Ch5) | 0 |
| 630 | `vmaxi.d` | Vector LSX/LASX (Ch5) | 0 |
| 631 | `vmaxi.du` | Vector LSX/LASX (Ch5) | 0 |
| 632 | `vmaxi.h` | Vector LSX/LASX (Ch5) | 0 |
| 633 | `vmaxi.hu` | Vector LSX/LASX (Ch5) | 0 |
| 634 | `vmaxi.w` | Vector LSX/LASX (Ch5) | 0 |
| 635 | `vmaxi.wu` | Vector LSX/LASX (Ch5) | 0 |
| 636 | `vmin.b` | Vector LSX/LASX (Ch5) | 0 |
| 637 | `vmin.bu` | Vector LSX/LASX (Ch5) | 0 |
| 638 | `vmin.d` | Vector LSX/LASX (Ch5) | 0 |
| 639 | `vmin.du` | Vector LSX/LASX (Ch5) | 0 |
| 640 | `vmin.h` | Vector LSX/LASX (Ch5) | 0 |
| 641 | `vmin.hu` | Vector LSX/LASX (Ch5) | 0 |
| 642 | `vmin.w` | Vector LSX/LASX (Ch5) | 0 |
| 643 | `vmin.wu` | Vector LSX/LASX (Ch5) | 0 |
| 644 | `vmini.b` | Vector LSX/LASX (Ch5) | 0 |
| 645 | `vmini.bu` | Vector LSX/LASX (Ch5) | 0 |
| 646 | `vmini.d` | Vector LSX/LASX (Ch5) | 0 |
| 647 | `vmini.du` | Vector LSX/LASX (Ch5) | 0 |
| 648 | `vmini.h` | Vector LSX/LASX (Ch5) | 0 |
| 649 | `vmini.hu` | Vector LSX/LASX (Ch5) | 0 |
| 650 | `vmini.w` | Vector LSX/LASX (Ch5) | 0 |
| 651 | `vmini.wu` | Vector LSX/LASX (Ch5) | 0 |
| 652 | `vmod.b` | Vector LSX/LASX (Ch5) | 0 |
| 653 | `vmod.bu` | Vector LSX/LASX (Ch5) | 0 |
| 654 | `vmod.d` | Vector LSX/LASX (Ch5) | 0 |
| 655 | `vmod.du` | Vector LSX/LASX (Ch5) | 0 |
| 656 | `vmod.h` | Vector LSX/LASX (Ch5) | 0 |
| 657 | `vmod.hu` | Vector LSX/LASX (Ch5) | 0 |
| 658 | `vmod.w` | Vector LSX/LASX (Ch5) | 0 |
| 659 | `vmod.wu` | Vector LSX/LASX (Ch5) | 0 |
| 660 | `vmskgez.b` | Vector LSX/LASX (Ch5) | 0 |
| 661 | `vmskltz.b` | Vector LSX/LASX (Ch5) | 0 |
| 662 | `vmskltz.d` | Vector LSX/LASX (Ch5) | 0 |
| 663 | `vmskltz.h` | Vector LSX/LASX (Ch5) | 0 |
| 664 | `vmskltz.w` | Vector LSX/LASX (Ch5) | 0 |
| 665 | `vmsknz.b` | Vector LSX/LASX (Ch5) | 0 |
| 666 | `vmsub.b` | Vector LSX/LASX (Ch5) | 0 |
| 667 | `vmsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 668 | `vmsub.h` | Vector LSX/LASX (Ch5) | 0 |
| 669 | `vmsub.w` | Vector LSX/LASX (Ch5) | 0 |
| 670 | `vmuh.b` | Vector LSX/LASX (Ch5) | 0 |
| 671 | `vmuh.bu` | Vector LSX/LASX (Ch5) | 0 |
| 672 | `vmuh.d` | Vector LSX/LASX (Ch5) | 0 |
| 673 | `vmuh.du` | Vector LSX/LASX (Ch5) | 0 |
| 674 | `vmuh.h` | Vector LSX/LASX (Ch5) | 0 |
| 675 | `vmuh.hu` | Vector LSX/LASX (Ch5) | 0 |
| 676 | `vmuh.w` | Vector LSX/LASX (Ch5) | 0 |
| 677 | `vmuh.wu` | Vector LSX/LASX (Ch5) | 0 |
| 678 | `vmul.b` | Vector LSX/LASX (Ch5) | 0 |
| 679 | `vmul.d` | Vector LSX/LASX (Ch5) | 0 |
| 680 | `vmul.h` | Vector LSX/LASX (Ch5) | 0 |
| 681 | `vmul.w` | Vector LSX/LASX (Ch5) | 0 |
| 682 | `vmulwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 683 | `vmulwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 684 | `vmulwev.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 685 | `vmulwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 686 | `vmulwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 687 | `vmulwev.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 688 | `vmulwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 689 | `vmulwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 690 | `vmulwev.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 691 | `vmulwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 692 | `vmulwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 693 | `vmulwev.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 694 | `vmulwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 695 | `vmulwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 696 | `vmulwod.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 697 | `vmulwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 698 | `vmulwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 699 | `vmulwod.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 700 | `vmulwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 701 | `vmulwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 702 | `vmulwod.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 703 | `vmulwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 704 | `vmulwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 705 | `vmulwod.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 706 | `vneg.b` | Vector LSX/LASX (Ch5) | 0 |
| 707 | `vneg.d` | Vector LSX/LASX (Ch5) | 0 |
| 708 | `vneg.h` | Vector LSX/LASX (Ch5) | 0 |
| 709 | `vneg.w` | Vector LSX/LASX (Ch5) | 0 |
| 710 | `vnor.v` | Vector LSX/LASX (Ch5) | 0 |
| 711 | `vnori.b` | Vector LSX/LASX (Ch5) | 0 |
| 712 | `vor.v` | Vector LSX/LASX (Ch5) | 0 |
| 713 | `vori.b` | Vector LSX/LASX (Ch5) | 0 |
| 714 | `vorn.v` | Vector LSX/LASX (Ch5) | 0 |
| 715 | `vpackev.b` | Vector LSX/LASX (Ch5) | 0 |
| 716 | `vpackev.d` | Vector LSX/LASX (Ch5) | 0 |
| 717 | `vpackev.h` | Vector LSX/LASX (Ch5) | 0 |
| 718 | `vpackev.w` | Vector LSX/LASX (Ch5) | 0 |
| 719 | `vpackod.b` | Vector LSX/LASX (Ch5) | 0 |
| 720 | `vpackod.d` | Vector LSX/LASX (Ch5) | 0 |
| 721 | `vpackod.h` | Vector LSX/LASX (Ch5) | 0 |
| 722 | `vpackod.w` | Vector LSX/LASX (Ch5) | 0 |
| 723 | `vpcnt.b` | Vector LSX/LASX (Ch5) | 0 |
| 724 | `vpcnt.d` | Vector LSX/LASX (Ch5) | 0 |
| 725 | `vpcnt.h` | Vector LSX/LASX (Ch5) | 0 |
| 726 | `vpcnt.w` | Vector LSX/LASX (Ch5) | 0 |
| 727 | `vpermi.w` | Vector LSX/LASX (Ch5) | 0 |
| 728 | `vpickev.b` | Vector LSX/LASX (Ch5) | 0 |
| 729 | `vpickev.d` | Vector LSX/LASX (Ch5) | 0 |
| 730 | `vpickev.h` | Vector LSX/LASX (Ch5) | 0 |
| 731 | `vpickev.w` | Vector LSX/LASX (Ch5) | 0 |
| 732 | `vpickod.b` | Vector LSX/LASX (Ch5) | 0 |
| 733 | `vpickod.d` | Vector LSX/LASX (Ch5) | 0 |
| 734 | `vpickod.h` | Vector LSX/LASX (Ch5) | 0 |
| 735 | `vpickod.w` | Vector LSX/LASX (Ch5) | 0 |
| 736 | `vpickve2gr.b` | Vector LSX/LASX (Ch5) | 0 |
| 737 | `vpickve2gr.bu` | Vector LSX/LASX (Ch5) | 0 |
| 738 | `vpickve2gr.d` | Vector LSX/LASX (Ch5) | 0 |
| 739 | `vpickve2gr.du` | Vector LSX/LASX (Ch5) | 0 |
| 740 | `vpickve2gr.h` | Vector LSX/LASX (Ch5) | 0 |
| 741 | `vpickve2gr.hu` | Vector LSX/LASX (Ch5) | 0 |
| 742 | `vpickve2gr.w` | Vector LSX/LASX (Ch5) | 0 |
| 743 | `vpickve2gr.wu` | Vector LSX/LASX (Ch5) | 0 |
| 744 | `vreplgr2vr.b` | Vector LSX/LASX (Ch5) | 0 |
| 745 | `vreplgr2vr.d` | Vector LSX/LASX (Ch5) | 0 |
| 746 | `vreplgr2vr.h` | Vector LSX/LASX (Ch5) | 0 |
| 747 | `vreplgr2vr.w` | Vector LSX/LASX (Ch5) | 0 |
| 748 | `vreplve.b` | Vector LSX/LASX (Ch5) | 0 |
| 749 | `vreplve.d` | Vector LSX/LASX (Ch5) | 0 |
| 750 | `vreplve.h` | Vector LSX/LASX (Ch5) | 0 |
| 751 | `vreplve.w` | Vector LSX/LASX (Ch5) | 0 |
| 752 | `vreplvei.b` | Vector LSX/LASX (Ch5) | 0 |
| 753 | `vreplvei.d` | Vector LSX/LASX (Ch5) | 0 |
| 754 | `vreplvei.h` | Vector LSX/LASX (Ch5) | 0 |
| 755 | `vreplvei.w` | Vector LSX/LASX (Ch5) | 0 |
| 756 | `vrotr.b` | Vector LSX/LASX (Ch5) | 0 |
| 757 | `vrotr.d` | Vector LSX/LASX (Ch5) | 0 |
| 758 | `vrotr.h` | Vector LSX/LASX (Ch5) | 0 |
| 759 | `vrotr.w` | Vector LSX/LASX (Ch5) | 0 |
| 760 | `vrotri.b` | Vector LSX/LASX (Ch5) | 0 |
| 761 | `vrotri.d` | Vector LSX/LASX (Ch5) | 0 |
| 762 | `vrotri.h` | Vector LSX/LASX (Ch5) | 0 |
| 763 | `vrotri.w` | Vector LSX/LASX (Ch5) | 0 |
| 764 | `vsadd.b` | Vector LSX/LASX (Ch5) | 0 |
| 765 | `vsadd.bu` | Vector LSX/LASX (Ch5) | 0 |
| 766 | `vsadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 767 | `vsadd.du` | Vector LSX/LASX (Ch5) | 0 |
| 768 | `vsadd.h` | Vector LSX/LASX (Ch5) | 0 |
| 769 | `vsadd.hu` | Vector LSX/LASX (Ch5) | 0 |
| 770 | `vsadd.w` | Vector LSX/LASX (Ch5) | 0 |
| 771 | `vsadd.wu` | Vector LSX/LASX (Ch5) | 0 |
| 772 | `vsat.b` | Vector LSX/LASX (Ch5) | 0 |
| 773 | `vsat.bu` | Vector LSX/LASX (Ch5) | 0 |
| 774 | `vsat.d` | Vector LSX/LASX (Ch5) | 0 |
| 775 | `vsat.du` | Vector LSX/LASX (Ch5) | 0 |
| 776 | `vsat.h` | Vector LSX/LASX (Ch5) | 0 |
| 777 | `vsat.hu` | Vector LSX/LASX (Ch5) | 0 |
| 778 | `vsat.w` | Vector LSX/LASX (Ch5) | 0 |
| 779 | `vsat.wu` | Vector LSX/LASX (Ch5) | 0 |
| 780 | `vseq.b` | Vector LSX/LASX (Ch5) | 0 |
| 781 | `vseq.d` | Vector LSX/LASX (Ch5) | 0 |
| 782 | `vseq.h` | Vector LSX/LASX (Ch5) | 0 |
| 783 | `vseq.w` | Vector LSX/LASX (Ch5) | 0 |
| 784 | `vseqi.b` | Vector LSX/LASX (Ch5) | 0 |
| 785 | `vseqi.d` | Vector LSX/LASX (Ch5) | 0 |
| 786 | `vseqi.h` | Vector LSX/LASX (Ch5) | 0 |
| 787 | `vseqi.w` | Vector LSX/LASX (Ch5) | 0 |
| 788 | `vsetallnez.b` | Vector LSX/LASX (Ch5) | 0 |
| 789 | `vsetallnez.d` | Vector LSX/LASX (Ch5) | 0 |
| 790 | `vsetallnez.h` | Vector LSX/LASX (Ch5) | 0 |
| 791 | `vsetallnez.w` | Vector LSX/LASX (Ch5) | 0 |
| 792 | `vsetanyeqz.b` | Vector LSX/LASX (Ch5) | 0 |
| 793 | `vsetanyeqz.d` | Vector LSX/LASX (Ch5) | 0 |
| 794 | `vsetanyeqz.h` | Vector LSX/LASX (Ch5) | 0 |
| 795 | `vsetanyeqz.w` | Vector LSX/LASX (Ch5) | 0 |
| 796 | `vseteqz.v` | Vector LSX/LASX (Ch5) | 0 |
| 797 | `vsetnez.v` | Vector LSX/LASX (Ch5) | 0 |
| 798 | `vshuf4i.b` | Vector LSX/LASX (Ch5) | 0 |
| 799 | `vshuf4i.d` | Vector LSX/LASX (Ch5) | 0 |
| 800 | `vshuf4i.h` | Vector LSX/LASX (Ch5) | 0 |
| 801 | `vshuf4i.w` | Vector LSX/LASX (Ch5) | 0 |
| 802 | `vshuf.b` | Vector LSX/LASX (Ch5) | 0 |
| 803 | `vshuf.d` | Vector LSX/LASX (Ch5) | 0 |
| 804 | `vshuf.h` | Vector LSX/LASX (Ch5) | 0 |
| 805 | `vshuf.w` | Vector LSX/LASX (Ch5) | 0 |
| 806 | `vsigncov.b` | Vector LSX/LASX (Ch5) | 0 |
| 807 | `vsigncov.d` | Vector LSX/LASX (Ch5) | 0 |
| 808 | `vsigncov.h` | Vector LSX/LASX (Ch5) | 0 |
| 809 | `vsigncov.w` | Vector LSX/LASX (Ch5) | 0 |
| 810 | `vsle.b` | Vector LSX/LASX (Ch5) | 0 |
| 811 | `vsle.bu` | Vector LSX/LASX (Ch5) | 0 |
| 812 | `vsle.d` | Vector LSX/LASX (Ch5) | 0 |
| 813 | `vsle.du` | Vector LSX/LASX (Ch5) | 0 |
| 814 | `vsle.h` | Vector LSX/LASX (Ch5) | 0 |
| 815 | `vsle.hu` | Vector LSX/LASX (Ch5) | 0 |
| 816 | `vsle.w` | Vector LSX/LASX (Ch5) | 0 |
| 817 | `vsle.wu` | Vector LSX/LASX (Ch5) | 0 |
| 818 | `vslei.b` | Vector LSX/LASX (Ch5) | 0 |
| 819 | `vslei.bu` | Vector LSX/LASX (Ch5) | 0 |
| 820 | `vslei.d` | Vector LSX/LASX (Ch5) | 0 |
| 821 | `vslei.du` | Vector LSX/LASX (Ch5) | 0 |
| 822 | `vslei.h` | Vector LSX/LASX (Ch5) | 0 |
| 823 | `vslei.hu` | Vector LSX/LASX (Ch5) | 0 |
| 824 | `vslei.w` | Vector LSX/LASX (Ch5) | 0 |
| 825 | `vslei.wu` | Vector LSX/LASX (Ch5) | 0 |
| 826 | `vsll.b` | Vector LSX/LASX (Ch5) | 0 |
| 827 | `vsll.d` | Vector LSX/LASX (Ch5) | 0 |
| 828 | `vsll.h` | Vector LSX/LASX (Ch5) | 0 |
| 829 | `vsll.w` | Vector LSX/LASX (Ch5) | 0 |
| 830 | `vslli.b` | Vector LSX/LASX (Ch5) | 0 |
| 831 | `vslli.d` | Vector LSX/LASX (Ch5) | 0 |
| 832 | `vslli.h` | Vector LSX/LASX (Ch5) | 0 |
| 833 | `vslli.w` | Vector LSX/LASX (Ch5) | 0 |
| 834 | `vsllwil.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 835 | `vsllwil.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 836 | `vsllwil.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 837 | `vsllwil.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 838 | `vsllwil.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 839 | `vsllwil.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 840 | `vslt.b` | Vector LSX/LASX (Ch5) | 0 |
| 841 | `vslt.bu` | Vector LSX/LASX (Ch5) | 0 |
| 842 | `vslt.d` | Vector LSX/LASX (Ch5) | 0 |
| 843 | `vslt.du` | Vector LSX/LASX (Ch5) | 0 |
| 844 | `vslt.h` | Vector LSX/LASX (Ch5) | 0 |
| 845 | `vslt.hu` | Vector LSX/LASX (Ch5) | 0 |
| 846 | `vslt.w` | Vector LSX/LASX (Ch5) | 0 |
| 847 | `vslt.wu` | Vector LSX/LASX (Ch5) | 0 |
| 848 | `vslti.b` | Vector LSX/LASX (Ch5) | 0 |
| 849 | `vslti.bu` | Vector LSX/LASX (Ch5) | 0 |
| 850 | `vslti.d` | Vector LSX/LASX (Ch5) | 0 |
| 851 | `vslti.du` | Vector LSX/LASX (Ch5) | 0 |
| 852 | `vslti.h` | Vector LSX/LASX (Ch5) | 0 |
| 853 | `vslti.hu` | Vector LSX/LASX (Ch5) | 0 |
| 854 | `vslti.w` | Vector LSX/LASX (Ch5) | 0 |
| 855 | `vslti.wu` | Vector LSX/LASX (Ch5) | 0 |
| 856 | `vsra.b` | Vector LSX/LASX (Ch5) | 0 |
| 857 | `vsra.d` | Vector LSX/LASX (Ch5) | 0 |
| 858 | `vsra.h` | Vector LSX/LASX (Ch5) | 0 |
| 859 | `vsra.w` | Vector LSX/LASX (Ch5) | 0 |
| 860 | `vsrai.b` | Vector LSX/LASX (Ch5) | 0 |
| 861 | `vsrai.d` | Vector LSX/LASX (Ch5) | 0 |
| 862 | `vsrai.h` | Vector LSX/LASX (Ch5) | 0 |
| 863 | `vsrai.w` | Vector LSX/LASX (Ch5) | 0 |
| 864 | `vsran.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 865 | `vsran.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 866 | `vsran.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 867 | `vsrani.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 868 | `vsrani.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 869 | `vsrani.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 870 | `vsrani.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 871 | `vsrar.b` | Vector LSX/LASX (Ch5) | 0 |
| 872 | `vsrar.d` | Vector LSX/LASX (Ch5) | 0 |
| 873 | `vsrar.h` | Vector LSX/LASX (Ch5) | 0 |
| 874 | `vsrar.w` | Vector LSX/LASX (Ch5) | 0 |
| 875 | `vsrari.b` | Vector LSX/LASX (Ch5) | 0 |
| 876 | `vsrari.d` | Vector LSX/LASX (Ch5) | 0 |
| 877 | `vsrari.h` | Vector LSX/LASX (Ch5) | 0 |
| 878 | `vsrari.w` | Vector LSX/LASX (Ch5) | 0 |
| 879 | `vsrarn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 880 | `vsrarn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 881 | `vsrarn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 882 | `vsrarni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 883 | `vsrarni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 884 | `vsrarni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 885 | `vsrarni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 886 | `vsrl.b` | Vector LSX/LASX (Ch5) | 0 |
| 887 | `vsrl.d` | Vector LSX/LASX (Ch5) | 0 |
| 888 | `vsrl.h` | Vector LSX/LASX (Ch5) | 0 |
| 889 | `vsrl.w` | Vector LSX/LASX (Ch5) | 0 |
| 890 | `vsrli.b` | Vector LSX/LASX (Ch5) | 0 |
| 891 | `vsrli.d` | Vector LSX/LASX (Ch5) | 0 |
| 892 | `vsrli.h` | Vector LSX/LASX (Ch5) | 0 |
| 893 | `vsrli.w` | Vector LSX/LASX (Ch5) | 0 |
| 894 | `vsrln.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 895 | `vsrln.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 896 | `vsrln.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 897 | `vsrlni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 898 | `vsrlni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 899 | `vsrlni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 900 | `vsrlni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 901 | `vsrlr.b` | Vector LSX/LASX (Ch5) | 0 |
| 902 | `vsrlr.d` | Vector LSX/LASX (Ch5) | 0 |
| 903 | `vsrlr.h` | Vector LSX/LASX (Ch5) | 0 |
| 904 | `vsrlr.w` | Vector LSX/LASX (Ch5) | 0 |
| 905 | `vsrlri.b` | Vector LSX/LASX (Ch5) | 0 |
| 906 | `vsrlri.d` | Vector LSX/LASX (Ch5) | 0 |
| 907 | `vsrlri.h` | Vector LSX/LASX (Ch5) | 0 |
| 908 | `vsrlri.w` | Vector LSX/LASX (Ch5) | 0 |
| 909 | `vsrlrn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 910 | `vsrlrn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 911 | `vsrlrn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 912 | `vsrlrni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 913 | `vsrlrni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 914 | `vsrlrni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 915 | `vsrlrni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 916 | `vssran.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 917 | `vssran.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 918 | `vssran.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 919 | `vssran.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 920 | `vssran.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 921 | `vssran.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 922 | `vssrani.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 923 | `vssrani.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 924 | `vssrani.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 925 | `vssrani.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 926 | `vssrani.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 927 | `vssrani.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 928 | `vssrani.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 929 | `vssrani.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 930 | `vssrarn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 931 | `vssrarn.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 932 | `vssrarn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 933 | `vssrarn.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 934 | `vssrarn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 935 | `vssrarn.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 936 | `vssrarni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 937 | `vssrarni.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 938 | `vssrarni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 939 | `vssrarni.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 940 | `vssrarni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 941 | `vssrarni.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 942 | `vssrarni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 943 | `vssrarni.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 944 | `vssrln.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 945 | `vssrln.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 946 | `vssrln.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 947 | `vssrln.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 948 | `vssrln.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 949 | `vssrln.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 950 | `vssrlni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 951 | `vssrlni.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 952 | `vssrlni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 953 | `vssrlni.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 954 | `vssrlni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 955 | `vssrlni.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 956 | `vssrlni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 957 | `vssrlni.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 958 | `vssrlrn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 959 | `vssrlrn.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 960 | `vssrlrn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 961 | `vssrlrn.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 962 | `vssrlrn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 963 | `vssrlrn.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 964 | `vssrlrni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 965 | `vssrlrni.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 966 | `vssrlrni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 967 | `vssrlrni.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 968 | `vssrlrni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 969 | `vssrlrni.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 970 | `vssrlrni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 971 | `vssrlrni.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 972 | `vssub.b` | Vector LSX/LASX (Ch5) | 0 |
| 973 | `vssub.bu` | Vector LSX/LASX (Ch5) | 0 |
| 974 | `vssub.d` | Vector LSX/LASX (Ch5) | 0 |
| 975 | `vssub.du` | Vector LSX/LASX (Ch5) | 0 |
| 976 | `vssub.h` | Vector LSX/LASX (Ch5) | 0 |
| 977 | `vssub.hu` | Vector LSX/LASX (Ch5) | 0 |
| 978 | `vssub.w` | Vector LSX/LASX (Ch5) | 0 |
| 979 | `vssub.wu` | Vector LSX/LASX (Ch5) | 0 |
| 980 | `vst` | Vector LSX/LASX (Ch5) | 0 |
| 981 | `vstelm.b` | Vector LSX/LASX (Ch5) | 0 |
| 982 | `vstelm.d` | Vector LSX/LASX (Ch5) | 0 |
| 983 | `vstelm.h` | Vector LSX/LASX (Ch5) | 0 |
| 984 | `vstelm.w` | Vector LSX/LASX (Ch5) | 0 |
| 985 | `vstx` | Vector LSX/LASX (Ch5) | 0 |
| 986 | `vsub.b` | Vector LSX/LASX (Ch5) | 0 |
| 987 | `vsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 988 | `vsub.h` | Vector LSX/LASX (Ch5) | 0 |
| 989 | `vsub.q` | Vector LSX/LASX (Ch5) | 0 |
| 990 | `vsub.w` | Vector LSX/LASX (Ch5) | 0 |
| 991 | `vsubi.bu` | Vector LSX/LASX (Ch5) | 0 |
| 992 | `vsubi.du` | Vector LSX/LASX (Ch5) | 0 |
| 993 | `vsubi.hu` | Vector LSX/LASX (Ch5) | 0 |
| 994 | `vsubi.wu` | Vector LSX/LASX (Ch5) | 0 |
| 995 | `vsubwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 996 | `vsubwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 997 | `vsubwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 998 | `vsubwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 999 | `vsubwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1000 | `vsubwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1001 | `vsubwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1002 | `vsubwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1003 | `vsubwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1004 | `vsubwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1005 | `vsubwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1006 | `vsubwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1007 | `vsubwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1008 | `vsubwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1009 | `vsubwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1010 | `vsubwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1011 | `vxor.v` | Vector LSX/LASX (Ch5) | 0 |
| 1012 | `vxori.b` | Vector LSX/LASX (Ch5) | 0 |
| 1013 | `xor` | Arithmetic/Shift (Ch3.1) | 0 |
| 1014 | `xori` | Arithmetic/Shift (Ch3.1) | 0 |
| 1015 | `xvabsd.b` | Vector LSX/LASX (Ch5) | 0 |
| 1016 | `xvabsd.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1017 | `xvabsd.d` | Vector LSX/LASX (Ch5) | 0 |
| 1018 | `xvabsd.du` | Vector LSX/LASX (Ch5) | 0 |
| 1019 | `xvabsd.h` | Vector LSX/LASX (Ch5) | 0 |
| 1020 | `xvabsd.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1021 | `xvabsd.w` | Vector LSX/LASX (Ch5) | 0 |
| 1022 | `xvabsd.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1023 | `xvadd.b` | Vector LSX/LASX (Ch5) | 0 |
| 1024 | `xvadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 1025 | `xvadd.h` | Vector LSX/LASX (Ch5) | 0 |
| 1026 | `xvadd.q` | Vector LSX/LASX (Ch5) | 0 |
| 1027 | `xvadd.w` | Vector LSX/LASX (Ch5) | 0 |
| 1028 | `xvadda.b` | Vector LSX/LASX (Ch5) | 0 |
| 1029 | `xvadda.d` | Vector LSX/LASX (Ch5) | 0 |
| 1030 | `xvadda.h` | Vector LSX/LASX (Ch5) | 0 |
| 1031 | `xvadda.w` | Vector LSX/LASX (Ch5) | 0 |
| 1032 | `xvaddi.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1033 | `xvaddi.du` | Vector LSX/LASX (Ch5) | 0 |
| 1034 | `xvaddi.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1035 | `xvaddi.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1036 | `xvaddwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1037 | `xvaddwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1038 | `xvaddwev.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1039 | `xvaddwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1040 | `xvaddwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1041 | `xvaddwev.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 1042 | `xvaddwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1043 | `xvaddwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1044 | `xvaddwev.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 1045 | `xvaddwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1046 | `xvaddwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1047 | `xvaddwev.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1048 | `xvaddwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1049 | `xvaddwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1050 | `xvaddwod.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1051 | `xvaddwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1052 | `xvaddwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1053 | `xvaddwod.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 1054 | `xvaddwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1055 | `xvaddwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1056 | `xvaddwod.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 1057 | `xvaddwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1058 | `xvaddwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1059 | `xvaddwod.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1060 | `xvand.v` | Vector LSX/LASX (Ch5) | 0 |
| 1061 | `xvandi.b` | Vector LSX/LASX (Ch5) | 0 |
| 1062 | `xvandn.v` | Vector LSX/LASX (Ch5) | 0 |
| 1063 | `xvavg.b` | Vector LSX/LASX (Ch5) | 0 |
| 1064 | `xvavg.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1065 | `xvavg.d` | Vector LSX/LASX (Ch5) | 0 |
| 1066 | `xvavg.du` | Vector LSX/LASX (Ch5) | 0 |
| 1067 | `xvavg.h` | Vector LSX/LASX (Ch5) | 0 |
| 1068 | `xvavg.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1069 | `xvavg.w` | Vector LSX/LASX (Ch5) | 0 |
| 1070 | `xvavg.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1071 | `xvavgr.b` | Vector LSX/LASX (Ch5) | 0 |
| 1072 | `xvavgr.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1073 | `xvavgr.d` | Vector LSX/LASX (Ch5) | 0 |
| 1074 | `xvavgr.du` | Vector LSX/LASX (Ch5) | 0 |
| 1075 | `xvavgr.h` | Vector LSX/LASX (Ch5) | 0 |
| 1076 | `xvavgr.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1077 | `xvavgr.w` | Vector LSX/LASX (Ch5) | 0 |
| 1078 | `xvavgr.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1079 | `xvbitclr.b` | Vector LSX/LASX (Ch5) | 0 |
| 1080 | `xvbitclr.d` | Vector LSX/LASX (Ch5) | 0 |
| 1081 | `xvbitclr.h` | Vector LSX/LASX (Ch5) | 0 |
| 1082 | `xvbitclr.w` | Vector LSX/LASX (Ch5) | 0 |
| 1083 | `xvbitclri.b` | Vector LSX/LASX (Ch5) | 0 |
| 1084 | `xvbitclri.d` | Vector LSX/LASX (Ch5) | 0 |
| 1085 | `xvbitclri.h` | Vector LSX/LASX (Ch5) | 0 |
| 1086 | `xvbitclri.w` | Vector LSX/LASX (Ch5) | 0 |
| 1087 | `xvbitrev.b` | Vector LSX/LASX (Ch5) | 0 |
| 1088 | `xvbitrev.d` | Vector LSX/LASX (Ch5) | 0 |
| 1089 | `xvbitrev.h` | Vector LSX/LASX (Ch5) | 0 |
| 1090 | `xvbitrev.w` | Vector LSX/LASX (Ch5) | 0 |
| 1091 | `xvbitrevi.b` | Vector LSX/LASX (Ch5) | 0 |
| 1092 | `xvbitrevi.d` | Vector LSX/LASX (Ch5) | 0 |
| 1093 | `xvbitrevi.h` | Vector LSX/LASX (Ch5) | 0 |
| 1094 | `xvbitrevi.w` | Vector LSX/LASX (Ch5) | 0 |
| 1095 | `xvbitsel.v` | Vector LSX/LASX (Ch5) | 0 |
| 1096 | `xvbitseli.b` | Vector LSX/LASX (Ch5) | 0 |
| 1097 | `xvbitset.b` | Vector LSX/LASX (Ch5) | 0 |
| 1098 | `xvbitset.d` | Vector LSX/LASX (Ch5) | 0 |
| 1099 | `xvbitset.h` | Vector LSX/LASX (Ch5) | 0 |
| 1100 | `xvbitset.w` | Vector LSX/LASX (Ch5) | 0 |
| 1101 | `xvbitseti.b` | Vector LSX/LASX (Ch5) | 0 |
| 1102 | `xvbitseti.d` | Vector LSX/LASX (Ch5) | 0 |
| 1103 | `xvbitseti.h` | Vector LSX/LASX (Ch5) | 0 |
| 1104 | `xvbitseti.w` | Vector LSX/LASX (Ch5) | 0 |
| 1105 | `xvbsll.v` | Vector LSX/LASX (Ch5) | 0 |
| 1106 | `xvbsrl.v` | Vector LSX/LASX (Ch5) | 0 |
| 1107 | `xvclo.b` | Vector LSX/LASX (Ch5) | 0 |
| 1108 | `xvclo.d` | Vector LSX/LASX (Ch5) | 0 |
| 1109 | `xvclo.h` | Vector LSX/LASX (Ch5) | 0 |
| 1110 | `xvclo.w` | Vector LSX/LASX (Ch5) | 0 |
| 1111 | `xvclz.b` | Vector LSX/LASX (Ch5) | 0 |
| 1112 | `xvclz.d` | Vector LSX/LASX (Ch5) | 0 |
| 1113 | `xvclz.h` | Vector LSX/LASX (Ch5) | 0 |
| 1114 | `xvclz.w` | Vector LSX/LASX (Ch5) | 0 |
| 1115 | `xvdiv.b` | Vector LSX/LASX (Ch5) | 0 |
| 1116 | `xvdiv.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1117 | `xvdiv.d` | Vector LSX/LASX (Ch5) | 0 |
| 1118 | `xvdiv.du` | Vector LSX/LASX (Ch5) | 0 |
| 1119 | `xvdiv.h` | Vector LSX/LASX (Ch5) | 0 |
| 1120 | `xvdiv.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1121 | `xvdiv.w` | Vector LSX/LASX (Ch5) | 0 |
| 1122 | `xvdiv.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1123 | `xvexth.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1124 | `xvexth.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1125 | `xvexth.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1126 | `xvexth.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1127 | `xvexth.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1128 | `xvexth.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 1129 | `xvexth.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1130 | `xvexth.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1131 | `xvextl.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1132 | `xvextl.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 1133 | `xvextrins.b` | Vector LSX/LASX (Ch5) | 0 |
| 1134 | `xvextrins.d` | Vector LSX/LASX (Ch5) | 0 |
| 1135 | `xvextrins.h` | Vector LSX/LASX (Ch5) | 0 |
| 1136 | `xvextrins.w` | Vector LSX/LASX (Ch5) | 0 |
| 1137 | `xvfadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 1138 | `xvfadd.s` | Vector LSX/LASX (Ch5) | 0 |
| 1139 | `xvfclass.d` | Vector LSX/LASX (Ch5) | 0 |
| 1140 | `xvfclass.s` | Vector LSX/LASX (Ch5) | 0 |
| 1141 | `xvfcmp.cond.d` | Vector LSX/LASX (Ch5) | 0 |
| 1142 | `xvfcmp.cond.s` | Vector LSX/LASX (Ch5) | 0 |
| 1143 | `xvfcvt.h.s` | Vector LSX/LASX (Ch5) | 0 |
| 1144 | `xvfcvt.s.d` | Vector LSX/LASX (Ch5) | 0 |
| 1145 | `xvfcvth.d.s` | Vector LSX/LASX (Ch5) | 0 |
| 1146 | `xvfcvth.s.h` | Vector LSX/LASX (Ch5) | 0 |
| 1147 | `xvfcvtl.d.s` | Vector LSX/LASX (Ch5) | 0 |
| 1148 | `xvfcvtl.s.h` | Vector LSX/LASX (Ch5) | 0 |
| 1149 | `xvfdiv.d` | Vector LSX/LASX (Ch5) | 0 |
| 1150 | `xvfdiv.s` | Vector LSX/LASX (Ch5) | 0 |
| 1151 | `xvffint.d.l` | Vector LSX/LASX (Ch5) | 0 |
| 1152 | `xvffint.d.lu` | Vector LSX/LASX (Ch5) | 0 |
| 1153 | `xvffint.s.l` | Vector LSX/LASX (Ch5) | 0 |
| 1154 | `xvffint.s.w` | Vector LSX/LASX (Ch5) | 0 |
| 1155 | `xvffint.s.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1156 | `xvffinth.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1157 | `xvffintl.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1158 | `xvflogb.d` | Vector LSX/LASX (Ch5) | 0 |
| 1159 | `xvflogb.s` | Vector LSX/LASX (Ch5) | 0 |
| 1160 | `xvfmadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 1161 | `xvfmadd.s` | Vector LSX/LASX (Ch5) | 0 |
| 1162 | `xvfmax.d` | Vector LSX/LASX (Ch5) | 0 |
| 1163 | `xvfmax.s` | Vector LSX/LASX (Ch5) | 0 |
| 1164 | `xvfmaxa.d` | Vector LSX/LASX (Ch5) | 0 |
| 1165 | `xvfmaxa.s` | Vector LSX/LASX (Ch5) | 0 |
| 1166 | `xvfmin.d` | Vector LSX/LASX (Ch5) | 0 |
| 1167 | `xvfmin.s` | Vector LSX/LASX (Ch5) | 0 |
| 1168 | `xvfmina.d` | Vector LSX/LASX (Ch5) | 0 |
| 1169 | `xvfmina.s` | Vector LSX/LASX (Ch5) | 0 |
| 1170 | `xvfmsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 1171 | `xvfmsub.s` | Vector LSX/LASX (Ch5) | 0 |
| 1172 | `xvfmul.d` | Vector LSX/LASX (Ch5) | 0 |
| 1173 | `xvfmul.s` | Vector LSX/LASX (Ch5) | 0 |
| 1174 | `xvfnmadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 1175 | `xvfnmadd.s` | Vector LSX/LASX (Ch5) | 0 |
| 1176 | `xvfnmsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 1177 | `xvfnmsub.s` | Vector LSX/LASX (Ch5) | 0 |
| 1178 | `xvfrecip.d` | Vector LSX/LASX (Ch5) | 0 |
| 1179 | `xvfrecip.s` | Vector LSX/LASX (Ch5) | 0 |
| 1180 | `xvfrint.d` | Vector LSX/LASX (Ch5) | 0 |
| 1181 | `xvfrint.s` | Vector LSX/LASX (Ch5) | 0 |
| 1182 | `xvfrintrm.d` | Vector LSX/LASX (Ch5) | 0 |
| 1183 | `xvfrintrm.s` | Vector LSX/LASX (Ch5) | 0 |
| 1184 | `xvfrintrne.d` | Vector LSX/LASX (Ch5) | 0 |
| 1185 | `xvfrintrne.s` | Vector LSX/LASX (Ch5) | 0 |
| 1186 | `xvfrintrp.d` | Vector LSX/LASX (Ch5) | 0 |
| 1187 | `xvfrintrp.s` | Vector LSX/LASX (Ch5) | 0 |
| 1188 | `xvfrintrz.d` | Vector LSX/LASX (Ch5) | 0 |
| 1189 | `xvfrintrz.s` | Vector LSX/LASX (Ch5) | 0 |
| 1190 | `xvfrsqrt.d` | Vector LSX/LASX (Ch5) | 0 |
| 1191 | `xvfrsqrt.s` | Vector LSX/LASX (Ch5) | 0 |
| 1192 | `xvfrstp.b` | Vector LSX/LASX (Ch5) | 0 |
| 1193 | `xvfrstp.h` | Vector LSX/LASX (Ch5) | 0 |
| 1194 | `xvfrstpi.b` | Vector LSX/LASX (Ch5) | 0 |
| 1195 | `xvfrstpi.h` | Vector LSX/LASX (Ch5) | 0 |
| 1196 | `xvfsqrt.d` | Vector LSX/LASX (Ch5) | 0 |
| 1197 | `xvfsqrt.s` | Vector LSX/LASX (Ch5) | 0 |
| 1198 | `xvfsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 1199 | `xvfsub.s` | Vector LSX/LASX (Ch5) | 0 |
| 1200 | `xvftint.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 1201 | `xvftint.lu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1202 | `xvftint.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1203 | `xvftint.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 1204 | `xvftint.wu.s` | Vector LSX/LASX (Ch5) | 0 |
| 1205 | `xvftinth.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1206 | `xvftintl.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1207 | `xvftintrm.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 1208 | `xvftintrm.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1209 | `xvftintrm.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 1210 | `xvftintrmh.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1211 | `xvftintrml.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1212 | `xvftintrne.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 1213 | `xvftintrne.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1214 | `xvftintrne.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 1215 | `xvftintrneh.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1216 | `xvftintrnel.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1217 | `xvftintrp.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 1218 | `xvftintrp.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1219 | `xvftintrp.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 1220 | `xvftintrph.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1221 | `xvftintrpl.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1222 | `xvftintrz.l.d` | Vector LSX/LASX (Ch5) | 0 |
| 1223 | `xvftintrz.lu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1224 | `xvftintrz.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1225 | `xvftintrz.w.s` | Vector LSX/LASX (Ch5) | 0 |
| 1226 | `xvftintrz.wu.s` | Vector LSX/LASX (Ch5) | 0 |
| 1227 | `xvftintrzh.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1228 | `xvftintrzl.l.s` | Vector LSX/LASX (Ch5) | 0 |
| 1229 | `xvhaddw.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1230 | `xvhaddw.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1231 | `xvhaddw.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1232 | `xvhaddw.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1233 | `xvhaddw.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1234 | `xvhaddw.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 1235 | `xvhaddw.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1236 | `xvhaddw.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1237 | `xvhsubw.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1238 | `xvhsubw.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1239 | `xvhsubw.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1240 | `xvhsubw.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1241 | `xvhsubw.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1242 | `xvhsubw.qu.du` | Vector LSX/LASX (Ch5) | 0 |
| 1243 | `xvhsubw.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1244 | `xvhsubw.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1245 | `xvilvh.b` | Vector LSX/LASX (Ch5) | 0 |
| 1246 | `xvilvh.d` | Vector LSX/LASX (Ch5) | 0 |
| 1247 | `xvilvh.h` | Vector LSX/LASX (Ch5) | 0 |
| 1248 | `xvilvh.w` | Vector LSX/LASX (Ch5) | 0 |
| 1249 | `xvilvl.b` | Vector LSX/LASX (Ch5) | 0 |
| 1250 | `xvilvl.d` | Vector LSX/LASX (Ch5) | 0 |
| 1251 | `xvilvl.h` | Vector LSX/LASX (Ch5) | 0 |
| 1252 | `xvilvl.w` | Vector LSX/LASX (Ch5) | 0 |
| 1253 | `xvinsgr2vr.d` | Vector LSX/LASX (Ch5) | 0 |
| 1254 | `xvinsgr2vr.w` | Vector LSX/LASX (Ch5) | 0 |
| 1255 | `xvinsve0.d` | Vector LSX/LASX (Ch5) | 0 |
| 1256 | `xvinsve0.w` | Vector LSX/LASX (Ch5) | 0 |
| 1257 | `xvld` | Vector LSX/LASX (Ch5) | 0 |
| 1258 | `xvldi` | Vector LSX/LASX (Ch5) | 0 |
| 1259 | `xvldrepl.b` | Vector LSX/LASX (Ch5) | 0 |
| 1260 | `xvldrepl.d` | Vector LSX/LASX (Ch5) | 0 |
| 1261 | `xvldrepl.h` | Vector LSX/LASX (Ch5) | 0 |
| 1262 | `xvldrepl.w` | Vector LSX/LASX (Ch5) | 0 |
| 1263 | `xvldx` | Vector LSX/LASX (Ch5) | 0 |
| 1264 | `xvmadd.b` | Vector LSX/LASX (Ch5) | 0 |
| 1265 | `xvmadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 1266 | `xvmadd.h` | Vector LSX/LASX (Ch5) | 0 |
| 1267 | `xvmadd.w` | Vector LSX/LASX (Ch5) | 0 |
| 1268 | `xvmaddwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1269 | `xvmaddwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1270 | `xvmaddwev.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1271 | `xvmaddwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1272 | `xvmaddwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1273 | `xvmaddwev.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 1274 | `xvmaddwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1275 | `xvmaddwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1276 | `xvmaddwev.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 1277 | `xvmaddwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1278 | `xvmaddwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1279 | `xvmaddwev.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1280 | `xvmaddwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1281 | `xvmaddwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1282 | `xvmaddwod.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1283 | `xvmaddwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1284 | `xvmaddwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1285 | `xvmaddwod.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 1286 | `xvmaddwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1287 | `xvmaddwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1288 | `xvmaddwod.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 1289 | `xvmaddwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1290 | `xvmaddwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1291 | `xvmaddwod.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1292 | `xvmax.b` | Vector LSX/LASX (Ch5) | 0 |
| 1293 | `xvmax.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1294 | `xvmax.d` | Vector LSX/LASX (Ch5) | 0 |
| 1295 | `xvmax.du` | Vector LSX/LASX (Ch5) | 0 |
| 1296 | `xvmax.h` | Vector LSX/LASX (Ch5) | 0 |
| 1297 | `xvmax.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1298 | `xvmax.w` | Vector LSX/LASX (Ch5) | 0 |
| 1299 | `xvmax.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1300 | `xvmaxi.b` | Vector LSX/LASX (Ch5) | 0 |
| 1301 | `xvmaxi.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1302 | `xvmaxi.d` | Vector LSX/LASX (Ch5) | 0 |
| 1303 | `xvmaxi.du` | Vector LSX/LASX (Ch5) | 0 |
| 1304 | `xvmaxi.h` | Vector LSX/LASX (Ch5) | 0 |
| 1305 | `xvmaxi.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1306 | `xvmaxi.w` | Vector LSX/LASX (Ch5) | 0 |
| 1307 | `xvmaxi.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1308 | `xvmin.b` | Vector LSX/LASX (Ch5) | 0 |
| 1309 | `xvmin.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1310 | `xvmin.d` | Vector LSX/LASX (Ch5) | 0 |
| 1311 | `xvmin.du` | Vector LSX/LASX (Ch5) | 0 |
| 1312 | `xvmin.h` | Vector LSX/LASX (Ch5) | 0 |
| 1313 | `xvmin.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1314 | `xvmin.w` | Vector LSX/LASX (Ch5) | 0 |
| 1315 | `xvmin.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1316 | `xvmini.b` | Vector LSX/LASX (Ch5) | 0 |
| 1317 | `xvmini.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1318 | `xvmini.d` | Vector LSX/LASX (Ch5) | 0 |
| 1319 | `xvmini.du` | Vector LSX/LASX (Ch5) | 0 |
| 1320 | `xvmini.h` | Vector LSX/LASX (Ch5) | 0 |
| 1321 | `xvmini.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1322 | `xvmini.w` | Vector LSX/LASX (Ch5) | 0 |
| 1323 | `xvmini.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1324 | `xvmod.b` | Vector LSX/LASX (Ch5) | 0 |
| 1325 | `xvmod.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1326 | `xvmod.d` | Vector LSX/LASX (Ch5) | 0 |
| 1327 | `xvmod.du` | Vector LSX/LASX (Ch5) | 0 |
| 1328 | `xvmod.h` | Vector LSX/LASX (Ch5) | 0 |
| 1329 | `xvmod.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1330 | `xvmod.w` | Vector LSX/LASX (Ch5) | 0 |
| 1331 | `xvmod.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1332 | `xvmskgez.b` | Vector LSX/LASX (Ch5) | 0 |
| 1333 | `xvmskltz.b` | Vector LSX/LASX (Ch5) | 0 |
| 1334 | `xvmskltz.d` | Vector LSX/LASX (Ch5) | 0 |
| 1335 | `xvmskltz.h` | Vector LSX/LASX (Ch5) | 0 |
| 1336 | `xvmskltz.w` | Vector LSX/LASX (Ch5) | 0 |
| 1337 | `xvmsknz.b` | Vector LSX/LASX (Ch5) | 0 |
| 1338 | `xvmsub.b` | Vector LSX/LASX (Ch5) | 0 |
| 1339 | `xvmsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 1340 | `xvmsub.h` | Vector LSX/LASX (Ch5) | 0 |
| 1341 | `xvmsub.w` | Vector LSX/LASX (Ch5) | 0 |
| 1342 | `xvmuh.b` | Vector LSX/LASX (Ch5) | 0 |
| 1343 | `xvmuh.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1344 | `xvmuh.d` | Vector LSX/LASX (Ch5) | 0 |
| 1345 | `xvmuh.du` | Vector LSX/LASX (Ch5) | 0 |
| 1346 | `xvmuh.h` | Vector LSX/LASX (Ch5) | 0 |
| 1347 | `xvmuh.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1348 | `xvmuh.w` | Vector LSX/LASX (Ch5) | 0 |
| 1349 | `xvmuh.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1350 | `xvmul.b` | Vector LSX/LASX (Ch5) | 0 |
| 1351 | `xvmul.d` | Vector LSX/LASX (Ch5) | 0 |
| 1352 | `xvmul.h` | Vector LSX/LASX (Ch5) | 0 |
| 1353 | `xvmul.w` | Vector LSX/LASX (Ch5) | 0 |
| 1354 | `xvmulwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1355 | `xvmulwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1356 | `xvmulwev.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1357 | `xvmulwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1358 | `xvmulwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1359 | `xvmulwev.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 1360 | `xvmulwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1361 | `xvmulwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1362 | `xvmulwev.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 1363 | `xvmulwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1364 | `xvmulwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1365 | `xvmulwev.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1366 | `xvmulwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1367 | `xvmulwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1368 | `xvmulwod.d.wu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1369 | `xvmulwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1370 | `xvmulwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1371 | `xvmulwod.h.bu.b` | Vector LSX/LASX (Ch5) | 0 |
| 1372 | `xvmulwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1373 | `xvmulwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1374 | `xvmulwod.q.du.d` | Vector LSX/LASX (Ch5) | 0 |
| 1375 | `xvmulwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1376 | `xvmulwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1377 | `xvmulwod.w.hu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1378 | `xvneg.b` | Vector LSX/LASX (Ch5) | 0 |
| 1379 | `xvneg.d` | Vector LSX/LASX (Ch5) | 0 |
| 1380 | `xvneg.h` | Vector LSX/LASX (Ch5) | 0 |
| 1381 | `xvneg.w` | Vector LSX/LASX (Ch5) | 0 |
| 1382 | `xvnor.v` | Vector LSX/LASX (Ch5) | 0 |
| 1383 | `xvnori.b` | Vector LSX/LASX (Ch5) | 0 |
| 1384 | `xvor.v` | Vector LSX/LASX (Ch5) | 0 |
| 1385 | `xvori.b` | Vector LSX/LASX (Ch5) | 0 |
| 1386 | `xvorn.v` | Vector LSX/LASX (Ch5) | 0 |
| 1387 | `xvpackev.b` | Vector LSX/LASX (Ch5) | 0 |
| 1388 | `xvpackev.d` | Vector LSX/LASX (Ch5) | 0 |
| 1389 | `xvpackev.h` | Vector LSX/LASX (Ch5) | 0 |
| 1390 | `xvpackev.w` | Vector LSX/LASX (Ch5) | 0 |
| 1391 | `xvpackod.b` | Vector LSX/LASX (Ch5) | 0 |
| 1392 | `xvpackod.d` | Vector LSX/LASX (Ch5) | 0 |
| 1393 | `xvpackod.h` | Vector LSX/LASX (Ch5) | 0 |
| 1394 | `xvpackod.w` | Vector LSX/LASX (Ch5) | 0 |
| 1395 | `xvpcnt.b` | Vector LSX/LASX (Ch5) | 0 |
| 1396 | `xvpcnt.d` | Vector LSX/LASX (Ch5) | 0 |
| 1397 | `xvpcnt.h` | Vector LSX/LASX (Ch5) | 0 |
| 1398 | `xvpcnt.w` | Vector LSX/LASX (Ch5) | 0 |
| 1399 | `xvperm.w` | Vector LSX/LASX (Ch5) | 0 |
| 1400 | `xvpermi.d` | Vector LSX/LASX (Ch5) | 0 |
| 1401 | `xvpermi.q` | Vector LSX/LASX (Ch5) | 0 |
| 1402 | `xvpermi.w` | Vector LSX/LASX (Ch5) | 0 |
| 1403 | `xvpickev.b` | Vector LSX/LASX (Ch5) | 0 |
| 1404 | `xvpickev.d` | Vector LSX/LASX (Ch5) | 0 |
| 1405 | `xvpickev.h` | Vector LSX/LASX (Ch5) | 0 |
| 1406 | `xvpickev.w` | Vector LSX/LASX (Ch5) | 0 |
| 1407 | `xvpickod.b` | Vector LSX/LASX (Ch5) | 0 |
| 1408 | `xvpickod.d` | Vector LSX/LASX (Ch5) | 0 |
| 1409 | `xvpickod.h` | Vector LSX/LASX (Ch5) | 0 |
| 1410 | `xvpickod.w` | Vector LSX/LASX (Ch5) | 0 |
| 1411 | `xvpickve2gr.d` | Vector LSX/LASX (Ch5) | 0 |
| 1412 | `xvpickve2gr.du` | Vector LSX/LASX (Ch5) | 0 |
| 1413 | `xvpickve2gr.w` | Vector LSX/LASX (Ch5) | 0 |
| 1414 | `xvpickve2gr.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1415 | `xvpickve.d` | Vector LSX/LASX (Ch5) | 0 |
| 1416 | `xvpickve.w` | Vector LSX/LASX (Ch5) | 0 |
| 1417 | `xvrepl128vei.b` | Vector LSX/LASX (Ch5) | 0 |
| 1418 | `xvrepl128vei.d` | Vector LSX/LASX (Ch5) | 0 |
| 1419 | `xvrepl128vei.h` | Vector LSX/LASX (Ch5) | 0 |
| 1420 | `xvrepl128vei.w` | Vector LSX/LASX (Ch5) | 0 |
| 1421 | `xvreplgr2vr.b` | Vector LSX/LASX (Ch5) | 0 |
| 1422 | `xvreplgr2vr.d` | Vector LSX/LASX (Ch5) | 0 |
| 1423 | `xvreplgr2vr.h` | Vector LSX/LASX (Ch5) | 0 |
| 1424 | `xvreplgr2vr.w` | Vector LSX/LASX (Ch5) | 0 |
| 1425 | `xvreplve0.b` | Vector LSX/LASX (Ch5) | 0 |
| 1426 | `xvreplve0.d` | Vector LSX/LASX (Ch5) | 0 |
| 1427 | `xvreplve0.h` | Vector LSX/LASX (Ch5) | 0 |
| 1428 | `xvreplve0.q` | Vector LSX/LASX (Ch5) | 0 |
| 1429 | `xvreplve0.w` | Vector LSX/LASX (Ch5) | 0 |
| 1430 | `xvreplve.b` | Vector LSX/LASX (Ch5) | 0 |
| 1431 | `xvreplve.d` | Vector LSX/LASX (Ch5) | 0 |
| 1432 | `xvreplve.h` | Vector LSX/LASX (Ch5) | 0 |
| 1433 | `xvreplve.w` | Vector LSX/LASX (Ch5) | 0 |
| 1434 | `xvrotr.b` | Vector LSX/LASX (Ch5) | 0 |
| 1435 | `xvrotr.d` | Vector LSX/LASX (Ch5) | 0 |
| 1436 | `xvrotr.h` | Vector LSX/LASX (Ch5) | 0 |
| 1437 | `xvrotr.w` | Vector LSX/LASX (Ch5) | 0 |
| 1438 | `xvrotri.b` | Vector LSX/LASX (Ch5) | 0 |
| 1439 | `xvrotri.d` | Vector LSX/LASX (Ch5) | 0 |
| 1440 | `xvrotri.h` | Vector LSX/LASX (Ch5) | 0 |
| 1441 | `xvrotri.w` | Vector LSX/LASX (Ch5) | 0 |
| 1442 | `xvsadd.b` | Vector LSX/LASX (Ch5) | 0 |
| 1443 | `xvsadd.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1444 | `xvsadd.d` | Vector LSX/LASX (Ch5) | 0 |
| 1445 | `xvsadd.du` | Vector LSX/LASX (Ch5) | 0 |
| 1446 | `xvsadd.h` | Vector LSX/LASX (Ch5) | 0 |
| 1447 | `xvsadd.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1448 | `xvsadd.w` | Vector LSX/LASX (Ch5) | 0 |
| 1449 | `xvsadd.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1450 | `xvsat.b` | Vector LSX/LASX (Ch5) | 0 |
| 1451 | `xvsat.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1452 | `xvsat.d` | Vector LSX/LASX (Ch5) | 0 |
| 1453 | `xvsat.du` | Vector LSX/LASX (Ch5) | 0 |
| 1454 | `xvsat.h` | Vector LSX/LASX (Ch5) | 0 |
| 1455 | `xvsat.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1456 | `xvsat.w` | Vector LSX/LASX (Ch5) | 0 |
| 1457 | `xvsat.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1458 | `xvseq.b` | Vector LSX/LASX (Ch5) | 0 |
| 1459 | `xvseq.d` | Vector LSX/LASX (Ch5) | 0 |
| 1460 | `xvseq.h` | Vector LSX/LASX (Ch5) | 0 |
| 1461 | `xvseq.w` | Vector LSX/LASX (Ch5) | 0 |
| 1462 | `xvseqi.b` | Vector LSX/LASX (Ch5) | 0 |
| 1463 | `xvseqi.d` | Vector LSX/LASX (Ch5) | 0 |
| 1464 | `xvseqi.h` | Vector LSX/LASX (Ch5) | 0 |
| 1465 | `xvseqi.w` | Vector LSX/LASX (Ch5) | 0 |
| 1466 | `xvsetallnez.b` | Vector LSX/LASX (Ch5) | 0 |
| 1467 | `xvsetallnez.d` | Vector LSX/LASX (Ch5) | 0 |
| 1468 | `xvsetallnez.h` | Vector LSX/LASX (Ch5) | 0 |
| 1469 | `xvsetallnez.w` | Vector LSX/LASX (Ch5) | 0 |
| 1470 | `xvsetanyeqz.b` | Vector LSX/LASX (Ch5) | 0 |
| 1471 | `xvsetanyeqz.d` | Vector LSX/LASX (Ch5) | 0 |
| 1472 | `xvsetanyeqz.h` | Vector LSX/LASX (Ch5) | 0 |
| 1473 | `xvsetanyeqz.w` | Vector LSX/LASX (Ch5) | 0 |
| 1474 | `xvseteqz.v` | Vector LSX/LASX (Ch5) | 0 |
| 1475 | `xvsetnez.v` | Vector LSX/LASX (Ch5) | 0 |
| 1476 | `xvshuf4i.b` | Vector LSX/LASX (Ch5) | 0 |
| 1477 | `xvshuf4i.d` | Vector LSX/LASX (Ch5) | 0 |
| 1478 | `xvshuf4i.h` | Vector LSX/LASX (Ch5) | 0 |
| 1479 | `xvshuf4i.w` | Vector LSX/LASX (Ch5) | 0 |
| 1480 | `xvshuf.b` | Vector LSX/LASX (Ch5) | 0 |
| 1481 | `xvshuf.d` | Vector LSX/LASX (Ch5) | 0 |
| 1482 | `xvshuf.h` | Vector LSX/LASX (Ch5) | 0 |
| 1483 | `xvshuf.w` | Vector LSX/LASX (Ch5) | 0 |
| 1484 | `xvsigncov.b` | Vector LSX/LASX (Ch5) | 0 |
| 1485 | `xvsigncov.d` | Vector LSX/LASX (Ch5) | 0 |
| 1486 | `xvsigncov.h` | Vector LSX/LASX (Ch5) | 0 |
| 1487 | `xvsigncov.w` | Vector LSX/LASX (Ch5) | 0 |
| 1488 | `xvsle.b` | Vector LSX/LASX (Ch5) | 0 |
| 1489 | `xvsle.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1490 | `xvsle.d` | Vector LSX/LASX (Ch5) | 0 |
| 1491 | `xvsle.du` | Vector LSX/LASX (Ch5) | 0 |
| 1492 | `xvsle.h` | Vector LSX/LASX (Ch5) | 0 |
| 1493 | `xvsle.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1494 | `xvsle.w` | Vector LSX/LASX (Ch5) | 0 |
| 1495 | `xvsle.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1496 | `xvslei.b` | Vector LSX/LASX (Ch5) | 0 |
| 1497 | `xvslei.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1498 | `xvslei.d` | Vector LSX/LASX (Ch5) | 0 |
| 1499 | `xvslei.du` | Vector LSX/LASX (Ch5) | 0 |
| 1500 | `xvslei.h` | Vector LSX/LASX (Ch5) | 0 |
| 1501 | `xvslei.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1502 | `xvslei.w` | Vector LSX/LASX (Ch5) | 0 |
| 1503 | `xvslei.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1504 | `xvsll.b` | Vector LSX/LASX (Ch5) | 0 |
| 1505 | `xvsll.d` | Vector LSX/LASX (Ch5) | 0 |
| 1506 | `xvsll.h` | Vector LSX/LASX (Ch5) | 0 |
| 1507 | `xvsll.w` | Vector LSX/LASX (Ch5) | 0 |
| 1508 | `xvslli.b` | Vector LSX/LASX (Ch5) | 0 |
| 1509 | `xvslli.d` | Vector LSX/LASX (Ch5) | 0 |
| 1510 | `xvslli.h` | Vector LSX/LASX (Ch5) | 0 |
| 1511 | `xvslli.w` | Vector LSX/LASX (Ch5) | 0 |
| 1512 | `xvsllwil.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1513 | `xvsllwil.du.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1514 | `xvsllwil.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1515 | `xvsllwil.hu.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1516 | `xvsllwil.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1517 | `xvsllwil.wu.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1518 | `xvslt.b` | Vector LSX/LASX (Ch5) | 0 |
| 1519 | `xvslt.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1520 | `xvslt.d` | Vector LSX/LASX (Ch5) | 0 |
| 1521 | `xvslt.du` | Vector LSX/LASX (Ch5) | 0 |
| 1522 | `xvslt.h` | Vector LSX/LASX (Ch5) | 0 |
| 1523 | `xvslt.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1524 | `xvslt.w` | Vector LSX/LASX (Ch5) | 0 |
| 1525 | `xvslt.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1526 | `xvslti.b` | Vector LSX/LASX (Ch5) | 0 |
| 1527 | `xvslti.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1528 | `xvslti.d` | Vector LSX/LASX (Ch5) | 0 |
| 1529 | `xvslti.du` | Vector LSX/LASX (Ch5) | 0 |
| 1530 | `xvslti.h` | Vector LSX/LASX (Ch5) | 0 |
| 1531 | `xvslti.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1532 | `xvslti.w` | Vector LSX/LASX (Ch5) | 0 |
| 1533 | `xvslti.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1534 | `xvsra.b` | Vector LSX/LASX (Ch5) | 0 |
| 1535 | `xvsra.d` | Vector LSX/LASX (Ch5) | 0 |
| 1536 | `xvsra.h` | Vector LSX/LASX (Ch5) | 0 |
| 1537 | `xvsra.w` | Vector LSX/LASX (Ch5) | 0 |
| 1538 | `xvsrai.b` | Vector LSX/LASX (Ch5) | 0 |
| 1539 | `xvsrai.d` | Vector LSX/LASX (Ch5) | 0 |
| 1540 | `xvsrai.h` | Vector LSX/LASX (Ch5) | 0 |
| 1541 | `xvsrai.w` | Vector LSX/LASX (Ch5) | 0 |
| 1542 | `xvsran.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1543 | `xvsran.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1544 | `xvsran.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1545 | `xvsrani.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1546 | `xvsrani.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1547 | `xvsrani.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1548 | `xvsrani.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1549 | `xvsrar.b` | Vector LSX/LASX (Ch5) | 0 |
| 1550 | `xvsrar.d` | Vector LSX/LASX (Ch5) | 0 |
| 1551 | `xvsrar.h` | Vector LSX/LASX (Ch5) | 0 |
| 1552 | `xvsrar.w` | Vector LSX/LASX (Ch5) | 0 |
| 1553 | `xvsrari.b` | Vector LSX/LASX (Ch5) | 0 |
| 1554 | `xvsrari.d` | Vector LSX/LASX (Ch5) | 0 |
| 1555 | `xvsrari.h` | Vector LSX/LASX (Ch5) | 0 |
| 1556 | `xvsrari.w` | Vector LSX/LASX (Ch5) | 0 |
| 1557 | `xvsrarn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1558 | `xvsrarn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1559 | `xvsrarn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1560 | `xvsrarni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1561 | `xvsrarni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1562 | `xvsrarni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1563 | `xvsrarni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1564 | `xvsrl.b` | Vector LSX/LASX (Ch5) | 0 |
| 1565 | `xvsrl.d` | Vector LSX/LASX (Ch5) | 0 |
| 1566 | `xvsrl.h` | Vector LSX/LASX (Ch5) | 0 |
| 1567 | `xvsrl.w` | Vector LSX/LASX (Ch5) | 0 |
| 1568 | `xvsrli.b` | Vector LSX/LASX (Ch5) | 0 |
| 1569 | `xvsrli.d` | Vector LSX/LASX (Ch5) | 0 |
| 1570 | `xvsrli.h` | Vector LSX/LASX (Ch5) | 0 |
| 1571 | `xvsrli.w` | Vector LSX/LASX (Ch5) | 0 |
| 1572 | `xvsrln.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1573 | `xvsrln.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1574 | `xvsrln.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1575 | `xvsrlni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1576 | `xvsrlni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1577 | `xvsrlni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1578 | `xvsrlni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1579 | `xvsrlr.b` | Vector LSX/LASX (Ch5) | 0 |
| 1580 | `xvsrlr.d` | Vector LSX/LASX (Ch5) | 0 |
| 1581 | `xvsrlr.h` | Vector LSX/LASX (Ch5) | 0 |
| 1582 | `xvsrlr.w` | Vector LSX/LASX (Ch5) | 0 |
| 1583 | `xvsrlri.b` | Vector LSX/LASX (Ch5) | 0 |
| 1584 | `xvsrlri.d` | Vector LSX/LASX (Ch5) | 0 |
| 1585 | `xvsrlri.h` | Vector LSX/LASX (Ch5) | 0 |
| 1586 | `xvsrlri.w` | Vector LSX/LASX (Ch5) | 0 |
| 1587 | `xvsrlrn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1588 | `xvsrlrn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1589 | `xvsrlrn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1590 | `xvsrlrni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1591 | `xvsrlrni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1592 | `xvsrlrni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1593 | `xvsrlrni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1594 | `xvssran.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1595 | `xvssran.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1596 | `xvssran.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1597 | `xvssran.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1598 | `xvssran.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1599 | `xvssran.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1600 | `xvssrani.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1601 | `xvssrani.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1602 | `xvssrani.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1603 | `xvssrani.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 1604 | `xvssrani.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1605 | `xvssrani.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1606 | `xvssrani.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1607 | `xvssrani.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1608 | `xvssrarn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1609 | `xvssrarn.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1610 | `xvssrarn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1611 | `xvssrarn.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1612 | `xvssrarn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1613 | `xvssrarn.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1614 | `xvssrarni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1615 | `xvssrarni.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1616 | `xvssrarni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1617 | `xvssrarni.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 1618 | `xvssrarni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1619 | `xvssrarni.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1620 | `xvssrarni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1621 | `xvssrarni.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1622 | `xvssrln.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1623 | `xvssrln.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1624 | `xvssrln.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1625 | `xvssrln.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1626 | `xvssrln.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1627 | `xvssrln.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1628 | `xvssrlni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1629 | `xvssrlni.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1630 | `xvssrlni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1631 | `xvssrlni.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 1632 | `xvssrlni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1633 | `xvssrlni.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1634 | `xvssrlni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1635 | `xvssrlni.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1636 | `xvssrlrn.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1637 | `xvssrlrn.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1638 | `xvssrlrn.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1639 | `xvssrlrn.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1640 | `xvssrlrn.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1641 | `xvssrlrn.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1642 | `xvssrlrni.b.h` | Vector LSX/LASX (Ch5) | 0 |
| 1643 | `xvssrlrni.bu.h` | Vector LSX/LASX (Ch5) | 0 |
| 1644 | `xvssrlrni.d.q` | Vector LSX/LASX (Ch5) | 0 |
| 1645 | `xvssrlrni.du.q` | Vector LSX/LASX (Ch5) | 0 |
| 1646 | `xvssrlrni.h.w` | Vector LSX/LASX (Ch5) | 0 |
| 1647 | `xvssrlrni.hu.w` | Vector LSX/LASX (Ch5) | 0 |
| 1648 | `xvssrlrni.w.d` | Vector LSX/LASX (Ch5) | 0 |
| 1649 | `xvssrlrni.wu.d` | Vector LSX/LASX (Ch5) | 0 |
| 1650 | `xvssub.b` | Vector LSX/LASX (Ch5) | 0 |
| 1651 | `xvssub.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1652 | `xvssub.d` | Vector LSX/LASX (Ch5) | 0 |
| 1653 | `xvssub.du` | Vector LSX/LASX (Ch5) | 0 |
| 1654 | `xvssub.h` | Vector LSX/LASX (Ch5) | 0 |
| 1655 | `xvssub.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1656 | `xvssub.w` | Vector LSX/LASX (Ch5) | 0 |
| 1657 | `xvssub.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1658 | `xvst` | Vector LSX/LASX (Ch5) | 0 |
| 1659 | `xvstelm.b` | Vector LSX/LASX (Ch5) | 0 |
| 1660 | `xvstelm.d` | Vector LSX/LASX (Ch5) | 0 |
| 1661 | `xvstelm.h` | Vector LSX/LASX (Ch5) | 0 |
| 1662 | `xvstelm.w` | Vector LSX/LASX (Ch5) | 0 |
| 1663 | `xvstx` | Vector LSX/LASX (Ch5) | 0 |
| 1664 | `xvsub.b` | Vector LSX/LASX (Ch5) | 0 |
| 1665 | `xvsub.d` | Vector LSX/LASX (Ch5) | 0 |
| 1666 | `xvsub.h` | Vector LSX/LASX (Ch5) | 0 |
| 1667 | `xvsub.q` | Vector LSX/LASX (Ch5) | 0 |
| 1668 | `xvsub.w` | Vector LSX/LASX (Ch5) | 0 |
| 1669 | `xvsubi.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1670 | `xvsubi.du` | Vector LSX/LASX (Ch5) | 0 |
| 1671 | `xvsubi.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1672 | `xvsubi.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1673 | `xvsubwev.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1674 | `xvsubwev.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1675 | `xvsubwev.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1676 | `xvsubwev.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1677 | `xvsubwev.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1678 | `xvsubwev.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1679 | `xvsubwev.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1680 | `xvsubwev.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1681 | `xvsubwod.d.w` | Vector LSX/LASX (Ch5) | 0 |
| 1682 | `xvsubwod.d.wu` | Vector LSX/LASX (Ch5) | 0 |
| 1683 | `xvsubwod.h.b` | Vector LSX/LASX (Ch5) | 0 |
| 1684 | `xvsubwod.h.bu` | Vector LSX/LASX (Ch5) | 0 |
| 1685 | `xvsubwod.q.d` | Vector LSX/LASX (Ch5) | 0 |
| 1686 | `xvsubwod.q.du` | Vector LSX/LASX (Ch5) | 0 |
| 1687 | `xvsubwod.w.h` | Vector LSX/LASX (Ch5) | 0 |
| 1688 | `xvsubwod.w.hu` | Vector LSX/LASX (Ch5) | 0 |
| 1689 | `xvxor.v` | Vector LSX/LASX (Ch5) | 0 |
| 1690 | `xvxori.b` | Vector LSX/LASX (Ch5) | 0 |
