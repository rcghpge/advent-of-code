#!/bin/bash

# Global verbosity flag
VERBOSE=false

# Print a message only in verbose mode
verbose_log() {
    if [ "$VERBOSE" = true ]; then
        echo "$1"
    fi
}

# Measure total execution time for the entire compiler run
measure_total_execution_time() {
    local cmd="$1"
    verbose_log "Measuring total execution time for: $cmd"

    local start_time=$(date +%s.%N)
    verbose_log "Starting command at $(date +'%Y-%m-%d %H:%M:%S')"
    eval "$cmd"
    local end_time=$(date +%s.%N)
    verbose_log "Finished command at $(date +'%Y-%m-%d %H:%M:%S')"

    local elapsed_time=$(echo "$end_time - $start_time" | bc)
    echo "Total Execution Time for '${cmd}': $elapsed_time seconds"
}

# Main setup function
main_setup() {
    echo "============================================="
    echo "🎉 Setting up Advent of Code environment..."
    echo "============================================="

    verbose_log "Ensuring necessary directories exist..."
    for dir in build build/obj data build/examples; do
        if [ -d "$dir" ]; then
            verbose_log "Directory '$dir' already exists. Skipping creation."
        else
            mkdir -p "$dir"
            verbose_log "Created directory '$dir'."
        fi
    done

    verbose_log "Cleaning previous builds..."
    if $VERBOSE; then
        make clean VERBOSE=1 || { echo "Error: Clean failed."; exit 1; }
    else
        make clean > /dev/null || { echo "Error: Clean failed."; exit 1; }
    fi

    verbose_log "Building all executables..."
    if $VERBOSE; then
        make all VERBOSE=1 || { echo "Error: Build failed."; exit 1; }
    else
        make all > /dev/null || { echo "Error: Build failed."; exit 1; }
    fi

    verbose_log "Installing all executables..."
    if $VERBOSE; then
        make install VERBOSE=1 || { echo "Error: Install failed."; exit 1; }
    else
        make install > /dev/null || { echo "Error: Install failed."; exit 1; }
    fi

    echo
    echo "============================================="
    echo "🎉 Setup Complete!"
    echo "============================================="
    echo
    echo "Your Advent of Code environment is ready. Use commands like:"
    echo "  main all         # Run all solutions with both GCC and Clang"
    echo "  gcc2 all         # Run all solutions with GCC"
    echo "  clang2 all       # Run all solutions with Clang"
    echo "  gcc2 <day>       # Run a specific day with GCC (e.g., gcc2 1)"
    echo "  clang2 <day>     # Run a specific day with Clang (e.g., clang2 1)"
    echo "  tracker          # Start tracker for monitoring changes in AoC workflow"
    echo
    echo "To reset the session cookie, set the environment variable AOC_SESSION_COOKIE or re-run this script."
}

# Parse command-line arguments
if [[ $# -gt 0 ]]; then
    case "$1" in
        -v|--verbose)
            VERBOSE=true
            shift
            main_setup
            ;;
        *)
            echo "Error: Unknown option '$1'."
            echo "Usage: $0 [-v|--verbose]"
            exit 1
            ;;
    esac
else
    # Default behavior: Display setup without verbose logs
    VERBOSE=false
    main_setup
fi

