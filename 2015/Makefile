# Build instructions

# Compiler and flags
CC_CLANG = clang
CC_GCC = gcc
LINKER_CLANG = lld
LINKER_GCC = gold
CFLAGS = -Wall -I./include
LDFLAGS = -lcurl

# Directories
SRC_DIR = src
DATA_DIR = data
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
OBJ_DIR_GCC = $(OBJ_DIR)/gcc
OBJ_DIR_CLANG = $(OBJ_DIR)/clang
INSTALL_DIR = /usr/local/bin
LIB_AOC_DIR = lib/aoc

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
DAY_SOURCES = $(wildcard $(SRC_DIR)/day*.c)
LIB_AOC_SOURCES = $(LIB_AOC_DIR)/aoc.c
HEADERS = $(wildcard include/*.h)

# Object files
GCC_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_GCC)/%.o, $(MAIN_SRC) $(DAY_SOURCES))
CLANG_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_CLANG)/%.o, $(MAIN_SRC) $(DAY_SOURCES))

# Executables
EXECUTABLE_AOC = $(BUILD_DIR)/aoc/aoc
EXECUTABLE_GCC = $(BUILD_DIR)/gcc/gcc2
EXECUTABLE_CLANG = $(BUILD_DIR)/clang/clang2
EXECUTABLE_MAIN = $(BUILD_DIR)/main/main
EXECUTABLE_EXAMPLE = $(BUILD_DIR)/example
EXECUTABLE_HELLO = $(BUILD_DIR)/hello

# Default target
all: aoc gcc2 clang2 main example hello

# Build aoc executable
aoc: $(EXECUTABLE_AOC)

$(EXECUTABLE_AOC): $(LIB_AOC_SOURCES) $(HEADERS)
	@mkdir -p $(BUILD_DIR)/aoc
	@echo "Building aoc executable: $@"
	$(CC_GCC) $(CFLAGS) -fuse-ld=$(LINKER_GCC) -DSTANDALONE_AOC $(LIB_AOC_SOURCES) -o $@ $(LDFLAGS)

# Build custom gcc2 compiler
gcc2: $(EXECUTABLE_GCC)

$(EXECUTABLE_GCC): $(GCC_OBJECTS)
	@mkdir -p $(BUILD_DIR)/gcc
	@echo "Building GCC2 executable: $@"
	$(CC_GCC) $(CFLAGS) -fuse-ld=$(LINKER_GCC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR_GCC)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR_GCC)
	@echo "Compiling with GCC: $<"
	$(CC_GCC) $(CFLAGS) -c $< -o $@

# Build custom clang2 compiler
clang2: $(EXECUTABLE_CLANG)

$(EXECUTABLE_CLANG): $(CLANG_OBJECTS)
	@mkdir -p $(BUILD_DIR)/clang
	@echo "Building Clang2 executable: $@"
	$(CC_CLANG) $(CFLAGS) -fuse-ld=$(LINKER_CLANG) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR_CLANG)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR_CLANG)
	@echo "Compiling with Clang: $<"
	$(CC_CLANG) $(CFLAGS) -c $< -o $@

# Build unified main compiler (both compilers)
main: gcc2 clang2
	@echo "Creating unified executable for both compilers: $(EXECUTABLE_MAIN)"
	@mkdir -p $(BUILD_DIR)/main
	@echo "#!/bin/bash" > $(EXECUTABLE_MAIN)
	@echo "$(BUILD_DIR)/gcc/gcc2 all" >> $(EXECUTABLE_MAIN)
	@echo "$(BUILD_DIR)/clang/clang2 all" >> $(EXECUTABLE_MAIN)
	@echo "$(BUILD_DIR)/main/main all" >> $(EXECUTABLE_MAIN)
	@chmod +x $(EXECUTABLE_MAIN)

# Build example executable
example: $(EXECUTABLE_EXAMPLE)

$(EXECUTABLE_EXAMPLE): examples/example.c
	@mkdir -p $(BUILD_DIR)/examples
	@echo "Building example executable: $@"
	$(CC_CLANG) $(CFLAGS) -fuse-ld=$(LINKER_CLANG) $< -o $@ $(LDFLAGS) -luv

# Build hello executable
hello: $(EXECUTABLE_HELLO)

$(EXECUTABLE_HELLO): examples/hello.c
	@mkdir -p $(BUILD_DIR)/examples
	@echo "Building hello executable: $@"
	$(CC_CLANG) $(CFLAGS) -fuse-ld=$(LINKER_CLANG) $< -o $@ $(LDFLAGS)

# Install all executables globally
install: aoc gcc2 clang2 main example hello
	@echo "Installing executables to /usr/local/bin..."
	@sudo cp $(EXECUTABLE_AOC) /usr/local/bin/aoc
	@sudo cp $(EXECUTABLE_GCC) /usr/local/bin/gcc2
	@sudo cp $(EXECUTABLE_CLANG) /usr/local/bin/clang2
	@sudo cp $(EXECUTABLE_MAIN) /usr/local/bin/main
	@sudo cp $(EXECUTABLE_EXAMPLE) /usr/local/bin/example
	@sudo cp $(EXECUTABLE_HELLO) /usr/local/bin/hello
	@sudo chmod +x /usr/local/bin/aoc /usr/local/bin/gcc2 /usr/local/bin/clang2 /usr/local/bin/main /usr/local/bin/example /usr/local/bin/hello

# Makefile checkpoint for compilers and aoc library
verify:
	@echo "Current PATH:"
	@echo $$PATH
	@echo "If necessary, add /usr/local/bin to your PATH:"
	@echo 'export PATH=/usr/local/bin:$$PATH'
	@echo "Installation of workflow environment complete!"

# Clean build artifacts and remove globally installed executables
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaning workflow environment..."
	@sudo rm -f /usr/local/bin/aoc /usr/local/bin/gcc2 /usr/local/bin/clang2 /usr/local/bin/main /usr/local/bin/example /usr/local/bin/hello
	@echo "Workflow environment cleaning complete."

