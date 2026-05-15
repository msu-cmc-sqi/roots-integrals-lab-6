CC = gcc
NASM = nasm
CFLAGS = -m32 -std=c99
NASM_FLAGS = -f elf32

# выбор метода поиска корня:
#   make             -- метод деления пополам (по умолчанию)
#   make METHOD=CHORDS -- метод хорд
ifeq ($(METHOD),CHORDS)
    CFLAGS += -DMETHOD_CHORDS
endif

SOURCES = src/main.c src/root.c src/integral.c
OBJECTS = main.o root.o integral.o f1.o f2.o f3.o
TARGET = prog

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

main.o: src/main.c include/funcs.h
	$(CC) $(CFLAGS) -c -o main.o src/main.c

root.o: src/root.c include/funcs.h
	$(CC) $(CFLAGS) -c -o root.o src/root.c

integral.o: src/integral.c include/funcs.h
	$(CC) $(CFLAGS) -c -o integral.o src/integral.c

f1.o: asm/f1.asm
	$(NASM) $(NASM_FLAGS) -o f1.o asm/f1.asm

f2.o: asm/f2.asm
	$(NASM) $(NASM_FLAGS) -o f2.o asm/f2.asm

f3.o: asm/f3.asm
	$(NASM) $(NASM_FLAGS) -o f3.o asm/f3.asm

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
