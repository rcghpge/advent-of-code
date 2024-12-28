# Build instructions

# Compiler and flags
CC_GCC = gcc
CC_CLANG = clang
CFLAGS = -Wall -I./include
LDFLAGS = -L./lib -luv

# Directories
SRC_DIR = src
DATA_DIR = data
BUILD_DIR_GCC = build_gcc
BUILD_DIR_CLANG = build_clang
OBJ_DIR_GCC = $(BUILD_DIR_GCC)/obj
OBJ_DIR_CLANG = $(BUILD_DIR_CLANG)/obj

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
DAY_SOURCES = $(wildcard $(SRC_DIR)/day*.c)
HEADERS = $(wildcard include/*.h)

# Object files
OBJ_FILES_GCC = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_GCC)/%.o, $(MAIN_SRC) $(DAY_SOURCES))
OBJ_FILES_CLANG = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_CLANG)/%.o, $(MAIN_SRC) $(DAY_SOURCES))

# Executables
EXECUTABLE_GCC = $(BUILD_DIR_GCC)/2015_solutions_gcc
EXECUTABLE_CLANG = $(BUILD_DIR_CLANG)/2015_solutions_clang

# Default target
all: validate_inputs gcc clang

# Validate input files (allow partial validation)
validate_inputs:
	@echo "Validating input files in $(DATA_DIR)..."
	@if [ ! -d $(DATA_DIR) ]; then \
		echo "Error: Data directory $(DATA_DIR) is missing. Please create it and add input files."; \
		exit 1; \
	fi
	@found_files=0; \
	missing_files=0; \
	for i in `seq 1 25`; do \
		if [ -f $(DATA_DIR)/$$i.txt ]; then \
			found_files=$$((found_files + 1)); \
		else \
			echo "Warning: Input file $(DATA_DIR)/$$i.txt is missing."; \
			missing_files=$$((missing_files + 1)); \
		fi; \
	done; \
	if [ $$found_files -eq 0 ]; then \
		echo "Error: No input files found in $(DATA_DIR)."; \
		exit 1; \
	fi; \
	echo "Input validation complete: Found $$found_files out of 25 files."

# GCC build target
gcc: $(EXECUTABLE_GCC)

$(EXECUTABLE_GCC): $(OBJ_FILES_GCC)
	$(CC_GCC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR_GCC)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(OBJ_DIR_GCC)
	$(CC_GCC) $(CFLAGS) -c $< -o $@

# Clang build target
clang: $(EXECUTABLE_CLANG)

$(EXECUTABLE_CLANG): $(OBJ_FILES_CLANG)
	$(CC_CLANG) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR_CLANG)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(OBJ_DIR_CLANG)
	$(CC_CLANG) $(CFLAGS) -c $< -o $@

# Run GCC build
run_gcc: validate_inputs
	@echo "Running GCC build..."
	./$(EXECUTABLE_GCC)

# Run Clang build
run_clang: validate_inputs
	@echo "Running Clang build..."
	./$(EXECUTABLE_CLANG)

# Run both GCC and Clang builds
run_all: validate_inputs
	@echo "Running all 25 days with GCC and Clang..."
	@echo "GCC Output:"
	./$(EXECUTABLE_GCC)
	@echo "Clang Output:"
	./$(EXECUTABLE_CLANG)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR_GCC) $(BUILD_DIR_CLANG)

