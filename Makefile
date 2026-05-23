CC=gcc
OPT_FLAG = -O3 -flto=auto
ifeq (${DEBUG},1)
	OPT_FLAG = -Og
endif

ifeq (${DIFF},1)
	GDB := 0
else
	GDB := 1
endif
CFLAGS ?= -g ${OPT_FLAG} -MMD -MP -Iinclude -Wall -Werror -Wno-error=unused-function \
          -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LDFLAGS ?= -lm -lrt -rdynamic ${OPT_FLAG} -lglib-2.0
ifeq (${GDB},1)
	CFLAGS += -DCONFIG_GDB
	GDB_SOURCES := src/gdbstub/gdbserver.c src/gdbstub/gdbstub.c \
	               src/gdbstub/gdbstub-xml.c src/gdbstub/la_gdbstub.c \
	               src/gdbstub/gdbutil.c
endif

ifeq (${CLI},1)
	CFLAGS += -DCONFIG_CLI
endif

ifeq (${DIFF},1)
	CFLAGS += -DCONFIG_DIFF -fPIC
endif

ifeq (${PERF},1)
	CFLAGS += -DCONFIG_PERF
endif

CORE := openc910

ifeq (${CORE},)
	CFLAGS += -D__CORE__=la464
else
	CFLAGS += -D__CORE__=$(CORE)
endif

ifeq (${PLUGIN},1)
	LDFLAGS += -ldl
	CFLAGS += -DCONFIG_PLUGIN
endif

arch := $(shell gcc -dumpmachine)
ifeq ($(arch),loongarch64-linux-gnu)
   LDFLAGS+=-Wl,-Tlink_script/loongarch64.lds
endif
ifeq (${DIFF},1)
	LDFLAGS += -shared -fPIC -Wl,--no-undefined
endif

BUILD_DIR := ./build
SRC_DIRS := ./

USER_SOURCES := src/fpu_helper.c  src/host-utils.c  src/int128.c  src/interpreter.c  src/main.c  src/softfloat.c src/vec_helper.c src/tcg-runtime-gvec.c src/syscall.c ${GDB_SOURCES} src/debug_cli.c src/cpu.c src/checkpoint.c
USER_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%_user.o,$(USER_SOURCES)))
USER_DEPS := $(USER_OBJS:.o=.d)

KERNEL_SOURCES := src/fpu_helper.c  src/host-utils.c  src/int128.c  src/interpreter.c  src/main.c  src/softfloat.c  src/tlb_helper.c src/cpu_helper.c src/vec_helper.c src/tcg-runtime-gvec.c src/serial.c src/serial_plus.c ${GDB_SOURCES} src/debug_cli.c src/cpu.c src/fifo.c src/checkpoint.c
KERNEL_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%_kernel.o,$(KERNEL_SOURCES)))
KERNEL_DEPS := $(KERNEL_OBJS:.o=.d)

DIFF_SOURCES := $(KERNEL_SOURCES) src/difftest.c
DIFF_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%_diff.o,$(DIFF_SOURCES)))
DIFF_DEPS := $(DIFF_OBJS:.o=.d)

ifeq (${DIFF},1)
	TARGETS = $(BUILD_DIR)/la_emu_ref.so
else
	TARGETS = $(BUILD_DIR)/la_emu_kernel
endif

all: $(TARGETS)
	make -C plugins -j

$(BUILD_DIR)/la_emu_user : ${USER_OBJS}
	$(CC) $(USER_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%_user.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DCONFIG_USER_ONLY=1 -c -o $@ $<

$(BUILD_DIR)/la_emu_kernel : ${KERNEL_OBJS}
	$(CC) $(KERNEL_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%_kernel.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/la_emu_ref.so : ${DIFF_OBJS}
	$(CC) $(DIFF_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%_diff.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<


KERNEL_DIR    := /home/airxs/user/cpu/system/linux/linux-6.10-labcore164-emu-dbg


CHECKPOINT_PC := 0x9000000000229d40



KERNEL := $(KERNEL_DIR)/vmlinux


run:
	./build/la_emu_kernel -z -s -m 16 -k $(KERNEL)

ckp:
	./build/la_emu_kernel -z -s -m 16 -C $(CHECKPOINT_PC) -k $(KERNEL)


BIOS_BIN := /home/airxs/user/cpu/loong64/software/u-boot/u-boot-dtb.bin

bios:
	./build/la_emu_kernel -z -s -m 16 -k $(BIOS_BIN) -b -E 0x9000000006200000

BIOS_SPL_BIN := /home/airxs/user/cpu/loong64/software/u-boot/spl/u-boot-spl-dtb.bin

spl:
	./build/la_emu_kernel -z -s -m 16 -k $(BIOS_SPL_BIN) -b -E 0x1c000000

dbg:
	./build/la_emu_kernel -m 16G -k $(KERNEL) -g -w -v

clean:
	rm -rf build

.EXTRA_PREREQS = Makefile
-include $(USER_DEPS)
-include $(KERNEL_DEPS)


