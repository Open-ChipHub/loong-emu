/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * LoongArch CPU helpers for qemu
 *
 * Copyright (c) 2024 Loongson Technology Corporation Limited
 *
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "internals.h"
#include "cpu-csr.h"

static int loongarch_map_tlb_entry(CPULoongArchState *env, hwaddr *physical,
                                   int *prot, target_ulong address,
                                   int access_type, int index, int mmu_idx)
{
    LoongArchTLB *tlb = &env->tlb[index];
    uint64_t plv = mmu_idx;
    uint64_t tlb_entry, tlb_ppn;
    uint8_t tlb_ps, n, tlb_p, tlb_v, tlb_d, tlb_w, tlb_plv, tlb_nx, tlb_nr, tlb_rplv;

    if (index >= LOONGARCH_STLB) {
        tlb_ps = FIELD_EX64(tlb->tlb_misc, TLB_MISC, PS);
    } else {
        tlb_ps = FIELD_EX64(env->CSR_STLBPS, CSR_STLBPS, PS);
    }
    n = (address >> tlb_ps) & 0x1;/* Odd or even */

    tlb_entry = n ? tlb->tlb_entry1 : tlb->tlb_entry0;
    tlb_p = FIELD_EX64(tlb_entry, TLBENTRY, P);
    tlb_v = FIELD_EX64(tlb_entry, TLBENTRY, V);
    tlb_d = FIELD_EX64(tlb_entry, TLBENTRY, D);
    tlb_w = FIELD_EX64(tlb_entry, TLBENTRY, W);
    tlb_plv = FIELD_EX64(tlb_entry, TLBENTRY, PLV);
    if (is_la64(env)) {
        tlb_ppn = FIELD_EX64(tlb_entry, TLBENTRY_64, PPN);
        tlb_nx = FIELD_EX64(tlb_entry, TLBENTRY_64, NX);
        tlb_nr = FIELD_EX64(tlb_entry, TLBENTRY_64, NR);
        tlb_rplv = FIELD_EX64(tlb_entry, TLBENTRY_64, RPLV);
    } else {
        tlb_ppn = FIELD_EX64(tlb_entry, TLBENTRY_32, PPN);
        tlb_nx = 0;
        tlb_nr = 0;
        tlb_rplv = 0;
    }

    /* Remove sw bit between bit12 -- bit PS*/
    tlb_ppn = tlb_ppn & ~(((0x1UL << (tlb_ps - 12)) -1));

    /* Check hw_ptw related issue. LA32R has no P/W bits in TLBELO. */
    if (!is_la32r(env) && enable_hw_ptw(env) && tlb_p && !tlb_v) {
        return TLBRET_PTW_SET_D;
    }
    if (!is_la32r(env) && enable_hw_ptw(env) && ptw_hw_setVD &&
        access_type == MMU_DATA_STORE && tlb_w && !tlb_d) {
        return TLBRET_PTW_SET_D;
    }

    /* Check access rights */
    if (!tlb_v) {
        return TLBRET_INVALID;
    }

    if (access_type == MMU_INST_FETCH && tlb_nx) {
        return TLBRET_XI;
    }

    if (access_type == MMU_DATA_LOAD && tlb_nr) {
        return TLBRET_RI;
    }

    if (((tlb_rplv == 0) && (plv > tlb_plv)) ||
        ((tlb_rplv == 1) && (plv != tlb_plv))) {
        return TLBRET_PE;
    }

    if ((access_type == MMU_DATA_STORE) && !tlb_d) {
        return TLBRET_DIRTY;
    }

    *physical = (tlb_ppn << R_TLBENTRY_64_PPN_SHIFT) |
                (address & MAKE_64BIT_MASK(0, tlb_ps));
    *prot = PAGE_READ;
    if (tlb_d) {
        *prot |= PAGE_WRITE;
    }
    if (!tlb_nx) {
        *prot |= PAGE_EXEC;
    }
    return TLBRET_MATCH;
}

/*
 * One tlb entry holds an adjacent odd/even pair, the vpn is the
 * content of the virtual page number divided by 2. So the
 * compare vpn is bit[47:15] for 16KiB page. while the vppn
 * field in tlb entry contains bit[47:13], so need adjust.
 * virt_vpn = vaddr[47:13]
 */
bool loongarch_tlb_search(CPULoongArchState *env, target_ulong vaddr,
                          int *index)
{
    LoongArchTLB *tlb;
    uint16_t csr_asid, tlb_asid, stlb_idx;
    uint8_t tlb_e, tlb_ps, tlb_g, stlb_ps;
    int i, compare_shift;
    uint64_t vpn, tlb_vppn;

    csr_asid = FIELD_EX64(env->CSR_ASID, CSR_ASID, ASID);
    stlb_ps = FIELD_EX64(env->CSR_STLBPS, CSR_STLBPS, PS);
    vpn = (vaddr & TARGET_VIRT_MASK) >> (stlb_ps + 1);
    stlb_idx = vpn & 0xff; /* VA[25:15] <==> TLBIDX.index for 16KiB Page */
    compare_shift = stlb_ps + 1 - R_TLB_MISC_VPPN_SHIFT;

    bool hit_stlb = false;
    bool hit_mtlb = false;

    if (is_la32r(env)) {
        vpn = (vaddr & TARGET_VIRT_MASK) >> (stlb_ps + 1);
        for (i = 0; i < 16; ++i) {
            tlb = &env->tlb[i];
            tlb_e = FIELD_EX64(tlb->tlb_misc, TLB_MISC, E);
            if (tlb_e) {
                tlb_vppn = FIELD_EX64(tlb->tlb_misc, TLB_MISC, VPPN);
                tlb_asid = FIELD_EX64(tlb->tlb_misc, TLB_MISC, ASID);
                tlb_g = FIELD_EX64(tlb->tlb_misc, TLB_MISC, G);

                if ((tlb_g == 1 || tlb_asid == csr_asid) &&
                    (vpn == (tlb_vppn >> compare_shift))) {
                    *index = i;
                    return true;
                }
            }
        }
        return false;
    }

    /* Search STLB */
    for (i = 0; i < 8; ++i) {
        tlb = &env->tlb[i * 256 + stlb_idx];
        tlb_e = FIELD_EX64(tlb->tlb_misc, TLB_MISC, E);
        if (tlb_e) {
            tlb_vppn = FIELD_EX64(tlb->tlb_misc, TLB_MISC, VPPN);
            tlb_asid = FIELD_EX64(tlb->tlb_misc, TLB_MISC, ASID);
            tlb_g = FIELD_EX64(tlb->tlb_misc, TLB_MISC, G);

            if ((tlb_g == 1 || tlb_asid == csr_asid) &&
                (vpn == (tlb_vppn >> compare_shift))) {
                *index = i * 256 + stlb_idx;
                if (check_level_mask(CPU_CHECK_TLB_MHIT)) {
                    lsassertm(!hit_stlb, "stlb multi-hit\n");
                    hit_stlb = true;
                } else {
                    return true;
                }
            }
        }
    }

    /* Search MTLB */
    for (i = LOONGARCH_STLB; i < LOONGARCH_TLB_MAX; ++i) {
        tlb = &env->tlb[i];
        tlb_e = FIELD_EX64(tlb->tlb_misc, TLB_MISC, E);
        if (tlb_e) {
            tlb_vppn = FIELD_EX64(tlb->tlb_misc, TLB_MISC, VPPN);
            tlb_ps = FIELD_EX64(tlb->tlb_misc, TLB_MISC, PS);
            tlb_asid = FIELD_EX64(tlb->tlb_misc, TLB_MISC, ASID);
            tlb_g = FIELD_EX64(tlb->tlb_misc, TLB_MISC, G);
            compare_shift = tlb_ps + 1 - R_TLB_MISC_VPPN_SHIFT;
            vpn = (vaddr & TARGET_VIRT_MASK) >> (tlb_ps + 1);
            if ((tlb_g == 1 || tlb_asid == csr_asid) &&
                (vpn == (tlb_vppn >> compare_shift))) {
                *index = i;
                if (check_level_mask(CPU_CHECK_TLB_MHIT)) {
                    lsassertm(!hit_stlb, "mtlb multi-hit with stlb\n");
                    lsassertm(!hit_mtlb, "mtlb multi-hit\n");
                    hit_mtlb = true;
                } else {
                    return true;
                }
            }
        }
    }
    return hit_stlb | hit_mtlb;
}

static void hw_ptw_setVD(CPULoongArchState *env, uint64_t* csr_tlbrelo,
                         uint64_t pte_addr, MMUAccessType access_type)
{
    if (is_la32r(env)) {
        bool v = FIELD_EX64(*csr_tlbrelo, TLBENTRY, V);
        bool d = FIELD_EX64(*csr_tlbrelo, TLBENTRY, D);
        bool write_d = access_type == MMU_DATA_STORE && v && !d;

        if (write_d) {
            *csr_tlbrelo = FIELD_DP64(*csr_tlbrelo, TLBENTRY, D, 1);
            uint32_t pte = ram_lduw(pte_addr & TARGET_PHYS_MASK);
            pte = FIELD_DP32(pte, TLBENTRY, D, 1);
            ram_stw(pte_addr & TARGET_PHYS_MASK, pte);
        }
        return;
    }

    bool p = FIELD_EX64(*csr_tlbrelo, TLBENTRY, P);
    bool v = FIELD_EX64(*csr_tlbrelo, TLBENTRY, V);
    bool w = FIELD_EX64(*csr_tlbrelo, TLBENTRY, W);
    bool d = FIELD_EX64(*csr_tlbrelo, TLBENTRY, D);

    bool write_d = false;
    bool write_v = p && !v;

    *csr_tlbrelo = FIELD_DP64(*csr_tlbrelo, TLBENTRY, V, p);
    if (access_type == MMU_DATA_STORE && w && !d) {
        *csr_tlbrelo = FIELD_DP64(*csr_tlbrelo, TLBENTRY, D, 1);
        write_d = true;
    }

    uint64_t pte = ram_ldd(pte_addr & TARGET_PHYS_MASK);
    if (write_v) {
        pte = FIELD_DP64(pte, TLBENTRY, V, 1);
    }
    if (write_d) {
        pte = FIELD_DP64(pte, TLBENTRY, D, 1);
    }

    ram_std(pte_addr & TARGET_PHYS_MASK, pte);
}

static int loongarch_map_address(CPULoongArchState *env, hwaddr *physical,
                                 int *prot, target_ulong address,
                                 MMUAccessType access_type, int mmu_idx)
{
    int index, match, tlbret = 0;

again:
    match = loongarch_tlb_search(env, address, &index);
    if (match) {
        tlbret = loongarch_map_tlb_entry(env, physical, prot,
                                       address, access_type, index, mmu_idx);
        if (tlbret != TLBRET_PTW_SET_D) {
            return tlbret;
        }
    }

    if (enable_hw_ptw(env) && (!match || (match && tlbret == TLBRET_PTW_SET_D)))
    {
        /* save tlbr csr state */
        uint64_t CSR_TLBRERA, CSR_TLBRBADV, CSR_TLBREHI, CSR_TLBRELO0, CSR_TLBRELO1;
        CSR_TLBRERA = env->CSR_TLBRERA;
        CSR_TLBRBADV = env->CSR_TLBRBADV;
        CSR_TLBREHI = env->CSR_TLBREHI;
        CSR_TLBRELO0 = env->CSR_TLBRELO0;
        CSR_TLBRELO1 = env->CSR_TLBRELO1;

        env->CSR_TLBRERA = FIELD_DP64(env->CSR_TLBRERA, CSR_TLBRERA, ISTLBR, 1);
        env->CSR_TLBRBADV = address;
        if (is_la64(env)) {
            env->CSR_TLBREHI = FIELD_DP64(env->CSR_TLBREHI, CSR_TLBREHI_64,
                                        VPPN, extract64(address, 13, 35));
        } else {
            env->CSR_TLBREHI = FIELD_DP64(env->CSR_TLBREHI, CSR_TLBREHI_32,
                                        VPPN, extract64(address, 13, 19));
        }
        uint64_t pt_base = ((address >> 63) & 0x1) ? env->CSR_PGDH : env->CSR_PGDL;
        uint64_t dir_phys_addr = 0, pte0_phys_addr = 0, pte1_phys_addr = 0, huge_phys_addr = 0;
        bool is_huge = false;
        bool is_odd_page = false;
        for (int level = 4; level >= 1; level--) {
            uint64_t dir_base, dir_width;
            get_dir_base_width(env, &dir_base, &dir_width, level);
            if (dir_width) {
                pt_base = helper_lddir(env, pt_base, level, 0, &dir_phys_addr);
                if (FIELD_EX64(pt_base, TLBENTRY, HUGE) && !is_huge) {
                    huge_phys_addr = dir_phys_addr;
                    is_huge = true;
                }
            }
        }

        helper_ldpte(env, pt_base, 0, 0, &pte0_phys_addr);
        helper_ldpte(env, pt_base, 1, 0, &pte1_phys_addr);

        uint8_t tlb_ps = FIELD_EX64(env->CSR_STLBPS, CSR_STLBPS, PS);
        is_odd_page = (address >> tlb_ps) & 0x1;/* Odd or even */

        if (ptw_hw_setVD) {
            if (is_huge) {
                hw_ptw_setVD(env, &env->CSR_TLBRELO0, huge_phys_addr, access_type);
                hw_ptw_setVD(env, &env->CSR_TLBRELO1, huge_phys_addr, access_type);
            } else {
                if (is_odd_page) {
                    hw_ptw_setVD(env, &env->CSR_TLBRELO1, pte1_phys_addr, access_type);
                } else {
                    hw_ptw_setVD(env, &env->CSR_TLBRELO0, pte0_phys_addr, access_type);
                }
            }
            if (!is_la32r(env)) {
                /* Set the V bit in the tlb entry to the value
                 * of the P bit in the page table entry */
                env->CSR_TLBRELO0 = FIELD_DP64(env->CSR_TLBRELO0, TLBENTRY, V,
                                               FIELD_EX64(env->CSR_TLBRELO0, TLBENTRY, P));
                env->CSR_TLBRELO1 = FIELD_DP64(env->CSR_TLBRELO1, TLBENTRY, V,
                                               FIELD_EX64(env->CSR_TLBRELO1, TLBENTRY, P));
            }

            // Since during the address translation process,
            // when tlb hits but D bit needs to be written,
            // ptw will be used to write D bit, so the hit tlb
            // entry needs to be invalidated here.
            // Otherwise, multiple hits in tlb may occur.
            helper_invtlb_page_asid_or_g(env, env->CSR_ASID, address);
        }

        if (!is_la32r(env)) {
            env->CSR_TLBRELO0 = FIELD_DP64(env->CSR_TLBRELO0, TLBENTRY, V,
                                           FIELD_EX64(env->CSR_TLBRELO0, TLBENTRY, V) |
                                           FIELD_EX64(env->CSR_TLBRELO0, TLBENTRY, P));
            env->CSR_TLBRELO1 = FIELD_DP64(env->CSR_TLBRELO1, TLBENTRY, V,
                                           FIELD_EX64(env->CSR_TLBRELO1, TLBENTRY, V) |
                                           FIELD_EX64(env->CSR_TLBRELO1, TLBENTRY, P));
        }
        if (!is_la32r(env) && access_type == MMU_DATA_STORE) {
            uint64_t *tlbrelo = is_odd_page ? &env->CSR_TLBRELO1 : &env->CSR_TLBRELO0;
            if (FIELD_EX64(*tlbrelo, TLBENTRY, V) &&
                FIELD_EX64(*tlbrelo, TLBENTRY, W)) {
                *tlbrelo = FIELD_DP64(*tlbrelo, TLBENTRY, D, 1);
            }
        }
        if (match && tlbret == TLBRET_PTW_SET_D) {
            helper_invtlb_page_asid_or_g(env, env->CSR_ASID, address);
        }

        helper_tlbfill(env);
        if (qemu_loglevel_mask(CPU_LOG_INT)) {
            qemu_log("%s: TLBRERA 0x%lx\n", __func__, env->CSR_TLBRERA);
        }

        if (qemu_loglevel_mask(CPU_LOG_PTW)) {
            qemu_log("PTW: address=0x%lx, is_odd_page=%d,is_huge=%d,CSR_TLBREHI=0x%lx,CSR_TLBRELO0=0x%lx,CSR_TLBRELO1=0x%lx,",
                address, is_odd_page, is_huge, env->CSR_TLBREHI, env->CSR_TLBRELO0, env->CSR_TLBRELO1);
            qemu_log("pte0_phys_addr=0x%lx,pte1_phys_addr=0x%lx,huge_phys_addr=0x%lx\n",
                pte0_phys_addr, pte1_phys_addr, huge_phys_addr);
        }

        /* restore tlbr csr state */
        env->CSR_TLBRERA = CSR_TLBRERA;
        env->CSR_TLBRBADV = CSR_TLBRBADV;
        env->CSR_TLBREHI = CSR_TLBREHI;
        env->CSR_TLBRELO0 = CSR_TLBRELO0;
        env->CSR_TLBRELO1 = CSR_TLBRELO1;

        goto again;
    }

    return TLBRET_NOMATCH;
}

static hwaddr dmw_va2pa(CPULoongArchState *env, target_ulong va,
                        target_ulong dmw)
{
    if (is_la64(env)) {
        return va & TARGET_VIRT_MASK;
    } else {
        uint32_t va32 = (uint32_t)va;
        uint32_t pseg = FIELD_EX32(dmw, CSR_DMW_32, PSEG);
        return (va32 & MAKE_64BIT_MASK(0, R_CSR_DMW_32_VSEG_SHIFT)) | \
            (pseg << R_CSR_DMW_32_VSEG_SHIFT);
    }
}

int get_physical_address(CPULoongArchState *env, hwaddr *physical,
                         int *prot, target_ulong address,
                         MMUAccessType access_type, int mmu_idx)
{
    int user_mode = mmu_idx == MMU_USER_IDX;
    int kernel_mode = mmu_idx == MMU_KERNEL_IDX;
    uint32_t plv, base_c, base_v;
    int64_t addr_high;
    uint8_t da = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, DA);
    uint8_t pg = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PG);

    if (!is_la64(env)) address = (uint32_t)address;

    /* Check PG and DA */
    if (da && !pg) {
        *physical = address & TARGET_PHYS_MASK;
        *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
        return TLBRET_MATCH;
    }

    plv = kernel_mode | (user_mode << R_CSR_DMW_PLV3_SHIFT);
    if (is_la64(env)) {
        base_v = address >> R_CSR_DMW_64_VSEG_SHIFT;
    } else {
        base_v = address >> R_CSR_DMW_32_VSEG_SHIFT;
    }
    /* Check direct map window */
    for (int i = 0; i < 4; i++) {
        if (is_la64(env)) {
            base_c = FIELD_EX64(env->CSR_DMW[i], CSR_DMW_64, VSEG);
        } else {
            base_c = FIELD_EX64(env->CSR_DMW[i], CSR_DMW_32, VSEG);
        }
        if ((plv & env->CSR_DMW[i]) && (base_c == base_v)) {
            *physical = dmw_va2pa(env, address, env->CSR_DMW[i]);
            *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
            return TLBRET_MATCH;
        }
    }

    /* Check valid extension */
    addr_high = sextract64(address, TARGET_VIRT_ADDR_SPACE_BITS, 16);
    if (!(addr_high == 0 || addr_high == -1)) {
        return TLBRET_BADADDR;
    }

    /* Mapped address */
    return loongarch_map_address(env, physical, prot, address,
                                 access_type, mmu_idx);
}

hwaddr loongarch_cpu_get_phys_page_debug(CPUState *cs, vaddr addr)
{
    CPULoongArchState *env = cpu_env(cs);
    hwaddr phys_addr;
    int prot;

    if (get_physical_address(env, &phys_addr, &prot, addr, MMU_DATA_LOAD,
                             cpu_mmu_index(cs, false)) != 0) {
        return -1;
    }
    return phys_addr;
}

static target_ulong helper_lddir_debug(CPULoongArchState *env, target_ulong base,
                          target_ulong level, uint32_t mem_idx, target_ulong address,
                          target_ulong* dir_phys_addr)
{
    CPUState *cs = env_cpu(env);
    target_ulong badvaddr, index, phys, ret;
    int shift;
    uint64_t dir_base, dir_width;

    if (unlikely((level == 0) || (level > 4))) {
        qemu_log_mask(LOG_GUEST_ERROR,
                      "Attepted LDDIR with level %"PRId64"\n", level);
        return base;
    }

    if (FIELD_EX64(base, TLBENTRY, HUGE)) {
        if (unlikely(level == 4)) {
            qemu_log_mask(LOG_GUEST_ERROR,
                          "Attempted use of level 4 huge page\n");
        }

        if (FIELD_EX64(base, TLBENTRY, LEVEL)) {
            return base;
        } else {
            return FIELD_DP64(base, TLBENTRY, LEVEL, level);
        }
    }

    badvaddr = address;
    base = base & TARGET_PHYS_MASK;

    /* 0:64bit, 1:128bit, 2:192bit, 3:256bit */
    shift = FIELD_EX64(env->CSR_PWCL, CSR_PWCL, PTEWIDTH);
    shift = (shift + 1) * 3;

    get_dir_base_width(env, &dir_base, &dir_width, level);
    index = (badvaddr >> dir_base) & ((1 << dir_width) - 1);
    phys = base | index << shift;
    *dir_phys_addr = phys;
    ret = ldq_phys(cs->as, phys & TARGET_PHYS_MASK);
    return ret;
}


static void helper_ldpte_debug(CPULoongArchState *env, target_ulong base, target_ulong odd,
                               uint32_t mem_idx, target_ulong* pte_phys_addr, target_ulong address,
                               target_ulong* pte_value)
{
    CPUState *cs = env_cpu(env);
    target_ulong phys, tmp0, ptindex, ptoffset0, ptoffset1, ps, badv;
    int shift;
    uint64_t ptbase = FIELD_EX64(env->CSR_PWCL, CSR_PWCL, PTBASE);
    uint64_t ptwidth = FIELD_EX64(env->CSR_PWCL, CSR_PWCL, PTWIDTH);
    uint64_t dir_base, dir_width;

    if (FIELD_EX64(base, TLBENTRY, HUGE)) {
        get_dir_base_width(env, &dir_base, &dir_width,
                           FIELD_EX64(base, TLBENTRY, LEVEL));

        base = FIELD_DP64(base, TLBENTRY, LEVEL, 0);
        base = FIELD_DP64(base, TLBENTRY, HUGE, 0);
        if (FIELD_EX64(base, TLBENTRY, HGLOBAL)) {
            base = FIELD_DP64(base, TLBENTRY, HGLOBAL, 0);
            base = FIELD_DP64(base, TLBENTRY, G, 1);
        }

        ps = dir_base + dir_width - 1;
        tmp0 = base;
        if (odd) {
            tmp0 += MAKE_64BIT_MASK(ps, 1);
        }
    } else {
        /* 0:64bit, 1:128bit, 2:192bit, 3:256bit */
        shift = FIELD_EX64(env->CSR_PWCL, CSR_PWCL, PTEWIDTH);
        shift = (shift + 1) * 3;
        badv = address;

        ptindex = (badv >> ptbase) & ((1 << ptwidth) - 1);
        ptindex = ptindex & ~0x1;   /* clear bit 0 */
        ptoffset0 = ptindex << shift;
        ptoffset1 = (ptindex + 1) << shift;

        phys = base | (odd ? ptoffset1 : ptoffset0);
        *pte_phys_addr = phys;
        tmp0 = ldq_phys(cs->as, phys & TARGET_PHYS_MASK);
        ps = ptbase;
    }
    *pte_value = tmp0;
}


static int loongarch_check_tlb_entry(CPULoongArchState *env, hwaddr *physical,
                                     int *prot, target_ulong address,
                                     int access_type, uint64_t tlb_entry0, int mmu_idx)
{
    uint64_t plv = mmu_idx;
    uint64_t tlb_entry, tlb_ppn;
    uint8_t tlb_ps, tlb_v, tlb_d, tlb_plv, tlb_nx, tlb_nr, tlb_rplv;

    tlb_ps = FIELD_EX64(env->CSR_STLBPS, CSR_STLBPS, PS);

    tlb_entry = tlb_entry0;
    tlb_v = FIELD_EX64(tlb_entry, TLBENTRY, V);
    tlb_d = FIELD_EX64(tlb_entry, TLBENTRY, D);
    tlb_plv = FIELD_EX64(tlb_entry, TLBENTRY, PLV);
    if (is_la64(env)) {
        tlb_ppn = FIELD_EX64(tlb_entry, TLBENTRY_64, PPN);
        tlb_nx = FIELD_EX64(tlb_entry, TLBENTRY_64, NX);
        tlb_nr = FIELD_EX64(tlb_entry, TLBENTRY_64, NR);
        tlb_rplv = FIELD_EX64(tlb_entry, TLBENTRY_64, RPLV);
    } else {
        tlb_ppn = FIELD_EX64(tlb_entry, TLBENTRY_32, PPN);
        tlb_nx = 0;
        tlb_nr = 0;
        tlb_rplv = 0;
    }

    /* Remove sw bit between bit12 -- bit PS*/
    tlb_ppn = tlb_ppn & ~(((0x1UL << (tlb_ps - 12)) -1));

    /* Check access rights */
    if (!tlb_v) {
        return TLBRET_INVALID;
    }

    if (access_type == MMU_INST_FETCH && tlb_nx) {
        return TLBRET_XI;
    }

    if (access_type == MMU_DATA_LOAD && tlb_nr) {
        return TLBRET_RI;
    }

    if (((tlb_rplv == 0) && (plv > tlb_plv)) ||
        ((tlb_rplv == 1) && (plv != tlb_plv))) {
        return TLBRET_PE;
    }

    if ((access_type == MMU_DATA_STORE) && !tlb_d) {
        return TLBRET_DIRTY;
    }

    *physical = (tlb_ppn << R_TLBENTRY_64_PPN_SHIFT) |
                (address & MAKE_64BIT_MASK(0, tlb_ps));
    *prot = PAGE_READ;
    if (tlb_d) {
        *prot |= PAGE_WRITE;
    }
    if (!tlb_nx) {
        *prot |= PAGE_EXEC;
    }
    return TLBRET_MATCH;
}


static int loongarch_map_address_debug(CPULoongArchState *env, hwaddr *physical,
                                 int *prot, target_ulong address,
                                 MMUAccessType access_type, int mmu_idx)
{
    int index, match;

    match = loongarch_tlb_search(env, address, &index);
    if (match) {
        return loongarch_map_tlb_entry(env, physical, prot,
                                       address, access_type, index, mmu_idx);
    }

    if (!match)
    {
        target_ulong pte_value;
        uint8_t tlb_ps;
        int n;

        uint64_t pt_base = ((address >> 63) & 0x1) ? env->CSR_PGDH : env->CSR_PGDL;
        uint64_t dir_phys_addr = 0, pte0_phys_addr = 0;
        bool is_huge = false;
        for (int level = 4; level >= 1; level--) {
            uint64_t dir_base, dir_width;
            get_dir_base_width(env, &dir_base, &dir_width, level);
            if (dir_width) {
                pt_base = helper_lddir_debug(env, pt_base, level, 0, address, &dir_phys_addr);
                if (FIELD_EX64(pt_base, TLBENTRY, HUGE) && !is_huge) {
                    is_huge = true;
                }
            }
        }

        tlb_ps = FIELD_EX64(env->CSR_STLBPS, CSR_STLBPS, PS);

        n = (address >> tlb_ps) & 0x1;/* Odd or even */
        helper_ldpte_debug(env, pt_base, n, 0, &pte0_phys_addr, address, &pte_value);

        return loongarch_check_tlb_entry(env, physical, prot, address,
                                        access_type, pte_value, mmu_idx);
    }

    return TLBRET_NOMATCH;
}


int get_physical_address_debug(CPULoongArchState *env, hwaddr *physical,
                         int *prot, target_ulong address,
                         MMUAccessType access_type, int mmu_idx)
{
    int user_mode = mmu_idx == MMU_USER_IDX;
    int kernel_mode = mmu_idx == MMU_KERNEL_IDX;
    uint32_t plv, base_c, base_v;
    int64_t addr_high;
    uint8_t da = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, DA);
    uint8_t pg = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PG);

    if (!is_la64(env)) address = (uint32_t)address;

    /* Check PG and DA */
    if (da && !pg) {
        *physical = address & TARGET_PHYS_MASK;
        *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
        return TLBRET_MATCH;
    }

    plv = kernel_mode | (user_mode << R_CSR_DMW_PLV3_SHIFT);
    if (is_la64(env)) {
        base_v = address >> R_CSR_DMW_64_VSEG_SHIFT;
    } else {
        base_v = address >> R_CSR_DMW_32_VSEG_SHIFT;
    }
    /* Check direct map window */
    for (int i = 0; i < 4; i++) {
        if (is_la64(env)) {
            base_c = FIELD_EX64(env->CSR_DMW[i], CSR_DMW_64, VSEG);
        } else {
            base_c = FIELD_EX64(env->CSR_DMW[i], CSR_DMW_32, VSEG);
        }
        if ((plv & env->CSR_DMW[i]) && (base_c == base_v)) {
            *physical = dmw_va2pa(env, address, env->CSR_DMW[i]);
            *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
            return TLBRET_MATCH;
        }
    }

    /* Check valid extension */
    addr_high = sextract64(address, TARGET_VIRT_ADDR_SPACE_BITS, 16);
    if (!(addr_high == 0 || addr_high == -1)) {
        return TLBRET_BADADDR;
    }

    /* Mapped address */
    return loongarch_map_address_debug(env, physical, prot, address,
                                        access_type, mmu_idx);
}
