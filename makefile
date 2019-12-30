CC=gcc
CFLAGS=-I.
DEPS=disassembler.h

emulator101: disassembler.o core8080.o
	$(CC) -o emulator101 disassembler.o core8080.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm *.o
