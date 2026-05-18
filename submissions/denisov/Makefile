CC = gcc
ASM = nasm

TARGET = lab6
SRC_DIR = src
BUILD_DIR = build

CFLAGS = -m32 -Wall -Wextra -std=c99 -O2
LDFLAGS = -m32
LDLIBS = -lm

METHOD ?= newton

ifeq ($(METHOD),chord)
    CFLAGS += -DUSE_CHORD
endif

all: $(TARGET)

$(TARGET): $(BUILD_DIR)/main.o $(BUILD_DIR)/functions.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/functions.o: $(SRC_DIR)/functions.asm | $(BUILD_DIR)
	$(ASM) -f elf32 -o $@ $<

newton:
	$(MAKE) clean
	$(MAKE) METHOD=newton

chord:
	$(MAKE) clean
	$(MAKE) METHOD=chord

test: all
	./$(TARGET) --test

report:
	cd report && pdflatex report.tex

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -f report/*.aux report/*.log report/*.out

.PHONY: all clean test newton chord report
