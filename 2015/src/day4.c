// Day 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include "day4.h"
#include "main.h"

#define INPUT_FILE "data/4.txt"

// Check if hash starts with 5 zeros
static inline int starts_with_5_zeros(const unsigned char hash[16]) {
    return hash[0] == 0 && hash[1] == 0 && (hash[2] & 0xF0) == 0;
}

// Check if hash starts with 6 zeros
static inline int starts_with_6_zeros(const unsigned char hash[16]) {
    return hash[0] == 0 && hash[1] == 0 && hash[2] == 0;
}

// Append a number to a string
static int append_number(char *dest, int num) {
    return sprintf(dest, "%d", num);
}

// Solve the problem for a given condition
static int solve(const char *input, size_t length, int start,
                 int (*starts_with)(const unsigned char hash[16])) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    char buffer[256];
    memcpy(buffer, input, length);
    char *numStart = buffer + length;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        perror("Error creating EVP_MD_CTX");
        return -1;
    }

    for (int i = start;; ++i) {
        int numLength = append_number(numStart, i);

        if (EVP_DigestInit_ex(mdctx, EVP_md5(), NULL) != 1 ||
            EVP_DigestUpdate(mdctx, buffer, length + numLength) != 1 ||
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

    printf("Part 1: %d\n", part1);
    printf("Part 2: %d\n", part2);
    printf("Execution Time: %.6f seconds\n", elapsed_time);

    free(input);
}
