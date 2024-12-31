#!/bin/bash

echo "Setting up Advent of Code environment..."

# Ensure necessary directories exist
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
        echo "Setting session cookie in ~/.bashrc..."
        echo "export AOC_SESSION_COOKIE=\"$cookie\"" >> ~/.bashrc
        source ~/.bashrc
    else
        echo "Error: No session cookie provided. Exiting setup."
        exit 1
    fi
else
    echo "Using existing Advent of Code session cookie."
fi

# Clean previous builds
echo "Cleaning previous builds..."
make clean || { echo "Error: Clean failed. Check the Makefile."; exit 1; }

# Build and install everything
echo "Building and installing executables..."
make all || { echo "Error: Build failed. Check the Makefile and sources."; exit 1; }
make install || { echo "Error: Installation failed. Check permissions."; exit 1; }

# Install tracker globally
TRACKER_SCRIPT="tracker.sh"
TRACKER_INSTALL_PATH="/usr/local/bin/tracker"

if [ -f "$TRACKER_INSTALL_PATH" ]; then
    echo "Tracker is already installed at $TRACKER_INSTALL_PATH."
    read -p "Do you want to overwrite the existing tracker? (y/n): " overwrite
    if [[ "$overwrite" =~ ^[Yy]$ ]]; then
        echo "Overwriting existing tracker..."
        if [ -f "$TRACKER_SCRIPT" ]; then
            sudo chmod +x "$TRACKER_SCRIPT" || { echo "Error: Failed to change permissions for $TRACKER_SCRIPT."; exit 1; }
            sudo cp "$TRACKER_SCRIPT" "$TRACKER_INSTALL_PATH" || { echo "Error: Failed to copy $TRACKER_SCRIPT to $TRACKER_INSTALL_PATH."; exit 1; }
            echo "Tracker has been updated."
        else
            echo "Error: $TRACKER_SCRIPT not found. Cannot update tracker."
        fi
    else
        echo "Skipping tracker installation."
    fi
else
    echo "Installing tracker..."
    if [ -f "$TRACKER_SCRIPT" ]; then
        sudo chmod +x "$TRACKER_SCRIPT" || { echo "Error: Failed to change permissions for $TRACKER_SCRIPT."; exit 1; }
        sudo cp "$TRACKER_SCRIPT" "$TRACKER_INSTALL_PATH" || { echo "Error: Failed to copy $TRACKER_SCRIPT to $TRACKER_INSTALL_PATH."; exit 1; }
        echo "Tracker installed."
    else
        echo "Error: $TRACKER_SCRIPT not found. Cannot install tracker."
    fi
fi

# Verify installation and environment
echo "Verifying installation..."
commands=("gcc2" "clang2" "main" "aoc" "example" "hello" "tracker")
missing_cmds=0

for cmd in "${commands[@]}"; do
    if ! command -v "$cmd" &> /dev/null; then
        echo "Warning: '$cmd' command is not globally available."
        missing_cmds=$((missing_cmds + 1))
    else
        echo "Verified '$cmd' command is available."
    fi
done

# Warn if any commands are missing
if [ $missing_cmds -ne 0 ]; then
    echo "Warning: Some commands are missing. Ensure executables are correctly installed and PATH is updated."
else
    echo "All commands are successfully installed and available globally."
fi

# Verify and update PATH
echo "Current PATH: $PATH"
if ! grep -q "/usr/local/bin" <<< "$PATH"; then
    echo "Adding /usr/local/bin to PATH..."
    echo 'export PATH=/usr/local/bin:$PATH' >> ~/.bashrc
    source ~/.bashrc
    echo "PATH updated successfully!"
else
    echo "/usr/local/bin is already in PATH."
fi

# Final message
echo
echo "============================================="
echo "🎉 Setup Complete!"
echo "============================================="
echo
echo "Your Advent of Code environment is ready. Use commands like:"
echo "  tracker      # Start tracker for AoC workflow changes"
echo "  aoc 1 2021       # Fetch input for Day 1, Year 2021"
echo "  aoc all 2021     # Fetch inputs for all days, Year 2021"
echo "  gcc2 1         # Run Day 1 solution with GCC"
echo "  clang2 1       # Run Day 1 solution with Clang"
echo "  main all         # Run all solutions with both GCC and Clang"
echo "  example          # Run the libuv example program"
echo "  hello            # Run the Hello World example program"
echo
echo "To reset the session cookie, set the environment variable AOC_SESSION_COOKIE or re-run this script."
