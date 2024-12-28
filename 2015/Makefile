# Build instructions

# Compiler and flags
CC_GCC = gcc
CC_CLANG = clang
CFLAGS = -Wall -I./include
LDFLAGS = -L./lib -luv

# Directories
SRC_DIR = src
BUILD_DIR_GCC = build_gcc
BUILD_DIR_CLANG = build_clang

# Source files and corresponding executables
SOURCES = $(wildcard $(SRC_DIR)/day*.c)
EXECUTABLES_GCC = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR_GCC)/%, $(SOURCES))
EXECUTABLES_CLANG = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR_CLANG)/%, $(SOURCES))

# Default target: Build with both compilers
all: gcc clang

# GCC build target
gcc: $(EXECUTABLES_GCC)

$(BUILD_DIR_GCC)/%: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR_GCC)
	$(CC_GCC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Clang build target
clang: $(EXECUTABLES_CLANG)

$(BUILD_DIR_CLANG)/%: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR_CLANG)
	$(CC_CLANG) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR_GCC) $(BUILD_DIR_CLANG)

