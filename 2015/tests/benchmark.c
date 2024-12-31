// Benchmark
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h" // Include run_day and run_all_days

// Function to measure elapsed time in seconds
static double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Benchmark individual day
void benchmark_day(int day) {
    printf("Benchmarking Day %d...\n", day);

    double start_time = get_time_in_seconds();

    // Run the solution for the specific day
    run_day(day);

    double elapsed_time = get_time_in_seconds() - start_time;
    printf("Day %d completed in %.6f seconds.\n", day, elapsed_time);
}

// Benchmark all days
void benchmark_all_days() {
    printf("Benchmarking all days...\n");

    double start_time = get_time_in_seconds();

    // Run solutions for all implemented days
    run_all_days();

    double elapsed_time = get_time_in_seconds() - start_time;
    printf("All days completed in %.6f seconds.\n", elapsed_time);
}

// Benchmark a range of days
void benchmark_days_range(int start_day, int end_day) {
    printf("Benchmarking days from %d to %d...\n", start_day, end_day);

    for (int day = start_day; day <= end_day; day++) {
        benchmark_day(day);
    }

    printf("Completed benchmarking range %d to %d.\n", start_day, end_day);
}

// Main function for benchmarking
int main(int argc, char *argv[]) {
    printf("Starting Advent of Code Benchmarking...\n");

    if (argc == 1) {
        // Default to benchmarking all days if no arguments are provided
        benchmark_all_days();
    } else if (argc == 2) {
        // Benchmark a single day
        int day = atoi(argv[1]);
        if (day < 1 || day > 25) {
            fprintf(stderr, "Error: Day %d is out of range (1-25).\n", day);
            return EXIT_FAILURE;
        }
        benchmark_day(day);
    } else if (argc == 3) {
        // Benchmark a range of days
        int start_day = atoi(argv[1]);
        int end_day = atoi(argv[2]);
        if (start_day < 1 || end_day > 25 || start_day > end_day) {
            fprintf(stderr, "Error: Invalid range %d-%d. Valid range is between 1 and 25.\n", start_day, end_day);
            return EXIT_FAILURE;
        }
        benchmark_days_range(start_day, end_day);
    } else {
        fprintf(stderr, "Usage: %s [day | start_day end_day]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Benchmarking completed successfully.\n");
    return EXIT_SUCCESS;
}

