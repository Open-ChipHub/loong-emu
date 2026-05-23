/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * QRSP — Minimal GDB Remote Serial Protocol client library
 *
 * Pure C, zero external dependencies beyond POSIX sockets.
 * Used by loong64-emu to connect to QEMU's GDB stub for
 * instruction-level differential testing.
 */
#ifndef QRSP_H
#define QRSP_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct qrsp_conn {
    int fd;
    char recv_buf[8192];
    int  recv_len;
} qrsp_conn_t;

/* Connect to host:port. Returns 0 on success, -1 on error. */
int  qrsp_connect(qrsp_conn_t *conn, const char *host, int port);

/* Close connection. */
void qrsp_close(qrsp_conn_t *conn);

/* Send a GDB RSP packet: wraps payload with $...#XX, waits for '+'.
 * Returns 0 on success, -1 on error. */
int  qrsp_send_packet(qrsp_conn_t *conn, const char *data, int len);

/* Receive one GDB RSP packet (payload only). Stops at '#', verifies checksum,
 * sends '+' or '-'. Returns payload length on success, -1 on error. */
int  qrsp_recv_packet(qrsp_conn_t *conn, char *buf, int buf_size);

/* ---- High-level helpers ---- */

/* Single-step: send "s", consume stop-reply. Returns 0 on success. */
int  qrsp_step(qrsp_conn_t *conn);

/* Read general registers ("g" packet). Decodes hex to binary.
 * bin_buf must be >= 280 bytes (35 registers * 8 bytes each).
 * Returns 0 on success, -1 on error. */
int  qrsp_read_g_packet(qrsp_conn_t *conn, uint8_t *bin_buf, int buf_size);

/* Continue execution until breakpoint or exit ("c" packet). */
int  qrsp_continue(qrsp_conn_t *conn);

/* Set ("Z") or remove ("z") a breakpoint/watchpoint.
 * kind: 0=software, 1=hardware, 2=write, 3=read, 4=access */
int  qrsp_set_breakpoint(qrsp_conn_t *conn, uint64_t addr, int kind);
int  qrsp_remove_breakpoint(qrsp_conn_t *conn, uint64_t addr, int kind);

/* Read single register via 'p' packet. Returns byte count on success, -1 on error.
 * buf must be large enough for the register (max 32 bytes for LASX). */
int  qrsp_read_register(qrsp_conn_t *conn, int regnum, uint8_t *buf, int buf_size);

/* Read memory via 'm' packet. Returns bytes read on success, -1 on error. */
int  qrsp_read_memory(qrsp_conn_t *conn, uint64_t addr, uint8_t *buf, int len);

/* Write memory via 'M' packet. Returns 0 on success, -1 on error. */
int  qrsp_write_memory(qrsp_conn_t *conn, uint64_t addr, const uint8_t *buf, int len);

/* Utility: decode a 16-char hex string to uint64_t (little-endian). */
uint64_t qrsp_hex_decode_le64(const char *hex);

#ifdef __cplusplus
}
#endif

#endif /* QRSP_H */
