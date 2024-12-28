# Advent of Code 2015 - C Language
Welcome to the Advent of Code 2015 directory in the C programming language! This directory contains solutions to the Advent of Code 2015 challenges written in C. The directory is designed to support both GCC and Clang compilers, ensuring portability and flexibility.

---
## Directory Structure

```
makefile
Copy code
2015/
├── Makefile            # Automates building, running, and cleaning the project
├── README.md           # Documentation of directory
├── setup.sh            # Script to set up the environment and dependencies
├── data/               # Input files for each challenge
│   ├── 1.txt           # Input for Day 1
│   └── ...             # Inputs for Days 2-25
├── examples/           # Example scripts and binaries (e.g., libuv timer demo)
│   ├── example         # Compiled binary for example.c
│   ├── example.c       # timer example
│   ├── hello           # Compiled binary for hello.c
│   └── hello.c         # Simple "Hello, World!" example
├── include/            # Header files
│   ├── day1.h
│   └── ...             # Headers for Days 3–25
├── lib/                # External libraries (e.g., libuv)
├── src/                # Source files for each day's solution
│   ├── main.c          # Main entry point
│   ├── day1.c          # Solution for Day 1
│   └── ...             # Solutions for Days 3–25
└── tests/              # Test files (optional)
```
---
## Setup Instructions
### Step 1: Run the Setup Script
Run the setup script to initialize your environment:
```
bash
Copy code
./setup.sh
```
This script:
- Ensures **GCC**, **Clang**, and **Make** are installed.
- Downloads and builds **libuv** if needed.
- Creates necessary directories (`data/`, `build_gcc/`, `build_clang/`).
- Validates input files in the `data/` directory.

### Step 2: Add Input Files
Pull input data for each day in the data/ directory:
- `1.txt`
- ...
- `25.txt`

---
## Building and Running
### Build the Project
To build the project with both GCC and Clang, use the following command:
```
bash
Copy code
make all
```

### Run the Solutions
- Run the GCC-compiled executable:
```
bash
Copy code
make run_gcc
```
- Run the Clang-compiled executable:
```
bash
Copy code
make run_clang
```
- Run both builds sequentially:
```
bash
Copy code
make run_all
```
### Clean the Project
To remove all build artifacts and start fresh:
```
bash
Copy code
make clean
```
---
## Input Validation
The Makefile validates that the input files are present in the data/ directory:

- If no input files are found:
```
yaml
Copy code
Error: No input files found in data/.
```
- If only some files are present, warnings are displayed:
```
kotlin
Copy code
Warning: Input file data/2.txt is missing.
Input validation complete: Found 1 out of 25 files.
```
---
## Contributing
You are welcome to contribute by:
- Providing feedback on how to improve overall directory and repository..
- Improving code readability or efficiency.
- Submitting issues or pull requests.

---
### Dependencies
This project depends on the following tools and libraries:
- **GCC:** GNU Compiler Collection
- **Clang:** LLVM Compiler
- **Make:** Build automation tool
- **libuv:** Cross-platform asynchronous I/O library

---
Happy coding! 
