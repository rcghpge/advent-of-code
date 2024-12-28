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
  echo "Clang compiler is not installed. Installing Clang compiler..."
  sudo apt-get install clang -y
else
  echo "Clang compiler is already installed."
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
  echo "Cloning libuv..."
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

# Compile the project with both GCC and Clang
echo "Compiling directory with GCC..."
make gcc
echo "Compiling directory with Clang compiler..."
make clang

echo "Setup complete! You can now run your solutions from the build_gcc and build_clang directories."

