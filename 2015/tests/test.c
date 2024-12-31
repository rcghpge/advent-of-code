// Test setup environment
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "main.h" // Include run_day and run_all_days
#include "aoc.h"  // Include fetch_day_data function
#include "day1.h"

// Function to read the session cookie from AOC_SESSION_COOKIE
const char *get_session_cookie() {
    static char session_cookie[512];
    FILE *file = fopen("AOC_SESSION_COOKIE", "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open AOC_SESSION_COOKIE. Ensure setup.sh has been run.\n");
        exit(EXIT_FAILURE);
    }

    if (!fgets(session_cookie, sizeof(session_cookie), file)) {
        fprintf(stderr, "Error: Failed to read session cookie from AOC_SESSION_COOKIE.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    // Remove trailing newline, if any
    size_t len = strlen(session_cookie);
    if (len > 0 && session_cookie[len - 1] == '\n') {
        session_cookie[len - 1] = '\0';
    }
    return session_cookie;
}

// Test individual days using run_day from main.c
void test_day(int day) {
    printf("Testing Day %d...\n", day);
    run_day(day);
    printf("Day %d passed.\n", day);
}

// Test all days using run_all_days from main.c
void test_all_days() {
    printf("Testing all days...\n");
    run_all_days();
    printf("All days passed.\n");
}

// Test data-fetching utilities
void test_fetch_day(int year, int day) {
    printf("Testing data-fetching for Day %d, Year %d...\n", day, year);

    // Fetch data for the specific day
    fetch_day_data(year, day, get_session_cookie());

    // Validate that the input file exists
    char filename[256];
    snprintf(filename, sizeof(filename), "../data/%d.txt", day);

    FILE *file = fopen(filename, "r");
    assert(file != NULL && "Error: Input file not found. Data-fetching failed.");
    fclose(file);

    printf("Data-fetching for Day %d passed.\n", day);
}

// Main function to run all tests
int main() {
    printf("Running all tests for Advent of Code...\n");

    // Test data-fetching for Day 1
    test_fetch_day(2015, 1);

    // Test individual days
    for (int day = 1; day <= 25; day++) {
        test_day(day);
    }

    // Test running all days
    test_all_days();

    printf("All tests passed successfully.\n");
    return 0;
}

