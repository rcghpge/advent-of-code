# Advent of Code 2015 - C Language

**WIP** - School is beginning soon. May not complete before semester starts.

Welcome to the **Advent of Code 2015** directory in the C programming language! This repository provides solutions to the Advent of Code 2015 challenges, written in C with support for both **GCC** and **Clang** compilers. The project is designed for portability, robustness, and flexibility.

---

## Directory Structure

```plaintext
2015/
├── Makefile            # Automates building, running, cleaning, and installing the project
├── README.md           # Documentation of the project
├── setup.sh            # Script to set up the environment and dependencies
├── data/               # Input files for each challenge
│   ├── 1.txt           # Input for Day 1
│   └── ...             # Inputs for Days 2–25
├── examples/           # Example scripts and binaries (e.g., libuv timer demo)
│   ├── example         # Compiled binary for example.c
│   ├── example.c       # Libuv timer example
│   ├── hello           # Compiled binary for hello.c
│   └── hello.c         # Simple "Hello, World!" example
├── include/            # Header files
│   ├── day1.h          # Header for Day 1 solution
│   └── ...             # Headers for Days 2–25
├── lib/                # External libraries (e.g., libuv)
│   └── aoc/            # Advent of Code utilities
├── src/                # Source files for each day's solution
│   ├── main.c          # Main entry point
│   ├── day1.c          # Solution for Day 1
│   └── ...             # Solutions for Days 2–25
└── tests/              # Test files (optional)
    ├── test.c          # Test utilities
    └── ...             # Additional test files
```

---

## Setup Instructions

### Step 1: Run the Setup Script

Run the setup script to initialize your environment:

```bash
./setup.sh
```

This script:
- Ensures **GCC**, **Clang**, **Make**, and other dependencies are installed.
- Configures your environment and sets up the **Advent of Code session cookie**.
- Creates necessary directories (`data/`, `build/`, `examples/`).
- Builds and installs tech stack (e.g., `aoc`, `gcc2`, `clang2`, etc.).

### Step 2: Fetch Input Data

Pull input data for each day into the `data/` directory using the `aoc` command. For example:

```bash
aoc 1 2015       # Fetch input for Day 1 of 2015
aoc all 2015     # Fetch inputs for all days of 2015
```

This will store input files like `data/1.txt`, `data/2.txt`, etc.

---

## Building and Running

### Build the Project

To build all components (e.g., `aoc`, `gcc2`, `clang2`, examples):

```bash
make all
```
Install Makefile workflow environment config

```bash
make install
```

### Run Solutions

- Run a specific day's solution using GCC:

```bash
gcc2 1
```

- Run a specific day's solution using Clang:

```bash
clang2 1
```

- Run all solutions with both compilers:

```bash
main all
```

### Examples in C

Run example in c provided in the `examples/` directory:

```bash
example       # Run the libuv timer example
hello         # Run the Hello World example
```

### Clean Workflow Environment

To remove all build artifacts and reset the environment:

```bash
make clean
```

---

## Input Validation

The `Makefile` validates that input files are present in the `data/` directory. If input files are missing or incomplete, appropriate warnings or errors are displayed.

- Example error message if no input files are found:

```plaintext
Error: No input files found in data/.
```

- Example warning if some files are missing:

```plaintext
Warning: Input file data/2.txt is missing.
Input validation complete: Found 24 out of 25 files.
```

---

## Dependencies

- [**GCC:**](https://gcc.gnu.org/onlinedocs/) GNU Compiler Collection
- [**Clang:**](https://clang.llvm.org/docs/index.html) LLVM Compiler
- [**Make:**](https://www.gnu.org/software/make/manual/make.html) Build automation tool
- [**libuv:**](https://libuv.org/) Cross-platform asynchronous I/O library
- [**cURL:**](https://curl.se/docs/manpage.html) Command-line tool for fetching input data

---

## Contributing

Contributions are welcome! Here’s how you can help:
- Provide feedback on improving the directory structure or code readability.
- Submit pull requests to optimize solutions or address issues.
- Add test cases to improve solution coverage and robustness.

---

## Troubleshooting

### Common Issues:
1. **Executable Not Found:**
   - Ensure the `setup.sh` script completed without errors.
   - Verify the stack is in your `PATH` using `which <command>`.

2. **Input Data Missing:**
   - Use the `aoc` command to fetch input data for specific days.

3. **Compiler Conflicts:**
   - Ensure GCC (`gcc2`) and Clang (`clang2`) compilers are correctly configured.

For additional help, submit an issue or ask for support.

---

Happy Coding! =)
