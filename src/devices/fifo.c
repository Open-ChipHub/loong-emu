/*
 * Generic FIFO component — circular buffer.
 * Copyright (c) 2012 Peter A. G. Crosthwaite
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "qemu/osdep.h"
#include "qemu/fifo8.h"

#define g_new(type, num)  (type*)malloc(sizeof(type) * num)
#define g_free(ptr)       free(ptr)

void fifo8_create(Fifo8 *fifo, uint32_t capacity)
{
    fifo->data = g_new(uint8_t, capacity);
    fifo->capacity = capacity;
    fifo->head = 0;
    fifo->num = 0;
}

void fifo8_destroy(Fifo8 *fifo)
{
    g_free(fifo->data);
}

void fifo8_push(Fifo8 *fifo, uint8_t data)
{
    assert(fifo->num < fifo->capacity);
    fifo->data[(fifo->head + fifo->num) % fifo->capacity] = data;
    fifo->num++;
}

void fifo8_push_all(Fifo8 *fifo, const uint8_t *data, uint32_t num)
{
    assert(fifo->num + num <= fifo->capacity);
    uint32_t start = (fifo->head + fifo->num) % fifo->capacity;

    if (start + num <= fifo->capacity) {
        memcpy(&fifo->data[start], data, num);
    } else {
        uint32_t avail = fifo->capacity - start;
        memcpy(&fifo->data[start], data, avail);
        memcpy(&fifo->data[0], &data[avail], num - avail);
    }
    fifo->num += num;
}

uint8_t fifo8_pop(Fifo8 *fifo)
{
    assert(fifo->num > 0);
    uint8_t ret = fifo->data[fifo->head++];
    fifo->head %= fifo->capacity;
    fifo->num--;
    return ret;
}

static const uint8_t *fifo8_peekpop_buf(Fifo8 *fifo, uint32_t max,
                                        uint32_t *numptr, bool do_pop)
{
    assert(max > 0 && max <= fifo->num);
    uint32_t num = MIN(fifo->capacity - fifo->head, max);
    uint8_t *ret = &fifo->data[fifo->head];

    if (do_pop) {
        fifo->head += num;
        fifo->head %= fifo->capacity;
        fifo->num -= num;
    }
    if (numptr) *numptr = num;
    return ret;
}

const uint8_t *fifo8_peek_buf(Fifo8 *fifo, uint32_t max, uint32_t *numptr)
{ return fifo8_peekpop_buf(fifo, max, numptr, false); }

const uint8_t *fifo8_pop_buf(Fifo8 *fifo, uint32_t max, uint32_t *numptr)
{ return fifo8_peekpop_buf(fifo, max, numptr, true); }

void fifo8_reset(Fifo8 *fifo)
{ fifo->num = 0; fifo->head = 0; }

bool fifo8_is_empty(Fifo8 *fifo)
{ return fifo->num == 0; }

bool fifo8_is_full(Fifo8 *fifo)
{ return fifo->num == fifo->capacity; }

uint32_t fifo8_num_free(Fifo8 *fifo)
{ return fifo->capacity - fifo->num; }

uint32_t fifo8_num_used(Fifo8 *fifo)
{ return fifo->num; }
