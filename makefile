CC=gcc
CFLAGS=-I. -L/usr/local/Cellar/argp-standalone/1.3/lib/ -largp
csrc = $(wildcard core/*.c) main.c

obj = $(csrc:.c=.o)

./build/%.o: $(csrc)
	gcc -c $@ -o $^ $(CFLAGS)

emulator101: $(obj)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -rf $(obj)