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
# CFLAGS ?= -g -O3 -flto=auto -march=native -mtune=native -MMD -MP -I. -Iinclude -DCONFIG_INT128
CFLAGS ?= -g ${OPT_FLAG} -MMD -MP -I. -Iinclude -Wall -Werror -Wno-error=unused-function \
          -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LDFLAGS ?= -lm -lrt -rdynamic ${OPT_FLAG} -lglib-2.0
ifeq (${GDB},1)
	CFLAGS += -DCONFIG_GDB
	GDB_SOURCES := gdbstub/gdbserver.c gdbstub/gdbstub.c \
	               gdbstub/gdbstub-xml.c gdbstub/la_gdbstub.c \
	               gdbstub/gdbutil.c
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

USER_SOURCES := fpu_helper.c  host-utils.c  int128.c  interpreter.c  main.c  softfloat.c vec_helper.c tcg-runtime-gvec.c syscall.c ${GDB_SOURCES} debug_cli.c cpu.c checkpoint.c
USER_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%_user.o,$(USER_SOURCES)))
USER_DEPS := $(USER_OBJS:.o=.d)

KERNEL_SOURCES := fpu_helper.c  host-utils.c  int128.c  interpreter.c  main.c  softfloat.c  tlb_helper.c cpu_helper.c vec_helper.c tcg-runtime-gvec.c serial.c serial_plus.c ${GDB_SOURCES} debug_cli.c cpu.c fifo.c checkpoint.c
KERNEL_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%_kernel.o,$(KERNEL_SOURCES)))
KERNEL_DEPS := $(KERNEL_OBJS:.o=.d)

DIFF_SOURCES := $(KERNEL_SOURCES) difftest.c
DIFF_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%_diff.o,$(DIFF_SOURCES)))
DIFF_DEPS := $(DIFF_OBJS:.o=.d)

# $(info $$USER_SOURCES is [${USER_SOURCES}])
# $(info $$USER_OBJS is [${USER_OBJS}])
# $(info $$USER_DEPS is [${USER_DEPS}])

# $(info $$KERNEL_SOURCES is [${KERNEL_SOURCES}])
# $(info $$KERNEL_OBJS is [${KERNEL_OBJS}])
# $(info $$KERNEL_DEPS is [${KERNEL_DEPS}])

# $(info $$DIFF_SOURCES is [${DIFF_SOURCES}])
# $(info $$DIFF_OBJS is [${DIFF_OBJS}])
# $(info $$DIFF_DEPS is [${DIFF_DEPS}])

ifeq (${DIFF},1)
	TARGETS = $(BUILD_DIR)/la_emu_ref.so
else
# 	TARGETS = $(BUILD_DIR)/la_emu_user $(BUILD_DIR)/la_emu_kernel
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


CHECKPOINT_PC := 0x90000000009fcd68
# CHECKPOINT_PC := 0x1200000e8
# CHECKPOINT_PC := 0x9000000000229d40
CHECKPOINT_PC := 0x9000000000229d40
CHECKPOINT_PC := 0x0x120000f4c



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


