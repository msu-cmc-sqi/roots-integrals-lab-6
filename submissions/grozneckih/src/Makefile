all:
	nasm -f elf32 funcs.asm -o funcs.o
	gcc -m32 -no-pie main.c funcs.o -lm -o program

clean:
	rm -f *.o program