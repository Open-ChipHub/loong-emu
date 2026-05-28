#define _GNU_SOURCE

#include "qemu/osdep.h"
#include "util.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>

#include <elf.h>

#include "sizes.h"
#include "cpu.h"
#include "cpu-csr.h"
#include "internals.h"

#if defined(CONFIG_GDB)
#include "gdbstub/gdbserver.h"
#endif
#if defined(CONFIG_USER_ONLY)
#include "user.h"
#else
#include "irq.h"
#include "serial.h"
#include "serial_plus.h"
#endif
#if defined(CONFIG_PLUGIN)
#include <dlfcn.h>
#endif
#if defined(CONFIG_DIFF_NET)
#include "diffnet.h"
#endif
#include "insn_stats.h"
#include "smp.h"

#if defined(CONFIG_PLUGIN)
la_emu_plugin_ops* plugin_ops;
char plugin_name[PATH_MAX];
char plugin_arg[PATH_MAX];
#endif
bool new_abi;
bool determined;
bool hw_ptw;
bool ptw_hw_setVD = true;
bool serial_plus;
#if !defined(CONFIG_USER_ONLY)
SerialState *ss;
timer_t serial_timerid;
volatile sig_atomic_t serial_timer_int;
#endif
__thread CPULoongArchState *current_env;

int gdbserver = 0;
#if defined(CONFIG_DIFF_NET)
static char diffnet_host[256];
static int  diffnet_port = -1;
static bool diffnet_spawn_mode = false;
#endif
extern int check_signal;
char *report_filename = "report_instruction.md";
const char *arch_name = NULL;
const char *kernel_cmdline = NULL;

#if !defined(CONFIG_CLI) && !defined(CONFIG_PLUGIN)
struct termios saved_termios;
int saved_stdin_flags;
bool term_saved;

void restore_terminal(void)
{
    if (term_saved) {
        tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
        fcntl(STDIN_FILENO, F_SETFL, saved_stdin_flags);
        term_saved = false;
    }
}
#endif

extern int64_t singlestep;

extern void handle_debug_cli(CPULoongArchState *env);
extern void show_register(CPULoongArchState *env);
extern void show_register_fpr(CPULoongArchState *env);
extern void set_fetch_breakpoint(int idx, target_long pc);
extern void restore_checkpoint(CPULoongArchState *env, char* image_dir);

void laemu_host_quit(void)
{
    laemu_exit(0);
}

#ifdef CONFIG_CLI
static void sigaction_entry_int(int signal, siginfo_t *si, void *arg) {
    if (check_signal > 3) {
        fprintf(stderr, "exit");
        laemu_exit(EXIT_SUCCESS);
    }
    check_signal++;
}
#endif

#if !defined (CONFIG_USER_ONLY) && !defined (CONFIG_DIFF)
static void sigaction_entry_timer(int signal, siginfo_t *si, void *arg) {
    timer_t id = *((timer_t*)si->si_value.sival_ptr);
    CPULoongArchState *env = loongarch_smp_find_env_by_timerid(id);
    if (env) {
        qemu_log_mask(CPU_LOG_TIMER, "TIMER alarmed, icount:%ld\n", env->icount);
        env->timer_int = true;
    } else {
        fprintf(stderr, "TIMER, it's somebody else!\n");
    }
}

static void sigaction_entry_serial_timer(int signal, siginfo_t *si, void *arg) {
    if (!current_env) return;
    timer_t id = *((timer_t*)si->si_value.sival_ptr);
    if (id==serial_timerid) {
        qemu_log_mask(CPU_LOG_TIMER, "SERIAL TIMER alarmed, icount:%ld\n", current_env->icount);
        serial_timer_int = true;
    } else {
        fprintf(stderr, "TIMER, it's somebody else!\n");
    }
}
static void kernel_setup_signal(void) {
    struct sigaction sa;

    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sigaction_entry_timer;
    sa.sa_flags     = SA_SIGINFO;
    lsassert (sigaction(SIGRTMIN, &sa, NULL) == 0);
}
#endif

#ifdef CONFIG_CLI
static void cli_setup_signal(void) {
    struct sigaction sa;

    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sigaction_entry_int;
    sa.sa_flags     = SA_SIGINFO;
    lsassert (sigaction(SIGINT, &sa, NULL) == 0);
}
#endif

#if !defined (CONFIG_DIFF)
static void setup_signal(void) {
#ifndef CONFIG_USER_ONLY
    kernel_setup_signal();
#endif
#ifdef CONFIG_CLI
    cli_setup_signal();
#endif
}
#endif

static const char * const excp_names[] = {
    [EXCCODE_INT] = "Interrupt",
    [EXCCODE_PIL] = "Page invalid exception for load",
    [EXCCODE_PIS] = "Page invalid exception for store",
    [EXCCODE_PIF] = "Page invalid exception for fetch",
    [EXCCODE_PME] = "Page modified exception",
    [EXCCODE_PNR] = "Page Not Readable exception",
    [EXCCODE_PNX] = "Page Not Executable exception",
    [EXCCODE_PPI] = "Page Privilege error",
    [EXCCODE_ADEF] = "Address error for instruction fetch",
    [EXCCODE_ADEM] = "Address error for Memory access",
    [EXCCODE_SYS] = "Syscall",
    [EXCCODE_BRK] = "Break",
    [EXCCODE_INE] = "Instruction Non-Existent",
    [EXCCODE_IPE] = "Instruction privilege error",
    [EXCCODE_FPD] = "Floating Point Disabled",
    [EXCCODE_FPE] = "Floating Point Exception",
    [EXCCODE_DBP] = "Debug breakpoint",
    [EXCCODE_BCE] = "Bound Check Exception",
    [EXCCODE_SXD] = "128 bit vector instructions Disable exception",
    [EXCCODE_ASXD] = "256 bit vector instructions Disable exception",
};

const char *loongarch_exception_name(int32_t exception)
{
    assert(excp_names[exception]);
    return excp_names[exception];
}

#ifndef CONFIG_USER_ONLY
char* ram;
#endif
uint64_t ram_size = SZ_4G;
static uint64_t ram_map_size __attribute__((unused)) = SZ_4G;
char* kernel_filename;
#ifndef CONFIG_DIFF
static int smp_cpus = 1;
#endif

void usage(void) {
#ifndef CONFIG_USER_ONLY
    fprintf(stderr, "la_emu_kernel -m n[G] -k kernel\n");
    fprintf(stderr, "-m Memory size(kernel mode)\n");
    fprintf(stderr, "-k Kernel vmlinux or checkpoint directory(kernel mode)\n");
#else
    fprintf(stderr, "usage: la_emu_user [-d exec,cpu,page,strace,unimp] [-D logfile] program [arguments...]\n");
#endif
    fprintf(stderr, "-d Log info, support: exec,cpu,fpu,int\n");
    fprintf(stderr, "-D Log file\n");
    fprintf(stderr, "-c Check item, support: tlb_mhit\n");
    fprintf(stderr, "-z Determined events\n");
    fprintf(stderr, "-g Enable gdbserver\n");
    fprintf(stderr, "--smp n       Number of virtual CPUs\n");
    fprintf(stderr, "-w Force enable hardware page table walker\n");
    fprintf(stderr, "-A arch       CPU model: la464, loongarch64, openc910, swiftcore\n");
    fprintf(stderr, "-N host:port  Enable network difftest against QEMU GDB stub\n");
    fprintf(stderr, "-R file       Instruction stats report (default: report_instruction.md)\n");
    laemu_exit(EXIT_SUCCESS);
}

#if defined(CONFIG_USER_ONLY)
/* Allocate a 4G stack region and return the initial stack pointer. */
static target_ulong user_setup_stack() {
    void* dst = mmap(NULL, SZ_4G, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    lsassert(dst != MAP_FAILED);
    return (target_ulong)(dst + SZ_4G - 64);
}
#endif

#define elfhdr Elf64_Ehdr
#define elf_phdr Elf64_Phdr
#if !defined (CONFIG_USER_ONLY) && !defined (CONFIG_DIFF)
/* Map the guest physical address span used by the low and high RAM windows. */
static char* alloc_ram(uint64_t ram_size) {
    ram_map_size = MAX(SZ_4G, SZ_2G + ram_size);
    void* part1 = mmap(NULL, ram_map_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    lsassertm(part1 != MAP_FAILED, " mmap ram_map_size=0x%" PRIx64 "\n",
              ram_map_size);
    return part1;
}

bool addr_in_ram(hwaddr pa) {
    if (pa >= ram_map_size) {
        return false;
    }

    return
        (pa < SZ_512M) ||
        (pa >= SZ_512M && pa < ram_size + SZ_512M) ||
        (pa >= 0x1c000000 && pa < 0x1c000000 + SZ_32M);
}

bool addr_range_in_ram(hwaddr pa, unsigned size) {
    if (size == 0 || pa + size - 1 < pa) {
        return false;
    }

    hwaddr end = pa + size - 1;
    if (end >= ram_map_size) {
        return false;
    }

    return
        (end < SZ_512M) ||
        (pa >= SZ_512M && end < ram_size + SZ_512M) ||
        (pa >= 0x1c000000 && end < 0x1c000000 + SZ_32M);
}

/* Load a raw binary image at entry_addr into guest RAM. */
bool load_binary(const char* filename, uint64_t entry_addr) {
    int ret = 0;
    int size = 0;
    uint8_t *data_ptr = NULL;
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror(filename);
        ret = 1;
        goto fail;
    }

    struct stat st;
    stat(filename, &st);
    size = st.st_size;
    data_ptr = (uint8_t*)malloc(size);

    if (read(fd, data_ptr, size) != size) {
        goto fail;
    }

    hwaddr load_pa = entry_addr & 0xffffffff;
    memcpy(ram + load_pa, data_ptr, size);

    hwaddr la32_dmw_alias = load_pa & 0x1fffffff;
    if (la32_dmw_alias != load_pa && addr_range_in_ram(la32_dmw_alias, size)) {
        memcpy(ram + la32_dmw_alias, data_ptr, size);
    }

    free((void*)data_ptr);
    data_ptr = 0;

fail:
    close(fd);
    if (data_ptr)
        free((void*)data_ptr);
    return ret;
}

/* Load ELF kernel image into guest RAM, return entry point via entry_addr. */
bool load_elf(const char* filename, uint64_t* entry_addr) {
    int size, i;
    uint64_t file_size;
    uint8_t e_ident[EI_NIDENT];
    uint8_t *data = NULL;
    int ret = 1;
    elfhdr ehdr;
    elf_phdr *phdr = NULL, *ph;
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror(filename);
        goto fail;
    }

    if (read(fd, e_ident, sizeof(e_ident)) != sizeof(e_ident))
        goto fail;
    if (e_ident[0] != ELFMAG0 ||
        e_ident[1] != ELFMAG1 ||
        e_ident[2] != ELFMAG2 ||
        e_ident[3] != ELFMAG3) {
            lsassertm(0, "%s is not an elf\n", filename);
    }
    bool is_elf32 = (e_ident[EI_CLASS] == ELFCLASS32);
    lsassert(is_elf32 || e_ident[EI_CLASS] == ELFCLASS64);
    lseek(fd, 0, SEEK_SET);

    uint64_t e_entry; uint64_t e_phoff; uint16_t e_phnum;
    if (is_elf32) {
        Elf32_Ehdr ehdr32;
        if (read(fd, &ehdr32, sizeof(ehdr32)) != sizeof(ehdr32)) goto fail;
        e_entry = ehdr32.e_entry & 0x1fffffff; e_phoff = ehdr32.e_phoff; e_phnum = ehdr32.e_phnum;
    } else {
        if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) goto fail;
        e_entry = ehdr.e_entry; e_phoff = ehdr.e_phoff; e_phnum = ehdr.e_phnum;
    }
    *entry_addr = e_entry;
    if (lseek(fd, e_phoff, SEEK_SET) != (long)e_phoff) goto fail;

    if (is_elf32) {
        Elf32_Phdr *ph32 = (Elf32_Phdr *)malloc(e_phnum * sizeof(Elf32_Phdr));
        if (!ph32) goto fail;
        size = e_phnum * sizeof(Elf32_Phdr);
        if (read(fd, ph32, size) != size) { free(ph32); goto fail; }
        for (i = 0; i < e_phnum; i++) {
            if (ph32[i].p_type == PT_LOAD && ph32[i].p_filesz > 0) {
                file_size = ph32[i].p_filesz; data = (uint8_t*)malloc(file_size);
                if (lseek(fd, ph32[i].p_offset, SEEK_SET) < 0) { free(ph32); goto fail; }
                if (read(fd, data, file_size) != (ssize_t)file_size) { free(ph32); goto fail; }
                memcpy(ram + (ph32[i].p_paddr & 0x1fffffff), data, file_size);
                free(data); data = NULL;
            }
        }
        free(ph32);
    } else {
        phdr = (elf_phdr *)malloc(e_phnum * sizeof(phdr[0]));
        if (!phdr) goto fail;
        size = e_phnum * sizeof(phdr[0]);
        if (read(fd, phdr, size) != size) { free(phdr); goto fail; }
        for (i = 0; i < e_phnum; i++) {
            ph = &phdr[i];
            if (ph->p_type == PT_LOAD && ph->p_filesz > 0) {
                file_size = ph->p_filesz; data = (uint8_t*)malloc(file_size);
                if (lseek(fd, ph->p_offset, SEEK_SET) < 0) { free(phdr); goto fail; }
                if (read(fd, data, file_size) != (ssize_t)file_size) { free(phdr); goto fail; }
                memcpy(ram + (ph->p_paddr & (is_elf32 ? 0x1fffffff : 0xffffffff)), data, file_size);
                free(data); data = NULL;
            }
        }
        free(phdr); phdr = NULL;
    }

fail:
    if (phdr) {
        free(phdr);
    }
    close(fd);
    return ret;
}
#endif

#if defined(CONFIG_USER_ONLY)
char *exec_path;
char real_exec_path[PATH_MAX];
struct image_info info;
abi_ulong e_phoff;
abi_ulong e_phnum;
target_ulong TARGET_PAGE_BITS;
target_ulong TARGET_PAGE_SIZE;
target_ulong TARGET_PAGE_MASK;

/* Load user-space ELF into process memory via mmap, set up image_info. */
bool load_elf_user(const char* filename, uint64_t* entry_addr) {
    int size, i;
    uint8_t e_ident[EI_NIDENT];
    int ret = 1;
    elfhdr ehdr;
    elf_phdr *phdr = NULL, *ph;
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror(filename);
        laemu_exit(EXIT_FAILURE);
    }

    if (read(fd, e_ident, sizeof(e_ident)) != sizeof(e_ident))
        goto fail;
    if (e_ident[0] != ELFMAG0 ||
        e_ident[1] != ELFMAG1 ||
        e_ident[2] != ELFMAG2 ||
        e_ident[3] != ELFMAG3) {
            lsassertm(0, "%s is not an elf\n", filename);
    }
    bool is_elf32 = (e_ident[EI_CLASS] == ELFCLASS32);
    lsassert(is_elf32 || e_ident[EI_CLASS] == ELFCLASS64);
    lseek(fd, 0, SEEK_SET);


    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr))
        goto fail;

    *entry_addr = is_elf32 ? ((Elf32_Ehdr*)&ehdr)->e_entry & 0x1fffffff : ehdr.e_entry;

    size = ehdr.e_phnum * sizeof(phdr[0]);
    e_phoff = ehdr.e_phoff;
    e_phnum = ehdr.e_phnum;
    if (lseek(fd, ehdr.e_phoff, SEEK_SET) != ehdr.e_phoff) {
        goto fail;
    }
    phdr = (elf_phdr *)malloc(size);
    if (!phdr)
        goto fail;
    if (read(fd, phdr, size) != size)
        goto fail;

    uint64_t loaddr = -1, hiaddr = 0;
    for(i = 0; i < ehdr.e_phnum; i++) {
        ph = &phdr[i];
        if (ph->p_type == PT_LOAD) {
            abi_ulong a = ph->p_vaddr & TARGET_PAGE_MASK;
            if (a < loaddr) {
                loaddr = a;
            }
            a = ph->p_vaddr + ph->p_memsz - 1;
            if (a > hiaddr) {
                hiaddr = a;
            }
        } else if (ph->p_type == PT_INTERP) {
            qemu_log("unsupported dynamic elf\n");
            laemu_exit(0);
        }
    }

    abi_ulong load_addr = loaddr;
    if (ehdr.e_type != ET_EXEC) {
        load_addr += 0x4000000;
    }

    load_addr = (abi_ulong)mmap((void*)load_addr, (size_t)hiaddr - loaddr + 1, PROT_NONE,
                            MAP_PRIVATE | MAP_ANON | MAP_NORESERVE |
                            (ehdr.e_type == ET_EXEC ? MAP_FIXED_NOREPLACE : 0),
                            -1, 0);
    if ((void*)load_addr == MAP_FAILED) {
        lsassert(0);
    }

    abi_ulong load_bias = load_addr - loaddr;
    info.load_bias = load_bias;
    info.code_offset = load_bias;
    info.data_offset = load_bias;
    info.load_addr = load_addr;
    info.entry = ehdr.e_entry + load_bias;
    info.start_code = -1;
    info.end_code = 0;
    info.start_data = -1;
    info.end_data = 0;
    /* Usual start for brk is after all sections of the main executable. */
    info.brk = TARGET_PAGE_ALIGN(hiaddr + load_bias);
    info.elf_flags = ehdr.e_flags;
    for(i = 0; i < ehdr.e_phnum; i++) {
        ph = &phdr[i];
        if (ph->p_type == PT_LOAD) {
            abi_ulong vaddr, vaddr_po, vaddr_ps, vaddr_ef, vaddr_em;
            int elf_prot = 0;

            if (ph->p_flags & PF_R) {
                elf_prot |= PROT_READ;
            }
            if (ph->p_flags & PF_W) {
                elf_prot |= PROT_WRITE;
            }
            if (ph->p_flags & PF_X) {
                elf_prot |= PROT_EXEC;
            }

            vaddr = load_bias + ph->p_vaddr;
            if (ph->p_align <= TARGET_PAGE_SIZE) {
                vaddr_po = vaddr & ~TARGET_PAGE_MASK;
                vaddr_ps = vaddr & TARGET_PAGE_MASK;
            } else {
                vaddr_po = vaddr & (ph->p_align - 1);
                vaddr_ps = vaddr & ~(ph->p_align - 1);
            }


            vaddr_ef = vaddr + ph->p_filesz;
            vaddr_em = vaddr + ph->p_memsz;
            qemu_log_mask(CPU_LOG_PAGE, "vaddr_po:%lx vaddr_ps:%lx vaddr_ef:%lx vaddr_em:%lx\n", vaddr_po, vaddr_ps, vaddr_ef, vaddr_em);
            if (ph->p_filesz != 0) {
                void* r = mmap((void*)vaddr_ps, ph->p_filesz + vaddr_po,
                                    elf_prot, MAP_PRIVATE | MAP_FIXED,
                                    fd, ph->p_offset - vaddr_po);
                lsassert(r != MAP_FAILED);
            }
            if (vaddr_ef < vaddr_em) {
                uint64_t end = vaddr_ps + ph->p_filesz + vaddr_po;
                memset((void*)end, 0, ROUND_UP(end, TARGET_PAGE_SIZE) - end);
                abi_ulong align_bss = TARGET_PAGE_ALIGN(vaddr_ef);
                abi_ulong end_bss = TARGET_PAGE_ALIGN(vaddr_em);
                if (align_bss != end_bss) {
                    void* r = mmap((void*)align_bss, end_bss - align_bss,
                            elf_prot, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
                    lsassert(r != MAP_FAILED);
                }
            }
            /* Find the full program boundaries.  */
            if (elf_prot & PROT_EXEC) {
                if (vaddr < info.start_code) {
                    info.start_code = vaddr;
                }
                if (vaddr_ef > info.end_code) {
                    info.end_code = vaddr_ef;
                }
            }
            if (elf_prot & PROT_WRITE) {
                if (vaddr < info.start_data) {
                    info.start_data = vaddr;
                }
                if (vaddr_ef > info.end_data) {
                    info.end_data = vaddr_ef;
                }
            }
        }
    }

    if (info.end_data == 0) {
        info.start_data = info.end_code;
        info.end_data = info.end_code;
    }

fail:
    if (phdr) {
        free(phdr);
    }
    close(fd);
    return ret;
}

#endif

/* Reset CPU state to initial values: PLV=0, interrupts disabled, DA=1, PG=0. */
void cpu_reset(CPUState* cs) {

    uint8_t tlb_ps;

    CPULoongArchState *env = cpu_env(cs);
    env->fcsr0_mask = FCSR0_M1 | FCSR0_M2 | FCSR0_M3;
    env->fcsr0 = 0x0;

    int n;
    /* Set csr registers value after reset */
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, PLV, 0);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, IE, 0);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, DA, 1);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, PG, 0);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, DATF, 0);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, DATM, 0);

    env->CSR_EUEN = FIELD_DP64(env->CSR_EUEN, CSR_EUEN, FPE, 0);
    env->CSR_EUEN = FIELD_DP64(env->CSR_EUEN, CSR_EUEN, SXE, 0);
    env->CSR_EUEN = FIELD_DP64(env->CSR_EUEN, CSR_EUEN, ASXE, 0);
    env->CSR_EUEN = FIELD_DP64(env->CSR_EUEN, CSR_EUEN, BTE, 0);

    env->CSR_MISC = 0;

    env->CSR_ECFG = FIELD_DP64(env->CSR_ECFG, CSR_ECFG, VS, 0);
    env->CSR_ECFG = FIELD_DP64(env->CSR_ECFG, CSR_ECFG, LIE, 0);

    env->CSR_ESTAT = env->CSR_ESTAT & (~MAKE_64BIT_MASK(0, 2));
    env->CSR_RVACFG = FIELD_DP64(env->CSR_RVACFG, CSR_RVACFG, RBITS, 0);
    env->CSR_CPUID = cs->cpu_index;
    env->CSR_TCFG = FIELD_DP64(env->CSR_TCFG, CSR_TCFG, EN, 0);
    env->CSR_LLBCTL = FIELD_DP64(env->CSR_LLBCTL, CSR_LLBCTL, KLO, 0);
    env->CSR_TLBRERA = FIELD_DP64(env->CSR_TLBRERA, CSR_TLBRERA, ISTLBR, 0);
    env->CSR_MERRCTL = FIELD_DP64(env->CSR_MERRCTL, CSR_MERRCTL, ISMERR, 0);
    env->CSR_TID = cs->cpu_index;

    /* set CSR_PWCL.PTBASE and CSR_STLBPS.PS bits from CSR_PRCFG2 */
    if (env->CSR_PRCFG2 == 0) {
        env->CSR_PRCFG2 = 0x3fffff000;
    }
    tlb_ps = ctz32(env->CSR_PRCFG2);

    env->CSR_STLBPS = FIELD_DP64(env->CSR_STLBPS, CSR_STLBPS, PS, tlb_ps);
    env->CSR_PWCL = FIELD_DP64(env->CSR_PWCL, CSR_PWCL, PTBASE, tlb_ps);

    for (n = 0; n < 4; n++) {
        env->CSR_DMW[n] = FIELD_DP64(env->CSR_DMW[n], CSR_DMW, PLV0, 0);
        env->CSR_DMW[n] = FIELD_DP64(env->CSR_DMW[n], CSR_DMW, PLV1, 0);
        env->CSR_DMW[n] = FIELD_DP64(env->CSR_DMW[n], CSR_DMW, PLV2, 0);
        env->CSR_DMW[n] = FIELD_DP64(env->CSR_DMW[n], CSR_DMW, PLV3, 0);
    }

#ifndef CONFIG_USER_ONLY
    env->pc = 0x1c000000;
    memset(env->tlb, 0, sizeof(env->tlb));
#endif

#ifdef CONFIG_TCG
    restore_fp_status(env);
#endif
    cs->exception_index = -1;
}

/* Handle all CPU exceptions and interrupts: save context, vector to handler. */
static void loongarch_cpu_do_interrupt(CPUState *cs)
{
    LoongArchCPU *cpu = LOONGARCH_CPU(cs);
    CPULoongArchState *env = &cpu->env;
    bool update_badinstr = 1;
    int cause = -1;
    bool tlbfill = FIELD_EX64(env->CSR_TLBRERA, CSR_TLBRERA, ISTLBR);
    uint32_t vec_size = FIELD_EX64(env->CSR_ECFG, CSR_ECFG, VS);

    if (cs->exception_index != EXCCODE_INT) {
        qemu_log_mask(CPU_LOG_INT,
                     "%s enter: pc " TARGET_FMT_lx " ERA " TARGET_FMT_lx
                     " TLBRERA " TARGET_FMT_lx " exception: %d (%s)\n",
                     __func__, env->pc, env->CSR_ERA, env->CSR_TLBRERA,
                     cs->exception_index,
                     loongarch_exception_name(cs->exception_index));
    }

    switch (cs->exception_index) {
    case EXCCODE_DBP:
        env->CSR_DBG = FIELD_DP64(env->CSR_DBG, CSR_DBG, DCL, 1);
        env->CSR_DBG = FIELD_DP64(env->CSR_DBG, CSR_DBG, ECODE, 0xC);
        goto set_DERA;
    set_DERA:
        env->CSR_DERA = env->pc;
        env->CSR_DBG = FIELD_DP64(env->CSR_DBG, CSR_DBG, DST, 1);
        set_pc(env, env->CSR_EENTRY + 0x480);
        break;
    case EXCCODE_INT:
        if (FIELD_EX64(env->CSR_DBG, CSR_DBG, DST)) {
            env->CSR_DBG = FIELD_DP64(env->CSR_DBG, CSR_DBG, DEI, 1);
            goto set_DERA;
        }
        QEMU_FALLTHROUGH;
    case EXCCODE_PIF:
    case EXCCODE_ADEF:
        cause = cs->exception_index;
        update_badinstr = 0;
        break;
    case EXCCODE_SYS:
    case EXCCODE_BRK:
    case EXCCODE_INE:
    case EXCCODE_IPE:
    case EXCCODE_FPD:
    case EXCCODE_FPE:
    case EXCCODE_SXD:
    case EXCCODE_ASXD:
    case EXCCODE_BCE:
    case EXCCODE_ADEM:
    case EXCCODE_PIL:
    case EXCCODE_PIS:
    case EXCCODE_PME:
    case EXCCODE_PNR:
    case EXCCODE_PNX:
    case EXCCODE_PPI:
        cause = cs->exception_index;
        break;
    default:
        qemu_log("Error: exception(%d) has not been supported\n",
                 cs->exception_index);
        abort();
    }

    if (update_badinstr) {
        env->CSR_BADI = cpu_ldl_code(env, env->pc);
    }

    /* Save PLV and IE */
    if (tlbfill) {
        env->CSR_TLBRPRMD = FIELD_DP64(env->CSR_TLBRPRMD, CSR_TLBRPRMD, PPLV,
                                       FIELD_EX64(env->CSR_CRMD,
                                       CSR_CRMD, PLV));
        env->CSR_TLBRPRMD = FIELD_DP64(env->CSR_TLBRPRMD, CSR_TLBRPRMD, PIE,
                                       FIELD_EX64(env->CSR_CRMD, CSR_CRMD, IE));
        /* set the DA mode */
        env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, DA, 1);
        env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, PG, 0);
        env->CSR_TLBRERA = FIELD_DP64(env->CSR_TLBRERA, CSR_TLBRERA,
                                      PC, (env->pc >> 2));
    } else {
        if (cause != EXCCODE_INT || (cause == EXCCODE_INT && FIELD_EX64(env->CSR_ECFG, CSR_ECFG, VS) == 0)) {
            env->CSR_ESTAT = FIELD_DP64(env->CSR_ESTAT, CSR_ESTAT, ECODE,
                                    EXCODE_MCODE(cause));
            env->CSR_ESTAT = FIELD_DP64(env->CSR_ESTAT, CSR_ESTAT, ESUBCODE,
                                    EXCODE_SUBCODE(cause));
        }
        env->CSR_PRMD = FIELD_DP64(env->CSR_PRMD, CSR_PRMD, PPLV,
                                   FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PLV));
        env->CSR_PRMD = FIELD_DP64(env->CSR_PRMD, CSR_PRMD, PIE,
                                   FIELD_EX64(env->CSR_CRMD, CSR_CRMD, IE));
        env->CSR_ERA = env->pc;
    }

    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, PLV, 0);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, IE, 0);

    if (vec_size) {
        vec_size = (1 << vec_size) * 4;
    }

    if  (cs->exception_index == EXCCODE_INT) {
        env->irq_count ++;
        /* Interrupt */
        uint32_t vector = 0;
        uint32_t pending = FIELD_EX64(env->CSR_ESTAT, CSR_ESTAT, IS);
        pending &= FIELD_EX64(env->CSR_ECFG, CSR_ECFG, LIE);

        /* Find the highest-priority interrupt. */
        vector = 31 - clz32(pending);
        set_pc(env, env->CSR_EENTRY + \
               (EXCCODE_EXTERNAL_INT + vector) * vec_size);
        qemu_log_mask(CPU_LOG_INT,
                      "%s: PC " TARGET_FMT_lx " ERA " TARGET_FMT_lx
                      " cause %d\n" "    A " TARGET_FMT_lx " D "
                      TARGET_FMT_lx " vector = %d ExC " TARGET_FMT_lx "ExS"
                      TARGET_FMT_lx "\n",
                      __func__, env->pc, env->CSR_ERA,
                      cause, env->CSR_BADV, env->CSR_DERA, vector,
                      env->CSR_ECFG, env->CSR_ESTAT);
    } else {
        if (tlbfill) {
            env->tlbr_count ++;
            set_pc(env, env->CSR_TLBRENTRY);
        } else {
            env->ecounter[cs->exception_index] ++;
            set_pc(env, env->CSR_EENTRY + EXCODE_MCODE(cause) * vec_size);
        }
        qemu_log_mask(CPU_LOG_INT,
                      "%s: PC " TARGET_FMT_lx " ERA " TARGET_FMT_lx
                      " cause %d%s\n, ESTAT " TARGET_FMT_lx
                      " EXCFG " TARGET_FMT_lx " BADVA " TARGET_FMT_lx
                      "BADI " TARGET_FMT_lx " SYS_NUM " TARGET_FMT_lu
                      " cpu %d asid " TARGET_FMT_lx "\n", __func__, env->pc,
                      tlbfill ? env->CSR_TLBRERA : env->CSR_ERA,
                      cause, tlbfill ? "(refill)" : "", env->CSR_ESTAT,
                      env->CSR_ECFG,
                      tlbfill ? env->CSR_TLBRBADV : env->CSR_BADV,
                      env->CSR_BADI, env->gpr[11], cs->cpu_index,
                      env->CSR_ASID);
    }
    cs->exception_index = -1;
}


void loongarch_cpu_set_irq(void *opaque, int irq, int level)
{
    CPULoongArchState *env = opaque;

    if (irq < 0 || irq >= N_IRQS) {
        lsassert(0);
        return;
    }

    env->CSR_ESTAT = deposit64(env->CSR_ESTAT, irq, 1, level != 0);
    if (level) {
        env_cpu(env)->halted = 0;
    }
}

/* Fetch instruction at env->pc, using TLB cache to accelerate address translation. */
static uint32_t fetch(CPULoongArchState *env, INSCache** ic) {
#if defined(CONFIG_USER_ONLY)
        uint32_t insn = ram_lduw(env->pc);
        *ic = cpu_get_ic(env, insn);
        return insn;

#else
    int insn;
    hwaddr ha;
    int prot;
    uint64_t addr = env->pc;
    int tc_index = TC_INDEX(addr);
    TLBCache* tc = env->tc_fetch + tc_index;
    uint64_t page_addr = addr & TARGET_PAGE_MASK;
    if (likely(page_addr == tc->va)) {
        ha = (addr & (TARGET_PAGE_SIZE - 1)) | tc->pa;
    } else {
        int mmu_idx = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PLV) == 0 ? MMU_KERNEL_IDX : MMU_USER_IDX;
        check_get_physical_address(env, &ha, &prot, addr, MMU_INST_FETCH, mmu_idx);
        tc->va = page_addr;
        tc->pa = ha & TARGET_PAGE_MASK;
    }
    insn = ram_lduw(ha);
    *ic = cpu_get_ic(env, insn);
    return insn;
#endif
}

extern uint64_t check_point_pc;
extern bool emu_cpu_check_point;
extern uint64_t check_point_hit_num;
extern uint64_t fetch_num;
extern long debug_print_pc;
extern FILE* CKP_DP_PC;
uint64_t pre_pc = 0;

#if defined(CONFIG_GDB)
extern bool gdb_verbose;
#endif

bool load_bios = false;
uint64_t load_bios_entry = 0x0;

int debug;
FILE* DUMP_DP_PC;
int has_opend = 0;
int check_dump_enable = 0;
uint64_t dbg_prev_pc = 0;

/* Trace PC logging */
static bool trace_enabled __attribute__((unused));
static char trace_log_name[PATH_MAX] __attribute__((unused));
static FILE *trace_file;

/* Debug watch variables -- set via gdb. */
unsigned long watch_pc;
unsigned long watch_era;
unsigned int  enable_watch_gdb_mode = 0;
int dump_enable_arch_mode = 0;
unsigned long watch_sp;


void checkpoint_context(CPULoongArchState *env);

uint64_t test_pc;
int exec_env(CPULoongArchState *env) {
    INSCache* ic;
    current_env = env;
    CPUState* cs = env_cpu(env);
    current_cpu = cs;
    while (1) {
        if (sigsetjmp(env_cpu(env)->jmp_env, 0) == 0) {
            uint32_t insn;
            while(1) {

#if defined (CONFIG_CLI)
                handle_debug_cli(env);
#endif

#if defined (CONFIG_DIFF)
                if (singlestep == 0) {
                    return 0;
                }
#endif

#if !defined (CONFIG_USER_ONLY) && !defined (CONFIG_DIFF)
                loongarch_cpu_check_irq(env);
                if (unlikely(loongarch_cpu_has_irq(env))) {
                    cs->exception_index = EXCCODE_INT;
                    loongarch_cpu_do_interrupt(cs);
                }
#endif

#if defined (CONFIG_GDB)
                if (gdbserver_has_message) {
                    return 1;
                }
                for (int i = 0; i < GDB_FETCCH_BREAKPOINT_NUM; i++) {
                    if (env->pc == gdb_fetch_breakpoint[i]) {
                        if (gdb_verbose)
                        fprintf(stderr, "hit breakpoint %lx\n", env->pc);
                        return 2;
                    }
                }
#endif

                if (unlikely(qemu_loglevel_mask(CPU_LOG_EXEC))) {
                    qemu_log("pc:%lx\n", env->pc);
                }
                if (trace_file) {
                    fprintf(trace_file, "%lx\n", env->pc);
                    fflush(trace_file);
                }
                if (unlikely(qemu_loglevel_mask(CPU_LOG_TB_CPU))) {
                    show_register(env);
                    if (unlikely(qemu_loglevel_mask(CPU_LOG_TB_FPU))) {
                        show_register_fpr(env);
                    }
                }
                test_pc = env->pc;

                if (emu_cpu_check_point && (env->pc == check_point_pc)){
                    fetch_num++;
                    if (fetch_num == check_point_hit_num) {
                        checkpoint_context(env);
                        CKP_DP_PC = fopen("checkpoint_pc.txt", "w");
                        debug_print_pc = 1;
                        printf("\033[1m\033[33mHit CheckPoint : %ld\n", fetch_num);
                    } 
                }

                if ((env->pc == watch_pc)
                    && (env->CSR_ERA == watch_era)
                    && enable_watch_gdb_mode) {
                    debug = 0;
                }

                if (check_dump_enable && !has_opend)
                {
                    DUMP_DP_PC = fopen("dump_pc.txt", "w");
                    has_opend = 1;
                }

                if (has_opend && !check_dump_enable) {
                    fclose(DUMP_DP_PC);
                    has_opend = 0;
                }


                insn = fetch(env, &ic);
#if defined(CONFIG_DIFF) || defined(CONFIG_DIFF_NET)
                env->insn = insn;
                env->prev_pc = env->pc;
#endif
#if defined(CONFIG_PLUGIN)
            if (plugin_ops && plugin_ops->emu_insn_before) {
                plugin_ops->emu_insn_before(env, env->pc, insn);
            }
#endif
                test_pc = env->pc;

                int r = interpreter(env, insn, ic);
                if (!is_la64(env)) env->pc = (uint32_t)env->pc;
                test_pc = env->pc;
                if (debug_print_pc) {
                    if (pre_pc != env->pc) {
                        fprintf(CKP_DP_PC, "%lx\n", env->pc);
                        pre_pc = env->pc;
                    }
                }

                if (check_dump_enable 
                    && (dbg_prev_pc != env->pc)) {
                    fprintf(DUMP_DP_PC, "%lx\n", env->pc);
                    fflush(DUMP_DP_PC);
                    dbg_prev_pc = env->pc;
                }

                if(unlikely(!r)) {
                    qemu_log("ill instruction, pc:%lx insn:%08x\n", env->pc, insn);
                }

#if defined(CONFIG_DIFF_NET)
                if (diffnet_enabled && diffnet_check(env) < 0) {
                    fprintf(stderr, "DIFFNET: halting on mismatch\n");
                    laemu_exit(EXIT_FAILURE);
                }
#endif

#if defined (CONFIG_DIFF) || defined (CONFIG_CLI)
                -- singlestep;
#endif
                env->icount ++;
                PERF_INC(COUNTER_INST);
                if (loongarch_smp_should_yield(env)) {
                    return 3;
                }

                /* Standalone step limit (no diffnet needed) */
                {
                    static long emu_max_inst = -1;
                    if (emu_max_inst < 0) {
                        const char *v = getenv("EMU_MAX_INSTRS");
                        emu_max_inst = v ? atoi(v) : INT64_MAX;
                    }
                    if (emu_max_inst > 0 && env->icount >= emu_max_inst)
                        laemu_exit(0);
                }

            }
        } else {
            loongarch_cpu_do_interrupt(cs);
#if defined(CONFIG_DIFF_NET)
            if (diffnet_enabled) {
                diffnet_step_exception(env);
            }
#endif
            env->ecount ++;
        }
    }
}

G_NORETURN void cpu_loop_exit(CPUState *cpu)
{
    /* Undo the setting in cpu_tb_exec.  */
    cpu->neg.can_do_io = true;
    /* Undo any setting in generated code.  */
    siglongjmp(cpu->jmp_env, 1);
}

void G_NORETURN do_raise_exception(CPULoongArchState *env,
                                   uint32_t exception,
                                   uintptr_t pc)
{
    CPUState *cs = env_cpu(env);
    cpu_clear_tc(env);

    qemu_log_mask(CPU_LOG_INT, "%s: %d (%s)\n",
                  __func__,
                  exception,
                  loongarch_exception_name(exception));
    cs->exception_index = exception;

    cpu_loop_exit(cs);
}

int qemu_loglevel;
FILE* logfile;
void handle_logfile(const char* filename) {
    logfile = fopen(optarg, "w");
    if (!logfile) {
        fprintf(stderr, "can not open logfile %s\n", filename);
        laemu_exit(EXIT_FAILURE);
    }
}

/* define log items */
typedef struct QEMULogItem {
    int mask;
    const char *name;
    const char *help;
} QEMULogItem;

const QEMULogItem qemu_log_items[] = {
    { CPU_LOG_TB_OUT_ASM, "out_asm",
      "show generated host assembly code for each compiled TB" },
    { CPU_LOG_TB_IN_ASM, "in_asm",
      "show target assembly code for each compiled TB" },
    { CPU_LOG_TB_OP, "op",
      "show micro ops for each compiled TB" },
    { CPU_LOG_TB_OP_OPT, "op_opt",
      "show micro ops after optimization" },
    { CPU_LOG_TB_OP_IND, "op_ind",
      "show micro ops before indirect lowering" },
    { CPU_LOG_INT, "int",
      "show interrupts/exceptions in short format" },
    { CPU_LOG_EXEC, "exec",
      "show trace before each executed TB (lots of logs)" },
    { CPU_LOG_TB_CPU, "cpu",
      "show CPU registers before entering a TB (lots of logs)" },
    { CPU_LOG_TB_FPU, "fpu",
      "include FPU registers in the 'cpu' logging" },
    { CPU_LOG_MMU, "mmu",
      "log MMU-related activities" },
    { CPU_LOG_PCALL, "pcall",
      "x86 only: show protected mode far calls/returns/exceptions" },
    { CPU_LOG_RESET, "cpu_reset",
      "show CPU state before CPU resets" },
    { LOG_UNIMP, "unimp",
      "log unimplemented functionality" },
    { LOG_GUEST_ERROR, "guest_errors",
      "log when the guest OS does something invalid (eg accessing a\n"
      "non-existent register)" },
    { CPU_LOG_PAGE, "page",
      "dump pages at beginning of user mode emulation" },
    { CPU_LOG_TB_NOCHAIN, "nochain",
      "do not chain compiled TBs so that \"exec\" and \"cpu\" show\n"
      "complete traces" },
    { CPU_LOG_PLUGIN, "plugin", "output from TCG plugins"},
    { LOG_STRACE, "strace",
      "log every user-mode syscall, its input, and its result" },
    { LOG_PER_THREAD, "tid",
      "open a separate log file per thread; filename must contain '%d'" },
    { CPU_LOG_TB_VPU, "vpu",
      "include VPU registers in the 'cpu' logging" },
    { CPU_LOG_TIMER, "timer",
      "log timer amd timer csr read/write" },
    { CPU_LOG_PTW, "ptw",
      "log Page Table Walker" },
    { 0, NULL, NULL },
};

void handle_logmask(const char* str) {
    const QEMULogItem *item;
    const char *start = str, *p;
    while (1) {
        p = strchr(start, ',');
        if (!p) {
            p = start + strlen(start);
        }
        for (item = qemu_log_items; item->mask != 0; item++) {
            if (strncmp(start, item->name, p - start) == 0) {
                qemu_loglevel |= item->mask;
                break;
            }
        }
        if (item->mask == 0) {
            fprintf(stderr, "unable to prase %s\n", start);
            laemu_exit(EXIT_FAILURE);
        }
        if (*p) {
            start = p + 1;
        } else {
            break;
        }
    };
}

int check_level;
typedef struct CheckItem {
    int mask;
    const char *name;
    const char *help;
} CheckItem;

const CheckItem check_items[] = {
    { CPU_CHECK_TLB_MHIT, "tlb_mhit",
      "check tlb multi-hit when refill tlb" },
    { 0, NULL, NULL },
};

void handle_checkmask(const char* str) {
    const CheckItem *item;
    const char *start = str, *p;
    while (1) {
        p = strchr(start, ',');
        if (!p) {
            p = start + strlen(start);
        }
        for (item = check_items; item->mask != 0; item++) {
            if (strncmp(start, item->name, p - start) == 0) {
                check_level |= item->mask;
                break;
            }
        }
        if (item->mask == 0) {
            fprintf(stderr, "unable to prase %s\n", start);
            laemu_exit(EXIT_FAILURE);
        }
        if (*p) {
            start = p + 1;
        } else {
            break;
        }
    };
}

#if !defined(CONFIG_USER_ONLY)
#if defined(CONFIG_DIFF)
static inline bool is_swiftcore_uart(hwaddr ha)
{
    return ((ha & ~0xfULL) == 0x1FE00000ULL) ||
           ((ha & ~0xfULL) == 0x1FF10000ULL);
}

static uint64_t swiftcore_uart_ld(hwaddr ha, int size)
{
    uint64_t data = 0;

    for (int i = 0; i < size; i++) {
        if (((ha + i) & 0xfULL) == 5) {
            data |= 0x60ULL << (i * 8);
        }
    }

    return data;
}
#endif

void do_io_st(hwaddr ha, uint64_t data, int size) {
    switch (ha)
    {
    case 0x1FF10000 ... 0x1FF11000:
#if defined(CONFIG_DIFF)
        if (is_swiftcore_uart(ha)) {
            break;
        }
#endif
        if (serial_plus) {
            serial_plus_ioport_write(ss, ha, data, size);
        } else {
            serial_ioport_write(NULL, ha, data, size);
        }
        break;
    case 0x1FE00000 ... 0x1FE01000:
#if defined(CONFIG_DIFF)
        if (is_swiftcore_uart(ha)) {
            break;
        }
#endif
        if (serial_plus) {
            serial_plus_ioport_write(ss, ha, data, size);
        } else {
            serial_ioport_write(NULL, ha, data, size);
        }
        break;
    case 0x11FF00000 ... 0x11FF10000:
        if (serial_plus) {
            serial_plus_ioport_write(ss, ha, data, size);
        } else {
            serial_ioport_write(NULL, ha, data, size);
        }
        break;    
    case 0x100d0014:
        if ((data & 0x3c00) == 0x3c00) {
            dump_exec_info(current_env, stderr);
#if defined(CONFIG_PERF)
            perf_report(current_env, stderr);
#endif
            laemu_exit(0);
        }
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "unknown IO store: pa=0x%" PRIx64
                      " size=%d data=0x%" PRIx64 " pc=0x%" PRIx64 "\n",
                      (uint64_t)ha, size, data,
                      current_env ? current_env->pc : 0);
    }
}
uint64_t do_io_ld(hwaddr ha, int size) {
    uint64_t data = 'x';
    switch (ha)
    {
    case 0x1FF10000 ... 0x1FF11000:
#if defined(CONFIG_DIFF)
        if (is_swiftcore_uart(ha)) {
            data = swiftcore_uart_ld(ha, size);
            break;
        }
#endif
        if (serial_plus) {
            data = serial_plus_ioport_read(ss, ha, size);
        } else {
            data = serial_ioport_read(NULL, ha, size);
        }
        break;
    case 0x1FE00000 ... 0x1FE01000:
#if defined(CONFIG_DIFF)
        if (is_swiftcore_uart(ha)) {
            data = swiftcore_uart_ld(ha, size);
            break;
        }
#endif
        if (serial_plus) {
            data = serial_plus_ioport_read(ss, ha, size);
        } else {
            data = serial_ioport_read(NULL, ha, size);
        }
        break;
    case 0x11FF00000 ... 0x11FF10000:
        if (serial_plus) {
            data = serial_plus_ioport_read(ss, ha, size);
        } else {
            data = serial_ioport_read(NULL, ha, size);
        }
        break;
    case 0x100d0014:
        data = 0;
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "unknown IO load: pa=0x%" PRIx64
                      " size=%d pc=0x%" PRIx64 "\n",
                      (uint64_t)ha, size,
                      current_env ? current_env->pc : 0);
        break;
    }
    return data;
}

void loongarch_cpu_check_irq(CPULoongArchState *env) {
    if (determined) {
        env->timer_counter -= (env->CSR_TCFG & CONSTANT_TIMER_ENABLE);
        if (env->timer_counter == 0) {
            env->timer_counter = INT64_MAX;
            loongarch_cpu_set_irq(env, IRQ_TIMER, 1);
            if (FIELD_EX64(env->CSR_TCFG, CSR_TCFG, PERIODIC)) {
                env->timer_counter = (env->CSR_TCFG & CONSTANT_TIMER_TICK_MASK) / TIME_SCALE;
            } else {
                env->CSR_TCFG = FIELD_DP64(env->CSR_TCFG, CSR_TCFG, EN, 0);
            }
        }
    } else {
        if (unlikely(env->timer_int)) {
            env->timer_int = false;
            loongarch_cpu_set_irq(env, IRQ_TIMER, 1);
            if (FIELD_EX64(env->CSR_TCFG, CSR_TCFG, PERIODIC)) {
                cpu_settimer(env, env->CSR_TCFG & CONSTANT_TIMER_TICK_MASK);
            } else {
                env->CSR_TCFG = FIELD_DP64(env->CSR_TCFG, CSR_TCFG, EN, 0);
            }
        }
    }
    if (unlikely(serial_timer_int)) {
        serial_timer_int = false;
        if (serial_plus) {
            serial_check_io(ss);
        } else {
            serial_io_check();
        }
    }
}

bool loongarch_cpu_has_irq(CPULoongArchState *env) {
    return FIELD_EX64(env->CSR_CRMD, CSR_CRMD, IE) && (FIELD_EX64(env->CSR_ESTAT, CSR_ESTAT, IS) & FIELD_EX64(env->CSR_ECFG, CSR_ECFG, LIE));
}
#endif

#ifndef CONFIG_DIFF

int main(int argc, char** argv, char **envp) {
    logfile = stderr;

    /* Handle long options not supported by getopt */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--trace") == 0) {
            trace_enabled = true;
            argv[i] = NULL;
        } else if (strcmp(argv[i], "--trance_log_name") == 0 && i + 1 < argc) {
            strncpy(trace_log_name, argv[i + 1], PATH_MAX - 1);
            trace_log_name[PATH_MAX - 1] = '\0';
            argv[i] = NULL;
            argv[++i] = NULL;
        } else if (strcmp(argv[i], "--smp") == 0 && i + 1 < argc) {
            smp_cpus = atoi(argv[i + 1]);
            if (smp_cpus < 1) {
                smp_cpus = 1;
            }
            argv[i] = NULL;
            argv[++i] = NULL;
        }
    }
    /* Compact argv: remove NULL entries */
    int new_argc = 1;
    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL) {
            argv[new_argc++] = argv[i];
        }
    }
    argc = new_argc;

    if (argc < 2) {
        usage();
    }
    int c;
    while ((c = getopt(argc, argv, "+m:nk:d:c:D:C:E:gzbvwsp:N:R:A:Q:")) != -1) {
        switch (c) {
            case 'm':
                ram_size = atol(optarg) << 30;
                break;
            case 'n':
                new_abi = true;
                break;
            case 'k':
                kernel_filename = optarg;
                break;
            case 'd':
                handle_logmask(optarg);
                break;
            case 'c':
                handle_checkmask(optarg);
                break;
            case 'D':
                handle_logfile(optarg);
                break;
            case 'C':
                sscanf(optarg, "%lx", &check_point_pc);
                emu_cpu_check_point = true;
                break;
            case 'E':
                sscanf(optarg, "%lx", &load_bios_entry);
                break;
            case 'g':
#if !defined (CONFIG_GDB)
                fprintf(stderr, "please make GDB=1\n");
                laemu_exit(0);
#endif
                gdbserver = 1;
                break;
            case 'z':
                determined = 1;
                break;
            case 'v':
#if defined(CONFIG_GDB)
                gdb_verbose = true;
#endif
                break;
            case 'b':
                load_bios = true;
                break;
            case 'w':
                hw_ptw = 1;
                break;
            case 's':
                serial_plus = 1;
                break;
            case 'p':{
#if defined(CONFIG_PLUGIN)
                char * p = strchr(optarg, ',');
                if (!p) {
                    strncpy(plugin_name, optarg, PATH_MAX);
                } else {
                    strncpy(plugin_name, optarg, p - optarg);
                    strncpy(plugin_arg, p + 1, PATH_MAX);
                }
                printf("plugin_name:%s\n", plugin_name);
                printf("plugin_arg:%s\n", plugin_arg);
#else
                fprintf(stderr, "please make PLUGIN=1\n");
                laemu_exit(0);
#endif
            }
                break;
#if defined(CONFIG_DIFF_NET)
            case 'N': {
                char buf[256];
                strncpy(buf, optarg, sizeof(buf) - 1);
                buf[sizeof(buf) - 1] = '\0';
                char *colon = strchr(buf, ':');
                if (colon) {
                    /* host:port  — connect to existing QEMU */
                    *colon = '\0';
                    strncpy(diffnet_host, buf, sizeof(diffnet_host) - 1);
                    diffnet_port = atoi(colon + 1);
                    diffnet_spawn_mode = false;
                } else {
                    /* port only — spawn QEMU automatically */
                    diffnet_port = atoi(buf);
                    diffnet_spawn_mode = true;
                }
                break;
            }
#else
            case 'N':
                fprintf(stderr, "DIFFNET not enabled. Build with DIFF_NET=1\n");
                laemu_exit(EXIT_FAILURE);
                break;
#endif
            case 'R': report_filename = optarg; break;
            case 'A': arch_name = optarg; break;
            case 'Q': kernel_cmdline = optarg; break;
            case '?':
                usage();
                return 1;
            default:
                abort();
        }
    }
    setup_signal();
    insn_stats_init();
#ifndef CONFIG_USER_ONLY
    ram = alloc_ram(ram_size);
    qemu_log("pid:%d, ram_size:%lx kernel_filename:%s\n", getpid(), ram_size, kernel_filename);
#endif
    uint64_t entry_addr;
#if defined(CONFIG_USER_ONLY)
    TARGET_PAGE_SIZE = getpagesize();
    TARGET_PAGE_BITS = ctz64(TARGET_PAGE_SIZE);
    TARGET_PAGE_MASK = ((target_ulong)-1 << TARGET_PAGE_BITS);

    qemu_log_mask(CPU_LOG_PAGE, "TARGET_PAGE_SIZE:%lx TARGET_PAGE_BITS:%ld TARGET_PAGE_MASK:%lx\n", TARGET_PAGE_SIZE, TARGET_PAGE_BITS, TARGET_PAGE_MASK);

    kernel_filename = argv[optind];
    if(!kernel_filename) {
        usage();
        laemu_exit(EXIT_FAILURE);
    }
    load_elf_user(kernel_filename, &entry_addr);
    target_set_brk(info.brk);
    exec_path = kernel_filename;
    if (realpath(exec_path, real_exec_path)) {
        exec_path = real_exec_path;
    }
#else
    if (load_bios) {
        load_binary(kernel_filename, load_bios_entry);
        printf("Load BIOS bin at 0x%lx\n", load_bios_entry);
    } else if (!is_directory(kernel_filename)) {
        load_elf(kernel_filename, &entry_addr);
    }

#if !defined (CONFIG_CLI) && !defined (CONFIG_PLUGIN)
    {
        extern struct termios saved_termios;
        extern int saved_stdin_flags;
        extern bool term_saved;

        saved_stdin_flags = fcntl(STDIN_FILENO, F_GETFL);
        tcgetattr(STDIN_FILENO, &saved_termios);
        term_saved = true;

        struct termios raw = saved_termios;
        raw.c_lflag &= ~(ECHO | ICANON | ISIG);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        fcntl(STDIN_FILENO, F_SETFL, saved_stdin_flags | O_NONBLOCK);
    }
#endif

    timer_t timerid;
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;
    lsassert (timer_create(CLOCK_MONOTONIC, &sev, &timerid) == 0);

#endif
    qemu_log_mask(CPU_LOG_PAGE, "entry_addr:%lx\n", entry_addr);

    CPULoongArchState *env = NULL;
    int create_cpus = 1;
#ifndef CONFIG_USER_ONLY
    create_cpus = smp_cpus;
#endif

    for (int cpu_i = 0; cpu_i < create_cpus; cpu_i++) {
        LoongArchCPU* cpu = aligned_alloc(64, sizeof(LoongArchCPU));
        memset(cpu, 0, sizeof(LoongArchCPU));
        CPUState *cs = CPU(cpu);
        cs->cluster_index = -1;
        cpu_register(cs);

        CPULoongArchState* cpu_env = &cpu->env;
        cs->env = cpu_env;
        cpu_reset(cs);
        if (arch_name) {
            if (strcmp(arch_name, "loongarch32r") == 0) loongarch_la32r_initfn(cpu_env);
            else if (strcmp(arch_name, "loongarch32s") == 0) loongarch_la32s_initfn(cpu_env);
            else if (strcmp(arch_name, "la464") == 0 ||
                     strcmp(arch_name, "loongarch64") == 0) loongarch_la464_initfn(cpu_env);
            else if (strcmp(arch_name, "openc910") == 0) loongarch_openc910_initfn(cpu_env);
            else if (strcmp(arch_name, "swiftcore") == 0) loongarch_swiftcore_initfn(cpu_env);
            else {
                fprintf(stderr, "Unknown arch: %s\n", arch_name);
                loongarch_core_initfn(cpu_env);
            }
        } else {
            loongarch_core_initfn(cpu_env);
        }
        cpu_clear_tc(cpu_env);
        cpu_env->timer_counter = INT64_MAX;

#ifndef CONFIG_USER_ONLY
        if (cpu_i == 0) {
            cpu_env->timerid = timerid;
        } else {
            struct sigevent cpu_sev;
            memset(&cpu_sev, 0, sizeof(cpu_sev));
            cpu_sev.sigev_notify = SIGEV_SIGNAL;
            cpu_sev.sigev_signo = SIGRTMIN;
            cpu_sev.sigev_value.sival_ptr = &cpu_env->timerid;
            lsassert(timer_create(CLOCK_MONOTONIC, &cpu_sev,
                                  &cpu_env->timerid) == 0);
        }
#endif

        if (cpu_i == 0) {
            current_cpu = cs;
            env = cpu_env;
        } else {
            cs->halted = 1;
            cpu_env->pc = 0;
        }
    }
    current_env = env;
    loongarch_smp_init_iocsr();

#ifndef CONFIG_USER_ONLY
    
    {
        qemu_irq irq = qemu_allocate_irq(loongarch_cpu_set_irq, (void*)env, 3);

        if (serial_plus) {
            // ss = simple_serial_init(0x1FF10000, irq, 115200);
            ss = simple_serial_init(0x1FE00000, irq, 115200);
        } else {
            serial_ioport_init(irq);
        }

        struct sigevent sev;
        sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIGRTMIN + 1;
        sev.sigev_value.sival_ptr = &serial_timerid;
        lsassert (timer_create(CLOCK_MONOTONIC, &sev, &serial_timerid) == 0);

        struct sigaction sa;
        memset(&sa, 0, sizeof(struct sigaction));
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = sigaction_entry_serial_timer;
        sa.sa_flags     = SA_SIGINFO | SA_RESTART;
        lsassert (sigaction(SIGRTMIN + 1, &sa, NULL) == 0);

        struct itimerspec its;
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = 5000000;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 5000000;
        lsassert(timer_settime(serial_timerid, 0, &its, NULL) == 0);
    }
#endif
    if (load_bios)
        env->pc = load_bios_entry & 0xffffffff;
    else
        env->pc = entry_addr;

    if (is_directory(kernel_filename)) {
        restore_checkpoint(env, kernel_filename);
        entry_addr = env->pc;
    }

#ifdef CONFIG_CLI
    set_fetch_breakpoint(0, entry_addr);

    if (determined == 0) {
        determined = 1;
        fprintf(stderr, "warn:auto enable -z (Determined events) option in cli mode\n");
    }
#endif

#if defined(CONFIG_USER_ONLY)
    target_ulong sp = user_setup_stack();
    int guest_argc = argc - optind;
    char** guest_argv = argv + optind;
    size_t* guest_argv_len = (size_t*)malloc(sizeof(size_t) * guest_argc);
    target_ulong* guest_argv_addr = (target_ulong*)malloc(sizeof(target_ulong) * guest_argc);
    for (int i = 0; i < guest_argc; i++) {
        guest_argv_len[i] = strlen(guest_argv[i]);
        sp -= (guest_argv_len[i] + 1);
        memcpy((void*)(sp), guest_argv[i], guest_argv_len[i]);
        guest_argv_addr[i] = sp;
    }

    int guest_envc = 0;
    while (envp[guest_envc]) {guest_envc ++;}
    char** guest_envv = envp;
    size_t* guest_envv_len = (size_t*)malloc(sizeof(size_t) * guest_envc);
    target_ulong* guest_envv_addr = (target_ulong*)malloc(sizeof(target_ulong) * guest_envc);
    for (int i = 0; i < guest_envc; i++) {
        guest_envv_len[i] = strlen(guest_envv[i]);
        sp -= (guest_envv_len[i] + 1);
        memcpy((void*)(sp), guest_envv[i], guest_envv_len[i]);
        guest_envv_addr[i] = sp;
    }
    sp = QEMU_ALIGN_DOWN(sp, 16);

    sp -= 16;
    abi_ulong u_rand_bytes = sp;
    ram_std(sp+0, 0x0011223344556677);
    ram_std(sp+8, 0x8899aabbccddeeff);

    if ((guest_argc + guest_envc) % 2 == 0) {
        sp -=8;
    }

    sp -= 16; ram_std(sp, AT_RANDOM);ram_std(sp+8, u_rand_bytes);
    sp -= 16; ram_std(sp, AT_PHDR);  ram_std(sp+8, (abi_ulong)(info.load_addr + e_phoff));
    sp -= 16; ram_std(sp, AT_PHENT); ram_std(sp+8, (abi_ulong)(sizeof (elf_phdr)));
    sp -= 16; ram_std(sp, AT_PHNUM); ram_std(sp+8, (abi_ulong)(e_phnum));
    sp -= 16; ram_std(sp, AT_PAGESZ);ram_std(sp+8, TARGET_PAGE_SIZE);
    sp -= 16; ram_std(sp, AT_UID);   ram_std(sp+8, (abi_ulong) getuid());
    sp -= 16; ram_std(sp, AT_EUID);  ram_std(sp+8, (abi_ulong) geteuid());
    sp -= 16; ram_std(sp, AT_GID);   ram_std(sp+8, (abi_ulong) getgid());
    sp -= 16; ram_std(sp, AT_EGID);  ram_std(sp+8, (abi_ulong) getegid());

    qemu_log_mask(CPU_LOG_PAGE, "aux addr %lx\n", sp);

    target_ulong guest_arg_size = ( 1 + guest_argc + 1 + guest_envc + 1) * 8;
    sp -= guest_arg_size;
    ram_std(sp, guest_argc);
    for (int i = 0; i < guest_argc; i++) {
        ram_std(sp + 8 + (i * 8), guest_argv_addr[i]);
    }
    for (int i = 0; i < guest_envc; i++) {
        ram_std(sp + (1 + guest_argc + 1) * 8 +(i * 8), guest_envv_addr[i]);
    }
    env->gpr[3] = sp;
    qemu_log_mask(CPU_LOG_PAGE, "init sp %lx\n", sp);
#endif
    current_env = env;
#if defined(CONFIG_PLUGIN)
    if (plugin_name[0]) {
        void* plugin_handle = dlopen(plugin_name, RTLD_LAZY);
        if (!plugin_handle) {
            fprintf(stderr, "%s\n", dlerror());
            laemu_exit(EXIT_FAILURE);
        }
        dlerror();

        la_emu_plugin_install_func_t install_func = dlsym(plugin_handle, "la_emu_plugin_install");
        char *error;
        if ((error = dlerror()) != NULL)  {
            fprintf(stderr, "%s\n", error);
            laemu_exit(EXIT_FAILURE);
        }
        plugin_ops = install_func(plugin_arg);
        if (plugin_ops && plugin_ops->emu_start) {
            plugin_ops->emu_start();
        }
    }
#endif
    if (trace_enabled) {
        const char *fname = trace_log_name[0] ? trace_log_name : "trace_pc.log";
        trace_file = fopen(fname, "w");
        if (!trace_file) {
            fprintf(stderr, "cannot open trace log %s\n", fname);
            laemu_exit(EXIT_FAILURE);
        }
        setvbuf(trace_file, NULL, _IONBF, 0);
        fprintf(stderr, "trace: logging PC to %s\n", fname);
    }
    if (gdbserver) {
#if defined(CONFIG_GDB)
#ifndef CONFIG_USER_ONLY
        if (!hw_ptw) {
            fprintf(stderr, "Enable hw_ptw options -w for better debug\n");
            fprintf(stderr, "Enable hw_ptw options -w for better debug\n");
            fprintf(stderr, "Enable hw_ptw options -w for better debug\n");
            sleep(3);
        }
#endif
        gdbserver_init(1239);
        gdbserver_has_message = 1;
        gdbserver_loop();
#endif
    } else {
#if defined(CONFIG_DIFF_NET)
        if (diffnet_port > 0) {
            int ret;
            if (diffnet_spawn_mode) {
                ret = diffnet_spawn(DIFFNET_QEMU_PATH, kernel_filename,
                                    load_bios_entry, diffnet_port, env);
            } else {
                ret = diffnet_init(diffnet_host, diffnet_port, env);
            }
            if (ret < 0) {
                laemu_exit(EXIT_FAILURE);
            }
            diffnet_enabled = true;
        }
#endif
#ifndef CONFIG_USER_ONLY
        if (loongarch_smp_cpu_count() > 1) {
            loongarch_smp_run();
        } else
#endif
        {
            exec_env(env);
        }
    }
    if (trace_file) {
        fclose(trace_file);
        trace_file = NULL;
    }
#if defined(CONFIG_DIFF_NET)
    diffnet_cleanup();
#endif
    fprintf(stderr, "end from main %s %d\n", __FILE__, __LINE__);
    return 0;
}

#endif
