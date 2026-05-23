#!/usr/bin/env python3
"""Generate missing vector instruction implementations from QEMU's trans_vec.c.inc."""
import re, sys

QEMU_TRANS = "/home/airxs/user/qemu/qemu-10.1.2/target/loongarch/tcg/insn_trans/trans_vec.c.inc"
INTERPRETER = "src/core/interpreter.c"
OUTPUT = "/tmp/vec_gen.c"

with open(QEMU_TRANS) as f:
    text = f.read()

entries = []
for m in re.finditer(r'TRANS\((\w+),\s*(LSX|LASX),\s*(\w+)(?:,\s*(.+?))?\s*\)', text):
    name, isa, gen_str, args = m.group(1), m.group(2), m.group(3), m.group(4) or ''
    parts = [p.strip() for p in args.split(',')]
    if gen_str.startswith('gvec_'):
        gtype = gen_str.replace('gvec_', '')
        tcg_op = parts[1] if len(parts) > 1 else ''
        esize = parts[0] if parts else ''
    else:
        gtype = gen_str.replace('gen_', '')
        tcg_op = parts[0] if parts else ''
        esize = ''
    entries.append({'name': name, 'isa': isa, 'gtype': gtype, 'tcg_op': tcg_op})

with open(INTERPRETER) as f:
    our_text = f.read()

existing = set(re.findall(r'^(?:static )?bool (trans_[vx]\w+)\(', our_text, re.MULTILINE))
missing = [e for e in entries if 'trans_' + e['name'] not in existing]
print(f"Total missing: {len(missing)}")

def elem_type(name):
    for s in ('_bu','_b'):
        if name.endswith(s): return ('B', 1, 'uint8_t' if s=='_bu' else 'int8_t')
    for s in ('_hu','_h'):
        if name.endswith(s): return ('H', 2, 'uint16_t' if s=='_hu' else 'int16_t')
    for s in ('_wu','_w'):
        if name.endswith(s): return ('W', 4, 'uint32_t' if s=='_wu' else 'int32_t')
    for s in ('_du','_d'):
        if name.endswith(s): return ('D', 8, 'uint64_t' if s=='_du' else 'int64_t')
    for s in ('_qu','_q'):
        if name.endswith(s): return ('Q', 16, 'Int128')
    return (None, 4, 'int32_t')

def signed_type(name):
    _, _, ct = elem_type(name)
    if ct.startswith('uint'):
        return ct
    return ct

def unsigned_type(name):
    _, _, ct = elem_type(name)
    if ct.startswith('uint'):
        return ct
    return 'u' + ct

def get_op(helper):
    if not helper: return None
    h = helper.lower()
    if 'ssadd' in h or 'usadd' in h or ('add' in h and 'sub' not in h): return 'ADD'
    if 'sssub' in h or 'ussub' in h or ('sub' in h and 'add' not in h): return 'SUB'
    if 'smax' in h: return 'SMAX'
    if 'smin' in h: return 'SMIN'
    if 'umax' in h: return 'UMAX'
    if 'umin' in h: return 'UMIN'
    if 'mul' in h: return 'MUL'
    if 'neg' in h: return 'NEG'
    if 'and' in h and 'nand' not in h: return 'AND'
    if 'or' in h: return 'OR'
    if 'xor' in h: return 'XOR'
    return None

lines = []
lines.append("/* === AUTO-GENERATED vector implementations === */")
lines.append("")

for e in missing:
    name = e['name']
    isa = e['isa']
    gtype = e['gtype']
    helper = e['tcg_op']
    vlen = 16 if isa == 'LSX' else 32
    et, eb, ct = elem_type(name)
    op = get_op(helper)
    tname = 'trans_' + name

    if gtype in ('vvv', 'xxx') and op and et:
        base = name.replace('xv', 'v') if name.startswith('xv') else name

        # Inline helper (only for LSX, LASX reuses)
        if isa == 'LSX':
            ut = unsigned_type(name)
            st = signed_type(name)

            if op == 'ADD':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = env->fpr[a->vj].vreg.{et}[i] + env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'SUB':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = env->fpr[a->vj].vreg.{et}[i] - env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'MUL':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = env->fpr[a->vj].vreg.{et}[i] * env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'SMAX':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++) {{")
                lines.append(f"        {st} vj = env->fpr[a->vj].vreg.{et}[i];")
                lines.append(f"        {st} vk = env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = vj > vk ? vj : vk;")
                lines.append(f"    }}")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'UMAX':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++) {{")
                lines.append(f"        {ut} vj = env->fpr[a->vj].vreg.{et}[i];")
                lines.append(f"        {ut} vk = env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = vj > vk ? vj : vk;")
                lines.append(f"    }}")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'SMIN':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++) {{")
                lines.append(f"        {st} vj = env->fpr[a->vj].vreg.{et}[i];")
                lines.append(f"        {st} vk = env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = vj < vk ? vj : vk;")
                lines.append(f"    }}")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'UMIN':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++) {{")
                lines.append(f"        {ut} vj = env->fpr[a->vj].vreg.{et}[i];")
                lines.append(f"        {ut} vk = env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = vj < vk ? vj : vk;")
                lines.append(f"    }}")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'AND':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = env->fpr[a->vj].vreg.{et}[i] & env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'OR':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = env->fpr[a->vj].vreg.{et}[i] | env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'XOR':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = env->fpr[a->vj].vreg.{et}[i] ^ env->fpr[a->vk].vreg.{et}[i];")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")
            elif op == 'NEG':
                lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {{")
                lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
                lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = -env->fpr[a->vj].vreg.{et}[i];")
                lines.append(f"    env->pc += 4; return true;")
                lines.append(f"}}")

        # Trans wrapper
        if isa == 'LSX':
            lines.append(f"static bool trans_{name}(CPULoongArchState *env, arg_{name} *restrict a) {{CHECK_FPE(16); return {name}(env, (arg_vvv*)a, 16);}}")
        else:
            lines.append(f"static bool trans_{name}(CPULoongArchState *env, arg_{base} *restrict a) {{CHECK_FPE(32); return {base}(env, (arg_vvv*)a, 32);}}")
        lines.append("")

    elif gtype in ('vv', 'xx') and et:
        base = name.replace('xv', 'v') if name.startswith('xv') else name
        if isa == 'LSX' and op in ('NEG',):
            # 2-operand negate
            lines.append(f"static inline bool {name}(CPULoongArchState *env, arg_vv *restrict a, uint32_t vlen) {{")
            lines.append(f"    for (uint32_t i = 0; i < vlen / {eb}; i++)")
            lines.append(f"        env->fpr[a->vd].vreg.{et}[i] = -env->fpr[a->vj].vreg.{et}[i];")
            lines.append(f"    env->pc += 4; return true;")
            lines.append(f"}}")
            lines.append(f"static bool trans_{name}(CPULoongArchState *env, arg_{name} *restrict a) {{CHECK_FPE(16); return {name}(env, (arg_vv*)a, 16);}}")
            lines.append(f"static bool trans_xv{name}(CPULoongArchState *env, arg_xv{name} *restrict a) {{CHECK_FPE(32); return {name}(env, (arg_vv*)a, 32);}}")
            lines.append("")

with open(OUTPUT, "w") as f:
    f.write('\n'.join(lines))

print(f"Generated {len(lines)} lines to {OUTPUT}")
print(f"trans wrappers: {len([l for l in lines if 'static bool trans_' in l])}")
