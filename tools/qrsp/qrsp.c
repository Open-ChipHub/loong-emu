/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * QRSP — Minimal GDB Remote Serial Protocol client library
 *
 * Implements the GDB RSP wire protocol:
 *   Packet format: $<payload>#<c1><c2>
 *   ACK:  '+'
 *   NAK:  '-'
 *
 * Reference: https://sourceware.org/gdb/onlinedocs/gdb/Remote-Protocol.html
 */

#include "qrsp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

/* ---- internal helpers ---- */

static inline int fromhex(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static uint8_t rsp_checksum(const char *data, int len)
{
    uint8_t sum = 0;
    for (int i = 0; i < len; i++) {
        sum += (uint8_t)data[i];
    }
    return sum;
}

static int recv_exact(qrsp_conn_t *conn, void *buf, int n)
{
    int off = 0;
    while (off < n) {
        ssize_t r = read(conn->fd, (char *)buf + off, n - off);
        if (r <= 0) return -1;
        off += r;
    }
    return 0;
}

static int recv_byte(qrsp_conn_t *conn)
{
    uint8_t b;
    if (recv_exact(conn, &b, 1) < 0) return -1;
    return b;
}

static int send_raw(qrsp_conn_t *conn, const char *buf, int len)
{
    int off = 0;
    while (off < len) {
        ssize_t w = write(conn->fd, buf + off, len - off);
        if (w <= 0) return -1;
        off += w;
    }
    return 0;
}

/* ---- public API ---- */

int qrsp_connect(qrsp_conn_t *conn, const char *host, int port)
{
    memset(conn, 0, sizeof(*conn));
    conn->fd = -1;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("qrsp: socket");
        return -1;
    }

    struct sockaddr_in addr;

    /* Try numeric IP first, fall back to getaddrinfo for hostnames */
    if (inet_aton(host, &addr.sin_addr) == 0) {
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        char port_str[16];
        snprintf(port_str, sizeof(port_str), "%d", port);
        int ret = getaddrinfo(host, port_str, &hints, &res);
        if (ret != 0) {
            fprintf(stderr, "qrsp: cannot resolve %s: %s\n", host, gai_strerror(ret));
            close(fd);
            return -1;
        }
        if (res->ai_addrlen > sizeof(addr)) {
            fprintf(stderr, "qrsp: address too large\n");
            freeaddrinfo(res);
            close(fd);
            return -1;
        }
        memcpy(&addr, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("qrsp: connect");
        close(fd);
        return -1;
    }

    int opt = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));

    conn->fd = fd;
    return 0;
}

void qrsp_close(qrsp_conn_t *conn)
{
    if (conn->fd >= 0) {
        close(conn->fd);
        conn->fd = -1;
    }
}

int qrsp_send_packet(qrsp_conn_t *conn, const char *data, int len)
{
    uint8_t csum = rsp_checksum(data, len);

    char header[4];
    header[0] = '$';
    int hlen = 1;

    /* build $<data>#<c1><c2> */
    if (send_raw(conn, header, hlen) < 0) return -1;
    if (len > 0 && send_raw(conn, data, len) < 0) return -1;

    char trailer[4];
    int tlen = snprintf(trailer, sizeof(trailer), "#%02x", csum);
    if (send_raw(conn, trailer, tlen) < 0) return -1;

    /* wait for ACK */
    int ack = recv_byte(conn);
    if (ack == '+') return 0;
    if (ack == '-') {
        /* one retry */
        if (send_raw(conn, header, hlen) < 0) return -1;
        if (len > 0 && send_raw(conn, data, len) < 0) return -1;
        if (send_raw(conn, trailer, tlen) < 0) return -1;
        ack = recv_byte(conn);
        if (ack == '+') return 0;
    }
    fprintf(stderr, "qrsp: expected ACK(+), got 0x%02x\n", ack);
    return -1;
}

int qrsp_recv_packet(qrsp_conn_t *conn, char *buf, int buf_size)
{
    /* skip until '$' */
    int c;
    for (;;) {
        c = recv_byte(conn);
        if (c < 0) return -1;
        if (c == '$') break;
        /* ignore inter-packet bytes (notifications, stray ACKs) */
    }

    /* read payload until '#' */
    int pos = 0;
    for (;;) {
        c = recv_byte(conn);
        if (c < 0) return -1;
        if (c == '#') break;
        if (pos < buf_size - 1) {
            buf[pos++] = (char)c;
        }
    }
    buf[pos] = '\0';

    /* read 2-char checksum */
    char cs_hex[3] = {0, 0, 0};
    if (recv_exact(conn, cs_hex, 2) < 0) return -1;

    uint8_t expected = rsp_checksum(buf, pos);
    uint8_t received = (uint8_t)((fromhex(cs_hex[0]) << 4) | fromhex(cs_hex[1]));

    if (expected != received) {
        fprintf(stderr, "qrsp: bad checksum (got %02x, expected %02x)\n",
                received, expected);
        if (send_raw(conn, "-", 1) < 0) return -1;
        return -1;
    }

    /* send ACK */
    if (send_raw(conn, "+", 1) < 0) return -1;
    return pos;
}

int qrsp_step(qrsp_conn_t *conn)
{
    if (qrsp_send_packet(conn, "s", 1) < 0) return -1;

    char reply[256];
    int n = qrsp_recv_packet(conn, reply, sizeof(reply));
    if (n < 0) return -1;

    /* stop reply: Sxx or Txx... */
    if (reply[0] != 'S' && reply[0] != 'T') {
        /* "Wxx" means exited, "Xxx" means terminated */
        if (reply[0] == 'W' || reply[0] == 'X') {
            fprintf(stderr, "qrsp: QEMU exited (packet: %s)\n", reply);
        } else {
            fprintf(stderr, "qrsp: unexpected stop reply: %s\n", reply);
        }
        return -1;
    }
    return 0;
}

int qrsp_read_g_packet(qrsp_conn_t *conn, uint8_t *bin_buf, int buf_size)
{
    if (buf_size < 280) {
        fprintf(stderr, "qrsp: bin_buf too small for g packet (%d < 280)\n", buf_size);
        return -1;
    }

    if (qrsp_send_packet(conn, "g", 1) < 0) return -1;

    /* hex payload can be up to 560 chars for 35 regs */
    char hex[1024];
    int n = qrsp_recv_packet(conn, hex, sizeof(hex));
    if (n < 0) return -1;

    /* "EXX" = error, "x" = empty (unsupported) */
    if (n == 0 || hex[0] == 'E' || (n == 1 && hex[0] == 'x')) {
        fprintf(stderr, "qrsp: g packet returned error: %s\n", hex);
        return -1;
    }

    /* decode hex to binary (little-endian per 8-byte register) */
    if (n < 560) {
        fprintf(stderr, "qrsp: g packet too short (%d hex chars, expected 560)\n", n);
        return -1;
    }

    memset(bin_buf, 0, 280);
    for (int i = 0; i < 280; i++) {
        int hi = fromhex(hex[i * 2]);
        int lo = fromhex(hex[i * 2 + 1]);
        if (hi < 0 || lo < 0) {
            fprintf(stderr, "qrsp: invalid hex char at offset %d\n", i * 2);
            return -1;
        }
        bin_buf[i] = (uint8_t)((hi << 4) | lo);
    }
    return 0;
}

uint64_t qrsp_hex_decode_le64(const char *hex)
{
    uint64_t val = 0;
    for (int i = 0; i < 8; i++) {
        int hi = fromhex(hex[i * 2]);
        int lo = fromhex(hex[i * 2 + 1]);
        val |= (uint64_t)((hi << 4) | lo) << (i * 8);
    }
    return val;
}

int qrsp_continue(qrsp_conn_t *conn)
{
    if (qrsp_send_packet(conn, "c", 1) < 0) return -1;

    char reply[256];
    int n = qrsp_recv_packet(conn, reply, sizeof(reply));
    if (n < 0) return -1;

    if (reply[0] != 'T' && reply[0] != 'S') {
        if (reply[0] == 'W' || reply[0] == 'X') {
            fprintf(stderr, "qrsp: QEMU exited (packet: %s)\n", reply);
        } else {
            fprintf(stderr, "qrsp: unexpected continue reply: %s\n", reply);
        }
        return -1;
    }
    return 0;
}

int qrsp_set_breakpoint(qrsp_conn_t *conn, uint64_t addr, int kind)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "Z%d,%lx,4", kind, addr);
    if (qrsp_send_packet(conn, cmd, strlen(cmd)) < 0) return -1;

    char reply[16];
    int n = qrsp_recv_packet(conn, reply, sizeof(reply));
    if (n < 0) return -1;
    if (strcmp(reply, "OK") != 0) {
        fprintf(stderr, "qrsp: set breakpoint failed: %s\n", reply);
        return -1;
    }
    return 0;
}

int qrsp_remove_breakpoint(qrsp_conn_t *conn, uint64_t addr, int kind)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "z%d,%lx,4", kind, addr);
    if (qrsp_send_packet(conn, cmd, strlen(cmd)) < 0) return -1;

    char reply[16];
    int n = qrsp_recv_packet(conn, reply, sizeof(reply));
    if (n < 0) return -1;
    if (strcmp(reply, "OK") != 0) {
        fprintf(stderr, "qrsp: remove breakpoint failed: %s\n", reply);
        return -1;
    }
    return 0;
}
