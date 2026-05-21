CC = gcc
CFLAGS = -m32 -Iinclude
NASM = nasm
NASMFLAGS = -f elf32

SRCDIR = src
ASMDIR = asm
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
ASMS = $(wildcard $(ASMDIR)/*.asm)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
OBJECTS += $(patsubst $(ASMDIR)/%.asm, $(OBJDIR)/%.o, $(ASMS))
TARGET = lab6
all: $(TARGET)
newton: CFLAGS += -DUSE_NEWTON
newton: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJDIR)/%.o: $(ASMDIR)/%.asm
	mkdir -p $(OBJDIR)
	$(NASM) $(NASMFLAGS) $< -o $@
clean:
	rm -rf $(OBJDIR) $(TARGET)
.PHONY: all clean newton
