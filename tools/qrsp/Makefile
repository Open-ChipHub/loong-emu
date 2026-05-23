CC=gcc
CFLAGS=-g -O2 -Wall -Werror -fPIC
AR=ar

libqrsp.a: qrsp.o
	$(AR) rcs $@ $^

qrsp.o: qrsp.c qrsp.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.a

.PHONY: clean
