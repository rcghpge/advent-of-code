#!/bin/bash

echo "Setting up Advent of Code 2015 directory..."

# Update system packages
echo "Updating system packages..."
sudo apt-get update -y

# Check for GCC installation
if ! [ -x "$(command -v gcc)" ]; then
  echo "GCC is not installed. Installing GCC..."
  sudo apt-get install gcc -y
else
  echo "GCC is already installed."
fi

# Check for Clang installation
if ! [ -x "$(command -v clang)" ]; then
  echo "Clang is not installed. Installing Clang..."
  sudo apt-get install clang -y
else
  echo "Clang is already installed."
fi

# Install Make if not already installed
if ! [ -x "$(command -v make)" ]; then
  echo "Make is not installed. Installing Make..."
  sudo apt-get install make -y
else
  echo "Make is already installed."
fi

# Install libuv
if [ ! -d "lib/libuv" ]; then
  echo "Cloning libuv library..."
  mkdir -p lib
  git clone https://github.com/libuv/libuv.git lib/libuv
  cd lib/libuv
  echo "Building libuv..."
  sh autogen.sh && ./configure && make && sudo make install
  cd ../../
  echo "libuv installed successfully."
else
  echo "libuv is already set up."
fi

# Ensure build directories exist
echo "Ensuring build directories exist..."
mkdir -p build_gcc build_clang

# Check for data directory and input files
if [ ! -d "data" ]; then
  echo "Creating data directory..."
  mkdir -p data
  echo "Place your input files (e.g., 1.txt, 2.txt, ..., 25.txt) in the data/ directory."
else
  echo "Checking for input data in the data directory..."
  for i in {1..25}; do
    if [ ! -f "data/$i.txt" ]; then
      echo "Warning: Input file data/$i.txt is missing."
    else
      echo "Found data/$i.txt."
    fi
  done
fi

# Compile the project with both GCC and Clang
echo "Compiling directory with GCC..."
make gcc || { echo "GCC compilation failed"; exit 1; }

echo "Compiling directory with Clang..."
make clang || { echo "Clang compilation failed"; exit 1; }

echo "Setup complete! You can now run your solutions from build_gcc and build_clang directories."
echo "Use 'make run_gcc', 'make run_clang', or 'make all' to execute the solutions, and ensure input files are in the data/ directory."

