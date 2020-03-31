CC=clang
CFLAGS=-I. -largp -lSDL2
csrc = $(wildcard core/*.c) $(wildcard gui/*.c) $(wildcard lib/*.c) $(wildcard cli/*.c) main.c

obj = $(csrc:.c=.o)

emulator101: $(obj)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -rf $(obj)
