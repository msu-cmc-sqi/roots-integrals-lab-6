# Makefile — Lab 6, Kotsin A.O.
# Build: make              (Newton's method, default)
#        make METHOD=METHOD_CHORDS  (Chord/regula-falsi method)

METHOD  ?= METHOD_NEWTON

CC       = gcc
ASM      = nasm
CFLAGS   = -m32 -Wall -Wextra -D$(METHOD)
ASMFLAGS = -f elf32

SRCDIR   = src
OBJDIR   = obj

SRCS     = $(SRCDIR)/main.c $(SRCDIR)/root.c $(SRCDIR)/integral.c
ASMSRCS  = $(SRCDIR)/functions.asm
OBJS     = $(OBJDIR)/main.o $(OBJDIR)/root.o $(OBJDIR)/integral.o $(OBJDIR)/functions.o

.PHONY: all clean

all: $(OBJDIR) prog

$(OBJDIR):
	mkdir -p $(OBJDIR)

prog: $(OBJS)
	$(CC) -m32 -no-pie -o $@ $^ -lm

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/root.h $(SRCDIR)/integral.h
	$(CC) $(CFLAGS) -I$(SRCDIR) -c -o $@ $<

$(OBJDIR)/root.o: $(SRCDIR)/root.c $(SRCDIR)/root.h
	$(CC) $(CFLAGS) -I$(SRCDIR) -c -o $@ $<

$(OBJDIR)/integral.o: $(SRCDIR)/integral.c $(SRCDIR)/integral.h
	$(CC) $(CFLAGS) -I$(SRCDIR) -c -o $@ $<

$(OBJDIR)/functions.o: $(SRCDIR)/functions.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	rm -rf $(OBJDIR) prog
