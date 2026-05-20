CC=gcc
ASM=nasm

CFLAGS=-O2 -std=c11 -Wall -Wextra -m32
LDFLAGS=-m32 -lm

all: lab6

lab6: src/main.o src/methods.o src/functions.o
	$(CC) -o $@ $^ $(LDFLAGS)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

src/methods.o: src/methods.c
	$(CC) $(CFLAGS) -c $< -o $@

src/functions.o: src/functions.asm
	$(ASM) -f elf32 $< -o $@

clean:
	rm -f src/*.o lab6