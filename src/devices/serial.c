/* LoongArch emu — simple NS16550 UART (no FIFO). */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "serial.h"

#define UART_LCR_DLAB   0x80
#define UART_IER_MSI    0x08
#define UART_IER_RLSI   0x04
#define UART_IER_THRI   0x02
#define UART_IER_RDI    0x01

#define UART_IIR_NO_INT 0x01
#define UART_IIR_ID     0x06
#define UART_IIR_MSI    0x00
#define UART_IIR_THRI   0x02
#define UART_IIR_RDI    0x04
#define UART_IIR_RLSI   0x06
#define UART_IIR_CTI    0x0C
#define UART_IIR_FENF   0x80
#define UART_IIR_FE     0xC0

#define UART_MCR_LOOP   0x10
#define UART_MCR_OUT2   0x08
#define UART_MCR_OUT1   0x04
#define UART_MCR_RTS    0x02
#define UART_MCR_DTR    0x01

#define UART_MSR_DCD    0x80
#define UART_MSR_RI     0x40
#define UART_MSR_DSR    0x20
#define UART_MSR_CTS    0x10
#define UART_MSR_DDCD   0x08
#define UART_MSR_TERI   0x04
#define UART_MSR_DDSR   0x02
#define UART_MSR_DCTS   0x01

#define UART_LSR_TEMT   0x40
#define UART_LSR_THRE   0x20
#define UART_LSR_BI     0x10
#define UART_LSR_FE     0x08
#define UART_LSR_PE     0x04
#define UART_LSR_OE     0x02
#define UART_LSR_DR     0x01

#define UART_FCR_ITL_1  0x00
#define UART_FCR_ITL_2  0x40
#define UART_FCR_ITL_3  0x80
#define UART_FCR_ITL_4  0xC0
#define UART_FCR_DMS    0x08
#define UART_FCR_XFR    0x04
#define UART_FCR_RFR    0x02
#define UART_FCR_FE     0x01

typedef struct SerialState {
    uint16_t divider;
    uint8_t  rbr, thr, tsr, ier, iir, lcr, mcr, lsr, msr, scr, fcr, fcr_vmstate;
    int      thr_ipending, last_break_enable;
} SerialState;

static SerialState s;
static char   input = 'x';
static bool   input_valid;
static qemu_irq serial_irq;
static bool host_escape_pending;

extern void laemu_host_quit(void);

bool serial_handle_host_input(uint8_t *ch)
{
    if (host_escape_pending) {
        host_escape_pending = false;
        if (*ch == 'x' || *ch == 'X') {
            laemu_host_quit();
        }
        if (*ch == 0x01) {
            return true;
        }
        return false;
    }

    if (*ch == 0x01) {
        host_escape_pending = true;
        return false;
    }

    return true;
}

static void serial_update_irq(void)
{
    if (serial_irq && (s.ier & UART_IER_RDI) && input_valid) {
        s.iir = UART_IIR_RDI;
        qemu_irq_raise(serial_irq);
    } else {
        if (!input_valid && (s.iir & UART_IIR_ID) == UART_IIR_RDI) {
            s.iir = UART_IIR_NO_INT;
        }
        qemu_irq_lower(serial_irq);
    }
}

void serial_ioport_init(qemu_irq irq)
{
    serial_irq = irq;
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
    s.lsr = UART_LSR_TEMT | UART_LSR_THRE;
    s.iir = UART_IIR_NO_INT;
    s.msr = UART_MSR_DCD | UART_MSR_DSR | UART_MSR_CTS;
    qemu_irq_lower(serial_irq);
}

static void try_read(void)
{
    if (!input_valid) {
        uint8_t ch;
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            if (serial_handle_host_input(&ch)) {
                input = ch;
                input_valid = true;
            }
        }
    }
    serial_update_irq();
}

void serial_io_check(void)
{
    try_read();
}

uint64_t serial_ioport_read(void *opaque, long addr, unsigned size)
{
    (void)opaque; (void)size;
    try_read();
    addr &= 7;
    switch (addr) {
    case 0:
        if (s.lcr & UART_LCR_DLAB) return s.divider & 0xff;
        { uint8_t c = input; input_valid = false; serial_update_irq(); return c; }
    case 1:
        return (s.lcr & UART_LCR_DLAB) ? (s.divider >> 8) & 0xff : s.ier;
    case 2:
        if (input_valid) s.iir = UART_IIR_RDI;
        else if ((s.iir & UART_IIR_ID) == UART_IIR_THRI) s.thr_ipending = 0;
        return s.iir;
    case 3:
    case 4:
    case 7:
        return 0;
    case 5:
        return s.lsr | (input_valid ? UART_LSR_DR : 0);
    case 6:
        return UART_MSR_DCD | UART_MSR_DSR | UART_MSR_CTS;
    default:
        assert(0);
    }
}

void serial_ioport_write(void *opaque, long addr, uint64_t val, unsigned size)
{
    (void)opaque; (void)size;
    try_read();
    addr &= 7;
    switch (addr) {
    case 0:
        if (s.lcr & UART_LCR_DLAB) {
            s.divider = (s.divider & 0xff00) | (val & 0xff);
        } else {
            fprintf(stderr, "%c", (char)val);
            fflush(stderr);
            s.lsr |= UART_LSR_TEMT | UART_LSR_THRE;
            if (input_valid) { s.iir |= UART_IIR_RDI; s.thr_ipending = 0; }
            else             { s.iir |= UART_IIR_THRI; s.thr_ipending = 1; }
        }
        break;
    case 1:
        if (s.lcr & UART_LCR_DLAB)
            s.divider = (s.divider & 0xff) | ((val << 8) & 0xff00);
        else {
            s.ier = val & 0x0f;
            serial_update_irq();
        }
        break;
    case 2:
        s.fcr = val & 0xC9;
        break;
    case 3:
        s.lcr = val;
        s.last_break_enable = (val >> 6) & 1;
        break;
    case 4:
        s.mcr = val & 0x1f;
        break;
    default:
        break;
    }
}
