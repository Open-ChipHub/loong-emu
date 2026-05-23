/*
 * 16550A UART emulation (with FIFO).
 * Copyright (c) 2003-2004 Fabrice Bellard, 2008 Citrix Systems, Inc.
 * SPDX-License-Identifier: MIT
 */
#include "serial_plus.h"
#include "cpu.h"

#define UART_LCR_DLAB    0x80
#define UART_IER_MSI     0x08
#define UART_IER_RLSI    0x04
#define UART_IER_THRI    0x02
#define UART_IER_RDI     0x01

#define UART_IIR_NO_INT  0x01
#define UART_IIR_ID      0x06
#define UART_IIR_MSI     0x00
#define UART_IIR_THRI    0x02
#define UART_IIR_RDI     0x04
#define UART_IIR_RLSI    0x06
#define UART_IIR_CTI     0x0C
#define UART_IIR_FENF    0x80
#define UART_IIR_FE      0xC0

#define UART_MCR_LOOP    0x10
#define UART_MCR_OUT2    0x08
#define UART_MCR_OUT1    0x04
#define UART_MCR_RTS     0x02
#define UART_MCR_DTR     0x01

#define UART_MSR_DCD     0x80
#define UART_MSR_RI      0x40
#define UART_MSR_DSR     0x20
#define UART_MSR_CTS     0x10
#define UART_MSR_DDCD    0x08
#define UART_MSR_TERI    0x04
#define UART_MSR_DDSR    0x02
#define UART_MSR_DCTS    0x01
#define UART_MSR_ANY_DELTA 0x0F

#define UART_LSR_TEMT    0x40
#define UART_LSR_THRE    0x20
#define UART_LSR_BI      0x10
#define UART_LSR_FE      0x08
#define UART_LSR_PE      0x04
#define UART_LSR_OE      0x02
#define UART_LSR_DR      0x01
#define UART_LSR_INT_ANY 0x1E

#define UART_FCR_ITL_1   0x00
#define UART_FCR_ITL_2   0x40
#define UART_FCR_ITL_3   0x80
#define UART_FCR_ITL_4   0xC0
#define UART_FCR_DMS     0x08
#define UART_FCR_XFR     0x04
#define UART_FCR_RFR     0x02
#define UART_FCR_FE      0x01

#define MAX_XMIT_RETRY   4

#ifdef DEBUG_SERIAL
#define DPRINTF(fmt, ...) \
    do { fprintf(stderr, "serial: " fmt, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) do {} while (0)
#endif

static void serial_receive1(void *opaque, const uint8_t *buf, int size);
static void serial_xmit(SerialState *s);

static inline void recv_fifo_put(SerialState *s, uint8_t chr)
{
    if (!fifo8_is_full(&s->recv_fifo))
        fifo8_push(&s->recv_fifo, chr);
    else
        s->lsr |= UART_LSR_OE;
}

static void serial_update_irq(SerialState *s)
{
    uint8_t tmp_iir = UART_IIR_NO_INT;

    if      ((s->ier & UART_IER_RLSI) && (s->lsr & UART_LSR_INT_ANY))
        tmp_iir = UART_IIR_RLSI;
    else if ((s->ier & UART_IER_RDI) && s->timeout_ipending)
        tmp_iir = UART_IIR_CTI;
    else if ((s->ier & UART_IER_RDI) && (s->lsr & UART_LSR_DR) &&
             (!(s->fcr & UART_FCR_FE) || s->recv_fifo.num >= s->recv_fifo_itl))
        tmp_iir = UART_IIR_RDI;
    else if ((s->ier & UART_IER_THRI) && s->thr_ipending)
        tmp_iir = UART_IIR_THRI;
    else if ((s->ier & UART_IER_MSI) && (s->msr & UART_MSR_ANY_DELTA))
        tmp_iir = UART_IIR_MSI;

    s->iir = tmp_iir | (s->iir & 0xF0);
    if (tmp_iir != UART_IIR_NO_INT)
        qemu_irq_raise(s->irq);
    else
        qemu_irq_lower(s->irq);
}

static void serial_update_msl(SerialState *s)
{
    uint8_t omsr = s->msr;
    s->msr = (s->msr & 0xF0) | (UART_MSR_DCD | UART_MSR_DSR | UART_MSR_CTS);
    if (s->msr != omsr) {
        s->msr |= ((s->msr >> 4) ^ (omsr >> 4));
        if ((s->msr & UART_MSR_TERI) && !(omsr & UART_MSR_RI))
            s->msr &= ~UART_MSR_TERI;
        serial_update_irq(s);
    }
}

static void serial_xmit(SerialState *s)
{
    do {
        assert(!(s->lsr & UART_LSR_TEMT));
        if (s->tsr_retry == 0) {
            assert(!(s->lsr & UART_LSR_THRE));
            if (s->fcr & UART_FCR_FE) {
                assert(!fifo8_is_empty(&s->xmit_fifo));
                s->tsr = fifo8_pop(&s->xmit_fifo);
                if (!s->xmit_fifo.num) s->lsr |= UART_LSR_THRE;
            } else {
                s->tsr = s->thr;
                s->lsr |= UART_LSR_THRE;
            }
            if ((s->lsr & UART_LSR_THRE) && !s->thr_ipending) {
                s->thr_ipending = 1;
                serial_update_irq(s);
            }
        }
        if (s->mcr & UART_MCR_LOOP) {
            serial_receive1(s, &s->tsr, 1);
        } else {
            while (write(STDOUT_FILENO, &s->tsr, 1) != 1) {
                if (errno != EINTR) {
                    fprintf(stderr, "serial: write failed: %s\n", strerror(errno));
                    break;
                }
            }
        }
        s->tsr_retry = 0;
    } while (!(s->lsr & UART_LSR_THRE));
    s->last_xmit_ts = 0;
    s->lsr |= UART_LSR_TEMT;
}

static void serial_write_fcr(SerialState *s, uint8_t val)
{
    s->fcr = val;
    if (val & UART_FCR_FE) {
        s->iir |= UART_IIR_FE;
        switch (val & 0xC0) {
        case UART_FCR_ITL_1: s->recv_fifo_itl = 1;  break;
        case UART_FCR_ITL_2: s->recv_fifo_itl = 4;  break;
        case UART_FCR_ITL_3: s->recv_fifo_itl = 8;  break;
        case UART_FCR_ITL_4: s->recv_fifo_itl = 14; break;
        }
    } else {
        s->iir &= ~UART_IIR_FE;
    }
}

void serial_plus_ioport_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    SerialState *s = (SerialState *)opaque;
    addr &= 7;

    switch (addr) {
    case 0:
        if (s->lcr & UART_LCR_DLAB) {
            s->divider = (size == 1) ? (s->divider & 0xff00) | val : val;
        } else {
            s->thr = (uint8_t)val;
            if (s->fcr & UART_FCR_FE) {
                if (fifo8_is_full(&s->xmit_fifo)) fifo8_pop(&s->xmit_fifo);
                fifo8_push(&s->xmit_fifo, s->thr);
            }
            s->thr_ipending = 0;
            s->lsr &= ~(UART_LSR_THRE | UART_LSR_TEMT);
            serial_update_irq(s);
            if (s->tsr_retry == 0) serial_xmit(s);
        }
        break;
    case 1:
        if (s->lcr & UART_LCR_DLAB) {
            s->divider = (s->divider & 0x00ff) | (val << 8);
        } else {
            uint8_t changed = (s->ier ^ val) & 0x0f;
            s->ier = val & 0x0f;
            if ((changed & UART_IER_MSI) && s->poll_msl >= 0)
                s->poll_msl = (s->ier & UART_IER_MSI) ? 1 : 0;
            if (changed & UART_IER_THRI) {
                s->thr_ipending = ((s->ier & UART_IER_THRI) && (s->lsr & UART_LSR_THRE)) ? 1 : 0;
            }
            if (changed) serial_update_irq(s);
        }
        break;
    case 2:
        if ((val ^ s->fcr) & UART_FCR_FE)
            val |= UART_FCR_XFR | UART_FCR_RFR;
        if (val & UART_FCR_RFR) {
            s->lsr &= ~(UART_LSR_DR | UART_LSR_BI);
            s->timeout_ipending = 0;
            fifo8_reset(&s->recv_fifo);
        }
        if (val & UART_FCR_XFR) {
            s->lsr |= UART_LSR_THRE;
            s->thr_ipending = 1;
            fifo8_reset(&s->xmit_fifo);
        }
        serial_write_fcr(s, val & 0xC9);
        serial_update_irq(s);
        break;
    case 3:
        s->lcr = val;
        s->last_break_enable = (val >> 6) & 1;
        break;
    case 4:
        s->mcr = val & 0x1f;
        break;
    case 7:
        s->scr = val;
        break;
    default:
        break;
    }
}

uint64_t serial_plus_ioport_read(void *opaque, hwaddr addr, unsigned size)
{
    SerialState *s = (SerialState *)opaque;
    uint32_t ret = 0;
    addr &= 7;

    switch (addr) {
    case 0:
        if (s->lcr & UART_LCR_DLAB) {
            ret = s->divider & 0xff;
        } else if (s->fcr & UART_FCR_FE) {
            ret = fifo8_is_empty(&s->recv_fifo) ? 0 : fifo8_pop(&s->recv_fifo);
            if (s->recv_fifo.num == 0)
                s->lsr &= ~(UART_LSR_DR | UART_LSR_BI);
            s->timeout_ipending = 0;
        } else {
            ret = s->rbr;
            s->lsr &= ~(UART_LSR_DR | UART_LSR_BI);
        }
        serial_update_irq(s);
        break;
    case 1:
        ret = (s->lcr & UART_LCR_DLAB) ? (s->divider >> 8) & 0xff : s->ier;
        break;
    case 2:
        ret = s->iir;
        if ((ret & UART_IIR_ID) == UART_IIR_THRI) {
            s->thr_ipending = 0;
            serial_update_irq(s);
        }
        break;
    case 3: ret = s->lcr; break;
    case 4: ret = s->mcr; break;
    case 5:
        ret = s->lsr;
        if (s->lsr & (UART_LSR_BI | UART_LSR_OE)) {
            s->lsr &= ~(UART_LSR_BI | UART_LSR_OE);
            serial_update_irq(s);
        }
        break;
    case 6:
        if (s->mcr & UART_MCR_LOOP) {
            ret = (s->mcr & 0x0c) << 4;
            ret |= (s->mcr & 0x02) << 3;
            ret |= (s->mcr & 0x01) << 5;
        } else {
            if (s->poll_msl >= 0) serial_update_msl(s);
            ret = s->msr;
            if (s->msr & UART_MSR_ANY_DELTA) {
                s->msr &= 0xF0;
                serial_update_irq(s);
            }
        }
        break;
    case 7: ret = s->scr; break;
    }
    return ret;
}

static int serial_can_receive(SerialState *s)
{
    if (s->fcr & UART_FCR_FE) {
        if (s->recv_fifo.num < UART_FIFO_LENGTH)
            return (s->recv_fifo.num <= s->recv_fifo_itl) ?
                   s->recv_fifo_itl - s->recv_fifo.num : 1;
        return 0;
    }
    return !(s->lsr & UART_LSR_DR);
}

static void fifo_timeout_int(void *opaque)
{
    SerialState *s = (SerialState *)opaque;
    if (s->recv_fifo.num) {
        s->timeout_ipending = 1;
        serial_update_irq(s);
    }
}

static void serial_receive1(void *opaque, const uint8_t *buf, int size)
{
    SerialState *s = (SerialState *)opaque;
    if (s->fcr & UART_FCR_FE) {
        for (int i = 0; i < size; i++)
            recv_fifo_put(s, buf[i]);
        s->lsr |= UART_LSR_DR;
    } else {
        if (s->lsr & UART_LSR_DR) s->lsr |= UART_LSR_OE;
        s->rbr = buf[0];
        s->lsr |= UART_LSR_DR;
    }
    serial_update_irq(s);
}

static void serial_reset(void *opaque)
{
    SerialState *s = (SerialState *)opaque;
    s->watch_tag = 0;
    s->rbr = 0;
    s->ier = 0;
    s->iir = UART_IIR_NO_INT;
    s->lcr = 0;
    s->lsr = UART_LSR_TEMT | UART_LSR_THRE;
    s->msr = UART_MSR_DCD | UART_MSR_DSR | UART_MSR_CTS;
    s->divider = 0x0C;
    s->mcr = UART_MCR_OUT2;
    s->scr = 0;
    s->tsr_retry = 0;
    s->char_transmit_time = (NANOSECONDS_PER_SECOND / 9600) * 10;
    s->poll_msl = 0;
    s->timeout_ipending = 0;
    fifo8_reset(&s->recv_fifo);
    fifo8_reset(&s->xmit_fifo);
    s->last_xmit_ts = 0;
    s->thr_ipending = 0;
    s->last_break_enable = 0;
    qemu_irq_lower(s->irq);
    serial_update_msl(s);
    s->msr &= ~UART_MSR_ANY_DELTA;
}

void simple_serial_realize_core(SerialState *s)
{
    fifo8_create(&s->recv_fifo, UART_FIFO_LENGTH);
    fifo8_create(&s->xmit_fifo, UART_FIFO_LENGTH);
    serial_reset(s);
}

void simple_serial_exit_core(SerialState *s)
{
    fifo8_destroy(&s->recv_fifo);
    fifo8_destroy(&s->xmit_fifo);
}

SerialState *simple_serial_init(unsigned long base, qemu_irq irq, int baudbase)
{
    SerialState *s = (SerialState *)malloc(sizeof(SerialState));
    s->irq = irq;
    s->baudbase = baudbase;
    simple_serial_realize_core(s);
    s->infd = 0;
    return s;
}

SerialState *simple_serial_restore(int base, qemu_irq irq, int baudbase, const char *filename)
{
    SerialState *s = (SerialState *)malloc(sizeof(SerialState));
    s->irq = irq;
    s->baudbase = baudbase;

    FILE *f = fopen(filename, "rb");
    if (!f) { perror(filename); laemu_exit(1); }

    uint64_t count;
    assert(fread(&count, sizeof(uint64_t), 1, f) == 1);
    assert(count == offsetof(SerialState, irq) + 2 * UART_FIFO_LENGTH);
    assert(fread((void *)s, offsetof(SerialState, irq), 1, f) == 1);

    s->recv_fifo.data = (uint8_t *)malloc(UART_FIFO_LENGTH);
    s->xmit_fifo.data = (uint8_t *)malloc(UART_FIFO_LENGTH);
    assert(fread(s->recv_fifo.data, UART_FIFO_LENGTH, 1, f) == 1);
    assert(fread(s->xmit_fifo.data, UART_FIFO_LENGTH, 1, f) == 1);
    fclose(f);

    s->infd = 0;
    return s;
}

void serial_check_io(SerialState *s)
{
    while (serial_can_receive(s)) {
        uint8_t c;
        int cnt = read(s->infd, &c, 1);
        if (cnt == -1) {
            if (errno == EAGAIN) return;
            fprintf(stderr, "serial: unexpected read error\n");
            abort();
        } else if (cnt == 0) {
            abort();
        } else if (cnt == 1) {
            serial_receive1(s, &c, 1);
            fifo_timeout_int(s);
        }
    }
}
