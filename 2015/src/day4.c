// Day 4 - The Ideal Stocking Stuffer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <openssl/evp.h>
#include "day4.h"
#include "main.h"

#define INPUT_FILE "data/4.txt"
#define MAX_BUFFER_SIZE 1024 // Increase buffer size if necessary

// Check if hash starts with 5 zeros
static inline int starts_with_5_zeros(const unsigned char hash[16]) {
    return hash[0] == 0 && hash[1] == 0 && (hash[2] & 0xF0) == 0;
}

// Check if hash starts with 6 zeros
static inline int starts_with_6_zeros(const unsigned char hash[16]) {
    return hash[0] == 0 && hash[1] == 0 && hash[2] == 0;
}

// Efficiently append a number to a string without using sprintf
static void append_number(char *dest, int num) {
    char temp[32]; // Buffer to hold the number as a string (up to 32 digits for large numbers)
    int len = 0;

    while (num > 0) {
        temp[len++] = (num % 10) + '0';  // Convert digit to character
        num /= 10;
    }

    // Reverse string to ensure correct order
    for (int i = 0; i < len; i++) {
        dest[i] = temp[len - 1 - i];
    }
    dest[len] = '\0';  // Null-terminate the string
}

// Solve problem for a given condition
static int solve(const char *input, size_t length, int start,
                 int (*starts_with)(const unsigned char hash[16])) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    char buffer[MAX_BUFFER_SIZE];  // Increase buffer size if necessary
    memcpy(buffer, input, length);
    char *numStart = buffer + length;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        perror("Error creating EVP_MD_CTX");
        return -1;
    }

    for (int i = start;; ++i) {
        append_number(numStart, i);  // Efficiently append number to string

        if (EVP_DigestInit_ex(mdctx, EVP_md5(), NULL) != 1 ||
            EVP_DigestUpdate(mdctx, buffer, length + strlen(numStart)) != 1 ||
            EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
            perror("Error computing MD5 hash");
            EVP_MD_CTX_free(mdctx);
            return -1;
        }

        if (starts_with(hash)) {
            EVP_MD_CTX_free(mdctx);
            return i;
        }
    }

    EVP_MD_CTX_free(mdctx);
    return 0; // Should never reach
}

void run_day4() {
    FILE *file = fopen(INPUT_FILE, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *input = malloc(file_size + 1);
    if (!input) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    fread(input, 1, file_size, file);
    input[file_size] = '\0';
    fclose(file);

    // Trim trailing newline or whitespace
    input[strcspn(input, "\r\n")] = '\0';

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    int part1 = solve(input, strlen(input), 1, starts_with_5_zeros);
    int part2 = solve(input, strlen(input), part1 + 1, starts_with_6_zeros);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Part 1 - First hash: %d\n", part1);
    printf("Part 2 - Second hash: %d\n", part2);
    printf("Execution time: %.6f seconds\n", elapsed_time);

    free(input);
}

