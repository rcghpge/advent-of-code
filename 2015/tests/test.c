// Test setup environment
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "main.h" // Include run_day and run_all_days
#include "aoc.h"  // Include fetchDayData function

// Test individual days using run_day from main.c
void test_day(int day) {
    printf("Testing Day %d...\n", day);

    // Redirect stderr to capture potential errors
    fflush(stderr);
    run_day(day);

    printf("Day %d passed.\n", day);
}

// Test all days using run_all_days from main.c
void test_all_days() {
    printf("Testing all days...\n");

    // Redirect stderr to capture potential errors
    fflush(stderr);
    run_all_days();

    printf("All days passed.\n");
}

// Test data-fetching utilities
void test_fetch_day(int year, int day, const char *session_cookie) {
    printf("Testing data-fetching for Day %d, Year %d...\n", day, year);

    // Fetch data for the specific day
    fetch_day_data(year, day, session_cookie);

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

    // your_session_cookie_here
    const char *session_cookie = "53616c7465645f5fe7a4a54e6a0f4e69c521a674aeba1d70ed5dfa5d9b02fe041c3216faee128f93c918210bcde1e0e25a2c674fae0612e2f01d68536037c2b4";

    // Test data-fetching for Day 1
    test_fetch_day(2015, 1, session_cookie);

    // Test individual days
    for (int day = 1; day <= 25; day++) {
        test_day(day);
    }

    // Test running all days
    test_all_days();

    printf("All tests passed successfully.\n");
    return 0;
}

