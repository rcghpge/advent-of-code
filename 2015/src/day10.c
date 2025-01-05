// Day 10 - Elves Look, Elves Say
#include "main.h"
#include "day10.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

// Vector for handling dynamic arrays of characters
typedef unsigned char u8;

typedef struct {
    char *items;
    size_t length;
    size_t capacity;
} vector_char;

// Create a vector to handle dynamic arrays of characters
void vector_char_create(vector_char *v, size_t capacity) {
    assert(capacity > 0);  // Ensure initial capacity is valid
    v->items = (char *)malloc(capacity * sizeof(char));
    if (v->items == NULL) {
        perror("Failed to allocate memory for vector");
        exit(EXIT_FAILURE);
    }
    v->length = 0;
    v->capacity = capacity;
}

void vector_char_destroy(vector_char *v) {
    free(v->items);
    v->items = NULL;
    v->length = 0;
    v->capacity = 0;
}

void vector_char_push(vector_char *v, char c) {
    if (v->length >= v->capacity) {
        v->capacity *= 2;  // Double the capacity when resizing
        v->items = (char *)realloc(v->items, v->capacity * sizeof(char));
        if (v->items == NULL) {
            perror("Failed to reallocate memory for vector");
            exit(EXIT_FAILURE);
        }
    }
    v->items[v->length++] = c;
}

void vector_char_clear(vector_char *v) {
    v->length = 0;
}

// Convert input string to "Look and Say" format
void say_convert(vector_char *input, vector_char *output) {
    vector_char_clear(output);  // Clear the output vector
    const char *n = input->items;
    size_t len = input->length;

    size_t count = 1;  // Start with count 1 for first character
    for (size_t i = 1; i < len; ++i) {
        if (n[i] == n[i - 1]) {
            count++;
        } else {
            vector_char_push(output, count + '0');
            vector_char_push(output, n[i - 1]);
            count = 1;
        }
    }
    vector_char_push(output, count + '0');  // Append last group
    vector_char_push(output, n[len - 1]);
}

size_t solve(vector_char *input, vector_char *output, int iterations) {
    while (iterations-- > 0) {
        say_convert(input, output);
        vector_char *tmp = input;
        input = output;
        output = tmp;  // Swap buffers to avoid copying large data
    }
    return input->length;
}

void read_input_from_file(const char *filename, vector_char *v) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch != '\n' && ch != '\r') {
            vector_char_push(v, ch);
        }
    }

    fclose(file);
}

void run_day10() {
    clock_t start_time = clock();

    const char *input_file = "data/10.txt";  // Default input file

    vector_char numbers, buffer;
    vector_char_create(&numbers, 1 << 20);
    vector_char_create(&buffer, 1 << 20);

    read_input_from_file(input_file, &numbers);

    size_t part1 = solve(&numbers, &buffer, 40);
    size_t part2 = solve(&numbers, &buffer, 10);

    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);

    vector_char_destroy(&numbers);
    vector_char_destroy(&buffer);
}

