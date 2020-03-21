CC=gcc
CFLAGS=-I.
csrc = $(wildcard core/*.c)

obj = $(csrc:.c=.o) main.o

./build/%.o: $(csrc)
	gcc -c $@ -o $^ $(CFLAGS)

emulator101: $(obj)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -rf $(obj)