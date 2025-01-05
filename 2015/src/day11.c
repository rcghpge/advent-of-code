// Day 11 - Corporate Policy
#include "main.h"
#include "day11.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT_LEN 64  // Max length of password for flexibility

// Read the input password from a file
static void read_input_from_file(const char *filename, char *buffer, size_t max_len) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    if (!fgets(buffer, max_len, file)) {
        perror("Failed to read input file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';  // Strip newline characters
    fclose(file);
}

// Reset characters to default sequence after invalid characters
static inline void default_sequence(char *pwd, const size_t start, const size_t len) {
    for (size_t i = start; i < len; ++i) {
        pwd[i] = 'a';
    }
}

// Increment password by one, handling wrap-around from 'z' to 'a'
static inline void increase(char *pwd, const size_t len) {
    for (size_t i = len - 1; i < len; --i) {
        if (pwd[i] == 'z') {
            pwd[i] = 'a';
        } else {
            pwd[i]++;
            break;
        }
    }
}

// Check for invalid characters and reset subsequent characters if found
static inline bool invalid_character_check(char *pwd, const size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (pwd[i] == 'i' || pwd[i] == 'o' || pwd[i] == 'l') {
            pwd[i]++;
            default_sequence(pwd, i + 1, len);
            return false;
        }
    }
    return true;
}

// Check for at least one increasing straight of three consecutive letters
static inline bool increasing_pattern_check(const char *pwd, const size_t len) {
    int increasingPatternCount = 1;
    for (size_t i = 0; i < len - 1; ++i) {
        if (pwd[i] + 1 == pwd[i + 1]) {
            increasingPatternCount++;
            if (increasingPatternCount >= 3) return true;
        } else {
            increasingPatternCount = 1;
        }
    }
    return false;
}

// Check for at least two different, non-overlapping pairs of characters
static inline bool different_pairs_check(const char *pwd, const size_t len) {
    int differentPairsCount = 0;
    char previous = '\0';
    for (size_t i = 0; i < len - 1; ++i) {
        if (pwd[i] == pwd[i + 1] && pwd[i] != previous) {
            differentPairsCount++;
            previous = pwd[i];
            i++;  // Skip second character of pair
        }
    }
    return differentPairsCount >= 2;
}

// Generate next valid password based on rules
static void next_password(const char *pwd, char *output, const size_t len) {
    memcpy(output, pwd, len);
    for (;;) {
        increase(output, len);
        if (invalid_character_check(output, len) &&
            increasing_pattern_check(output, len) &&
            different_pairs_check(output, len)) {
            break;
        }
    }
}

void run_day11() {
    clock_t start_time = clock();  // Start timer

    const char *input_file = "data/11.txt";  // Default input file
    char input[MAX_INPUT_LEN] = {0};

    read_input_from_file(input_file, input, MAX_INPUT_LEN);
    size_t input_len = strlen(input);

    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        exit(EXIT_FAILURE);
    }

    char part1[MAX_INPUT_LEN + 1] = {0};
    char part2[MAX_INPUT_LEN + 1] = {0};

    next_password(input, part1, input_len);   // Generate first valid password
    next_password(part1, part2, input_len);  // Generate second valid password

    printf("Part 1 - First password: %.*s\n", (int)input_len, part1);
    printf("Part 2 - Second password: %.*s\n", (int)input_len, part2);

    clock_t end_time = clock();  // End timer
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

