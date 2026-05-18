CC=gcc
ASM=nasm
CFLAGS=-m32 -Wall -Wextra -O2
ASMFLAGS=-f elf32
LDFLAGS=-m32 -lm
METHOD?=BISECTION

ifeq ($(METHOD),COMBINED)
    CFLAGS += -DCOMBINED
endif

all: main

main: main.o root.o integral.o funcs.o
	$(CC) main.o root.o integral.o funcs.o $(LDFLAGS) -o main

main.o: main.c funcs.h
	$(CC) $(CFLAGS) -c main.c

root.o: root.c funcs.h
	$(CC) $(CFLAGS) -c root.c

integral.o: integral.c funcs.h
	$(CC) $(CFLAGS) -c integral.c

funcs.o: funcs.asm
	$(ASM) $(ASMFLAGS) funcs.asm -o funcs.o

clean:
	rm -f *.o main
