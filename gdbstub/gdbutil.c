
#include <qemu/osdep.h>
#include "cpu.h"

int gdb_breakpoint_check(CPUState *cs, int type, vaddr addr);


#define HELP_FUNC_UP_ADDR (0xFF)
#define HELP_FUNC_DW_ADDR (0xA0)

extern int check_dump_enable;

extern int dump_enable_arch_mode;

int gdb_breakpoint_check(CPUState *cs, int type, vaddr addr){

    if (((addr > HELP_FUNC_DW_ADDR) && (addr < HELP_FUNC_UP_ADDR)))
        goto check_low;
    else if (((addr >> 48) & 0xfff))
        goto check_high;
    else
        return 0;

check_low:
    /// Enable RISC-V 
    if (addr == 0xB0) {
        check_dump_enable = !check_dump_enable;
        dump_enable_arch_mode = 1;
    }

    /// Enable LoongArch 
    if (addr == 0xB1) {
        check_dump_enable = !check_dump_enable;
        dump_enable_arch_mode = 0;
    }

    extern unsigned int  enable_watch_gdb_mode;
    if (addr == 0xB3) {
        enable_watch_gdb_mode = 1;
    }
    goto check_ok;

    extern unsigned long watch_pc;
    extern unsigned long watch_sp;
    extern unsigned long watch_era;

check_high:
    /// Enable LoongArch 
    if (((addr >> 48) & 0xffff) == 0x9001) {
        watch_pc = addr & 0xf000ffffffffffff;
    }

    if (((addr >> 48) & 0xffff) == 0x9002) {
        watch_sp = addr & 0xf000ffffffffffff;
    }

    if (((addr >> 48) & 0xffff) == 0x9003) {
        watch_era = addr & 0xf000ffffffffffff;
    }

check_ok:
    return 1;
}
