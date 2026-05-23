# Build test binaries for LoongArch64 difftest
CROSS_PREFIX ?= /home/airxs/local/gcc-14.1.0-loongarch64-linux-gnu/bin/loongarch64-linux-gnu-
AS      = $(CROSS_PREFIX)as
LD      = $(CROSS_PREFIX)ld
OBJCOPY = $(CROSS_PREFIX)objcopy

SRCS = $(wildcard test_*.S)
BINS = $(SRCS:.S=.bin)

CFLAGS =

# Link at 0x1c000000 — LoongArch reset vector / QEMU -bios load address
TEXT_ADDR = 0x1c000000

all: $(BINS)

%.elf: %.S
	$(AS) -o $@.o $<
	$(LD) -Ttext=$(TEXT_ADDR) -o $@ $@.o

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o *.elf *.bin

.PHONY: all clean
