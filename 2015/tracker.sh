#!/bin/bash

# Global variables
TRACKER_INSTALL_PATH="/usr/local/bin/tracker"
TRACKER_SCRIPT=$(realpath "$0") # Full path of this script

# Debug mode (set to "true" to enable, "false" to disable)
DEBUG=false # set to true or fals to debug tracker 

# Enable debugging if DEBUG is true
if [ "$DEBUG" == "true" ]; then
    set -x  # Enables debug mode (prints each command before executing)
fi

# Function to install tracker globally
install_tracker() {
    echo "Installing tracker globally..."

    if [ -f "$TRACKER_INSTALL_PATH" ]; then
        echo "Tracker is already installed at $TRACKER_INSTALL_PATH."
        echo "To reinstall or update, remove it and run this script again."
    else
        sudo chmod +x "$TRACKER_SCRIPT" || { echo "Error: Failed to change permissions."; exit 1; }
        sudo cp "$TRACKER_SCRIPT" "$TRACKER_INSTALL_PATH" || { echo "Error: Failed to copy script."; exit 1; }
        echo "Tracker installed at $TRACKER_INSTALL_PATH."
        echo "You can now run 'tracker' globally."
    fi

    # Exit after installation to prevent running the tracker
    exit 0
}

# Function to track changes and rebuild selectively
track_and_rebuild() {
    echo "Tracking changes in 'day*.c', 'main.c', and related files..."
    find src/ include/ Makefile -type f \( -name "day*.c" -o -name "main.c" -o -name "*.h" \) | entr -c bash -c "
        echo \"$(date): Changes detected. Rebuilding impacted targets...\";
        make gcc2 clang2 aoc main all || { echo \"$(date): Error: Build failed. Check your code.\"; exit 1; }
        echo \"$(date): Rebuild complete. Use commands like gcc2, clang2, or main to test your solutions.\";
    "
}

# Function to run a single rebuild
single_run() {
    echo "Running a single rebuild of 'day*.c', 'main.c', and related files..."

    echo "Rebuilding impacted targets..."
    make gcc2 clang2 aoc main all || { echo 'Error: Build failed. Check your code.'; exit 1; }
    echo "Rebuild complete. Use commands like gcc2, clang2, or main to test your solutions."
}

# Function to display help information
show_help() {
    echo "Usage: tracker [install | --single-run | --help]"
    echo "install        Install tracker globally at /usr/local/bin."
    echo "--single-run   Run the rebuild process once and exit."
    echo "--help         Display this help message."
}

# Verify dependencies
verify_dependencies() {
    echo "Verifying dependencies..."
    local commands=("make" "entr")
    for cmd in "${commands[@]}"; do
        if ! command -v "$cmd" &> /dev/null; then
            echo "Error: '$cmd' is not installed or not in your PATH."
            echo "Please install '$cmd' before running this script."
            exit 1
        fi
    done
    echo "All dependencies are verified."
}

# Trap to handle Ctrl+C
trap "echo 'File tracker has been stopped.'; exit 0" SIGINT

# Main script execution
main() {
    # Automatically install tracker if not already installed
    if [ "$0" != "$TRACKER_INSTALL_PATH" ]; then
        echo "Tracker is not installed. Installing now..."
        install_tracker
    fi

    # Parse command-line arguments
    case "$1" in
        install)
            install_tracker
            ;;
        --single-run)
            verify_dependencies
            single_run
            exit 0
            ;;
        --help)
            show_help
            exit 0
            ;;
        "")
            verify_dependencies
            echo "File tracker is actively monitoring. Press Ctrl+C to stop."
            track_and_rebuild
            ;;
        *)
            echo "Error: Unknown option '$1'. Use --help for usage information."
            exit 1
            ;;
    esac
}

# Execute the main function with arguments
main "$@"

