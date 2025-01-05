# Advent of Code 2015 - C Programming Language

Welcome to my **Advent of Code 2015** solutions written in **C**! This directory provides robust and optimized solutions for Advent of Code 2015, with support for both **GCC** and **Clang** compilers. The project emphasizes performance, modularity, and clarity.

---

## Directory Structure

```plaintext
2015/
├── Makefile        # Automates building, running, cleaning, and installing the project
├── README.md       # Project documentation
├── setup.sh        # Script to set up the environment and dependencies
├── tracker.sh      # Script to monitor and log changes in the workflow
├── data/           # Input files for each challenge (1.txt, 2.txt, ..., 25.txt)
├── examples/       # Example C programs (e.g., hello.c, example.c)
├── include/        # Header files (day1.h, day2.h, ..., day25.h)
├── lib/            # External libraries (e.g., libuv, aoc utilities)
├── src/            # Source files for each day's solution (day1.c, ..., day25.c)
└── tests/          # Test files for verifying correctness and performance
```

---

## Setup Instructions
### Step 1: Initialize the Workflow Environment
Run the `setup.sh` script to configure your environment:

```bash
./setup.sh
```

The `setup.sh` script:
- Ensures **GCC**, **Clang**, **Make**, and other dependencies are installed.
- Configures the **Advent of Code session cookie** for automated input fetching.
- Creates necessary directories (`data/`, `build/`, `examples/`).
- Builds and installs tools (e.g., `gcc2`, `clang2`, `main`, etc.).

Use `./setup.sh -v` for verbose output during setup.

### Step 2: Monitor Workflow Changes (Optional)
Use `tracker.sh` to monitor workflow changes:

```bash
tracker
trackr --help
```

The `tracker.sh` script logs changes in files, directories, and build artifacts to help debug issues and track progress.

---

## Building and Running Solutions
### Build the Project
To build all components (e.g., `aoc`, `gcc2`, `clang2`, examples):

```bash
make all
```

Install the workflow environment:

```bash
make install
```
### Run Specific Solutions
- Run Day 1 solution with GCC:

```bash
gcc2 1
```

- Run Day 1 solution with Clang:

```bash
clang2 1
```

- Run all solutions with both compilers:

```bash
main all
```

### Run example programs in the `examples/` directory:
```bash
example       # Run the libuv timer example
hello         # Run the Hello World example
```

### Clean Workflow Environment
Remove all build artifacts and reset the environment:

```bash
make clean
```

---

## Performance Monitoring
### Measure Execution Time
The `main`, `gcc2`, and `clang2` commands provide total execution times:
- Example for running all solutions:

```bash
main all
```

Output:

```plaintext
Running all solutions for GCC2...
...
Total Execution Time for GCC2: 3.542 seconds

Running all solutions for CLANG2...
...
Total Execution Time for CLANG2: 3.482 seconds
```

---

## Dependencies
- [GCC](https://gcc.gnu.org/): GNU Compiler Collection
- [Clang](https://clang.llvm.org/): LLVM Compiler
- [Make](https://www.gnu.org/software/make/): Build automation tool
- [libuv](https://libuv.org/): Cross-platform asynchronous I/O library
- [cURL](https://libuv.org/)Command-line tool for fetching input data

---

## Contributing
Contributions are welcome! Here's how you can help:
- Optimize solutions for performance or clarity.
- Add test cases to improve coverage and robustness.
- Submit pull requests for improvements or new features.
- Tracker is a new toolchain I built. It can become general purpose.

---

## Troubleshooting
### Common Issues
1. Executable Not Found:
- Ensure `setup.sh` completed without errors.
- Verify the stack is in your `PATH` using `which <command>`.

2. Input Data Missing:
- Use the `aoc` command to fetch input data for specific days.

3. Compiler Conflicts:
- Ensure GCC(`gcc2`) and Clang(`clang2`) compilers are installed correctly.

4. Permission Errors:
- Ensure proper permissions for the `setup.sh` script and executables (`chmod +x setup.sh`).

---
Happy Coding!
