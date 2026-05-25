#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "irq.h"

void serial_ioport_init(qemu_irq irq);
void serial_io_check(void);
uint64_t serial_ioport_read(void *opaque, long addr, unsigned size);
void serial_ioport_write(void *opaque, long addr, uint64_t val, unsigned size);

#endif
