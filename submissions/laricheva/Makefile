CC = gcc
ASM = nasm
CFLAGS = -m32 -Wall -Wextra -O2 -Iinclude
ASMFLAGS = -f elf32
LDFLAGS = -m32 -lm
EPS ?= 1e-5
METHOD ?= BISECTION

ifeq ($(METHOD),NEWTON)
    CFLAGS += -DUSE_NEWTON
endif

CFLAGS += -DEPS=$(EPS)f

TARGET = prog
OBJS = src/main.o src/numerical.o asm/functions.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) -c -o $@ $<

asm/%.o: asm/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
