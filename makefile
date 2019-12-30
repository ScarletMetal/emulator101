CC=gcc
CFLAGS=-I.
DEPS=disassembler.h core8080.h

emulator101: disassembler.o core8080.o main.o
	$(CC) -o emulator101 disassembler.o core8080.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm *.o
