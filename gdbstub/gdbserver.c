#include "qemu/osdep.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "cpu.h"

#include "gdbserver.h"

bool gdb_verbose;

uint64_t gdb_fetch_breakpoint[GDB_FETCCH_BREAKPOINT_NUM];

int sock_fd = -1;

volatile int gdbserver_has_message;
static void io_handler(int sig) {
    // fprintf(stderr, "signal %d %s\n", sig, strsignal(sig));
    gdbserver_has_message = 1;
}

void remote_prepare(int port) {
    int ret;
    struct sockaddr_in addr;
    const int one = 1;
    int listen_fd;

    listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
    if (listen_fd < 0) {
        perror("socket() failed");
        laemu_exit(-1);
    }

    ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if (ret < 0) {
        perror("setsockopt() failed");
        laemu_exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind() failed");
        laemu_exit(-1);
    }

    ret = listen(listen_fd, 1);
    if (ret < 0) {
        perror("listen() failed");
        laemu_exit(-1);
    }

    fprintf(stderr, "Listening on port %d\n", port);

    sock_fd = accept(listen_fd, NULL, NULL);
    if (sock_fd < 0) {
        perror("accept() failed");
        laemu_exit(-1);
    } else {
        fprintf(stderr, "sock_fd:%d\n", sock_fd);
    }

    ret = setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof(one));
    ret = setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    // enable_async_notification(sock_fd);
    close(listen_fd);
}

void dump_nchar(char* str, int n) {
    for (int i = 0; i < n; i++) {
        fprintf(stderr, "%c", str[i]);
    }
    fprintf(stderr, "\n");
}

#define RECEIVE_BUF_SIZE 0x100000

// ring buffer
#define RECEIVE_RING_BUF_SIZE 0x1000
#define RECEIVE_RING_BUF_LEN  0x100
static char receive_ring_buf[RECEIVE_RING_BUF_LEN][RECEIVE_BUF_SIZE];

static int receive_ring_index = 0;
static int consume_ring_index = 0;

const char* ack_response = "+";
const char* empty_response = "$#00";

bool cpu_can_run;

void gdbserver_read_message() {
    while (gdbserver_has_message) {
        gdbserver_has_message = 0;
        ssize_t r = read(sock_fd, receive_ring_buf[receive_ring_index], RECEIVE_RING_BUF_SIZE);
        if (r < 0) {
            if (gdb_verbose) fprintf(stderr, "read failed %ld\n", r);
        } else if (r == 0) {
            if (gdb_verbose && 0) fprintf(stderr, "read nothing\n");
            continue;
        } else {
            
            if ((receive_ring_index + 1) == consume_ring_index) {
                perror("receive_ring_buf is full!\n");
            }
            else if (receive_ring_index == (RECEIVE_RING_BUF_LEN-1)) {
                receive_ring_index = 0;
            }
            else {
                receive_ring_index ++;
            }
        }
    }
}

void gdb_read_byte(uint8_t ch);

void gdbserver_handle_message() {
    int i;
    if (consume_ring_index == receive_ring_index) {
        if (gdb_verbose && 0)
            fprintf(stderr,"receive_ring_buf is empty!\n");
        return;
    }
    if (gdb_verbose && 0)
        fprintf(stderr, "Read: %s\n", receive_ring_buf[consume_ring_index]);

    // read gdb command by fsm.
    for (i = 0; i < strlen((char*)receive_ring_buf[consume_ring_index]); i++) {
        gdb_read_byte(receive_ring_buf[consume_ring_index][i]);
    }

    // FIXME: Must clear ring buffer, GDB Error: Invalid hex digit XX
    memset(receive_ring_buf[consume_ring_index], 0x0, RECEIVE_BUF_SIZE);

    if (consume_ring_index == (RECEIVE_RING_BUF_LEN-1)) {
        consume_ring_index = 0;
    } else {
        consume_ring_index ++;
    }
}

int gdb_put_packet(const char *buf);

int gdbserver_loop(void) {
    
    while (1) {    
        gdbserver_read_message();
        
        gdbserver_handle_message();
        
        if (cpu_can_run) {
            int r = exec_env(current_env);
            if (gdb_verbose)
                fprintf(stderr, "gdb_exec_env exit %d\n", r);
            if (r == 2) {
                gdb_put_packet("S05");
                cpu_can_run = false;
            }
        }
    }
}

extern void gdb_init_gdbserver_state(void);

void gdbserver_init(int port) {
    
    remote_prepare(port);
    
    signal(SIGIO, io_handler);
    
    gdb_init_gdbserver_state();

    for (int i = 0; i < GDB_FETCCH_BREAKPOINT_NUM; i++) {
        gdb_fetch_breakpoint[i] = ~0x0UL;
    }

    gdb_fetch_breakpoint[GDB_FETCCH_BREAKPOINT_NUM-1] = 0x0UL;
    
    /* Set the process receiving SIGIO/SIGURG signals to us. */
    if (fcntl(sock_fd, F_SETOWN, getpid()) < 0) {
        perror("fcntl F_SETOWN");
        laemu_exit(1);
    }
    if (fcntl(sock_fd, F_SETFL, FASYNC | O_NONBLOCK) < 0) {
        perror("fcntl FASYNC");
        laemu_exit(1);
    }
}

