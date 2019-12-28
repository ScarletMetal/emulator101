CC=gcc
CFLAGS=-I.
DEPS=disassembler.h

emulator101: disassembler.o 8080core.o
	$(CC) -o emulator101 disassembler.o 8080core.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
