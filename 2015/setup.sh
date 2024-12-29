#!/bin/bash

echo "Setting up Advent of Code environment..."

# Ensure build directories exist
echo "Ensuring necessary directories exist..."

for dir in build build/obj data build/examples; do
    if [ -d "$dir" ]; then
        echo "Directory '$dir' already exists. Skipping creation."
    else
        mkdir -p "$dir"
        echo "Created directory '$dir'."
    fi
done

# Prompt for Advent of Code session cookie if not already set
if [ -z "$AOC_SESSION_COOKIE" ]; then
    echo "Advent of Code session cookie not found in environment."
    read -p "Enter your Advent of Code session cookie: " cookie
    if [ -n "$cookie" ]; then
        export AOC_SESSION_COOKIE=$cookie
        echo "Persisting session cookie to ~/.bashrc..."
        echo "export AOC_SESSION_COOKIE=\"$cookie\"" >> ~/.bashrc
        source ~/.bashrc
    else
        echo "[ERROR] No session cookie provided. Exiting setup."
        exit 1
    fi
else
    echo "Using existing Advent of Code session cookie."
fi

# Clean previous builds
echo "Cleaning previous builds..."
make clean || { echo "[ERROR] Clean failed. Check the Makefile."; exit 1; }

# Build and install everything
echo "Building and installing executables..."
make all || { echo "[ERROR] Build failed. Check the Makefile and sources."; exit 1; }
make install || { echo "[ERROR] Installation failed. Check permissions."; exit 1; }

# Verify installation and environment
echo "Verifying installation..."
commands=("gcc2" "clang2" "main" "aoc" "example" "hello")
missing_cmds=0

for cmd in "${commands[@]}"; do
    if ! command -v "$cmd" &> /dev/null; then
        echo "[WARNING] '$cmd' command is not globally available."
        missing_cmds=$((missing_cmds + 1))
    else
        echo "Verified '$cmd' command is available."
    fi
done

# Warn if any commands are missing
if [ $missing_cmds -ne 0 ]; then
    echo "[WARNING] Some commands are missing. Ensure executables are correctly installed and PATH is updated."
else
    echo "All commands are successfully installed and available globally."
fi

# Verify compilers and update PATH
echo "Current PATH: $PATH"
if ! grep -q "/usr/local/bin" <<< "$PATH"; then
    echo "Adding /usr/local/bin to PATH..."
    echo 'export PATH=/usr/local/bin:$PATH' >> ~/.bashrc
    source ~/.bashrc
    echo "PATH updated successfully!"
else
    echo "/usr/local/bin is already in PATH."
fi

echo
echo "Initial workflow environment setup is complete!"
echo "Complete the final installation through make. Example usage:"
echo "  aoc 1 2021       # Fetch input for Day 1, Year 2021"
echo "  aoc all 2021     # Fetch inputs for all days, Year 2021"
echo "  gcc2 1         # Run Day 1 solution with GCC"
echo "  clang2 1       # Run Day 1 solution with Clang"
echo "  main all         # Run all solutions with both GCC and Clang"
echo "  example          # Run the libuv example program"
echo "  hello            # Run the Hello World example program"
echo
echo "To reset the session cookie, set the environment variable AOC_SESSION_COOKIE or re-run this script."

