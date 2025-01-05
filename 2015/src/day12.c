// Day 12 - JSAbacusFramework.io
#include "main.h"
#include "day12.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Forward declaration of parse_object
static int parse_object(const char **input, int ignore_red);

// Helper function to skip whitespace
static void skip_whitespace(const char **input) {
    while (isspace(**input)) {
        (*input)++;
    }
}

// Helper function to parse numbers
static int parse_number(const char **input) {
    char *end;
    int value = (int)strtol(*input, &end, 10);
    *input = end;
    return value;
}

// Helper function to skip strings
static void skip_string(const char **input) {
    (*input)++; // Skip opening '"'
    while (**input && **input != '"') {
        if (**input == '\\') {
            (*input)++; // Skip escaped character
        }
        (*input)++;
    }
    if (**input == '"') {
        (*input)++; // Skip closing '"'
    } else {
        fprintf(stderr, "Error: Unterminated string in JSON.\n");
        exit(EXIT_FAILURE);
    }
}

// Recursive function to sum values in JSON arrays
static int parse_array(const char **input, int ignore_red) {
    int sum = 0;
    (*input)++; // Skip '['
    skip_whitespace(input);

    while (**input != ']') {
        skip_whitespace(input);

        if (**input == '{') {
            sum += parse_object(input, ignore_red);
        } else if (**input == '[') {
            sum += parse_array(input, ignore_red);
        } else if (isdigit(**input) || **input == '-') {
            sum += parse_number(input);
        } else if (**input == '"') {
            skip_string(input); // Skip over strings
        } else {
            fprintf(stderr, "Error: Unexpected character '%c' in JSON array.\n", **input);
            exit(EXIT_FAILURE);
        }

        skip_whitespace(input);

        if (**input == ',') {
            (*input)++; // Skip ','
        } else if (**input != ']') {
            fprintf(stderr, "Error: Expected ',' or ']' but found '%c'\n", **input);
            exit(EXIT_FAILURE);
        }
    }

    (*input)++; // Skip closing ']'
    return sum;
}

// Recursive function to sum values in JSON objects
static int parse_object(const char **input, int ignore_red) {
    int sum = 0;
    int contains_red = 0;
    (*input)++; // Skip '{'

    while (**input != '}') {
        skip_whitespace(input);

        if (**input == '"') {
            skip_string(input); // Skip key string

            skip_whitespace(input);

            if (**input == ':') {
                (*input)++; // Skip ':'
                skip_whitespace(input);

                if (**input == '"') {
                    if (ignore_red && strncmp(*input + 1, "red", 3) == 0 && (*input)[4] == '"') {
                        contains_red = 1;
                    }
                    skip_string(input); // Skip value string
                } else if (**input == '{') {
                    sum += parse_object(input, ignore_red);
                } else if (**input == '[') {
                    sum += parse_array(input, ignore_red);
                } else if (isdigit(**input) || **input == '-') {
                    sum += parse_number(input);
                }
            }
        }

        skip_whitespace(input);

        if (**input == ',') {
            (*input)++; // Skip ','
        } else if (**input != '}') {
            fprintf(stderr, "Error: Expected ',' or '}' but found '%c'\n", **input);
            exit(EXIT_FAILURE);
        }
    }

    (*input)++; // Skip closing '}'
    return contains_red ? 0 : sum;
}

// Entry point for parsing JSON
static int parse_json(const char *input, int ignore_red) {
    skip_whitespace(&input);
    if (*input == '{') {
        return parse_object(&input, ignore_red);
    } else if (*input == '[') {
        return parse_array(&input, ignore_red);
    } else {
        fprintf(stderr, "Error: Expected '{' or '[' at root but found '%c'\n", *input);
        exit(EXIT_FAILURE);
    }
}

// Framework-compatible function to execute Day 12 solution
void run_day12() {
    clock_t start_time = clock();

    const char *input_file = "data/12.txt";
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *json_data = malloc(length + 1);
    if (!json_data) {
        perror("Failed to allocate memory for input");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(json_data, 1, length, file);
    json_data[length] = '\0';
    fclose(file);

    // Solve parts
    int part1 = parse_json(json_data, 0); // Sum all numbers
    int part2 = parse_json(json_data, 1); // Sum ignoring objects with "red"

    // Print results
    printf("Part 1 - First total sum: %d\n", part1);
    printf("Part 2 - Second total sum: %d\n", part2);

    free(json_data);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

