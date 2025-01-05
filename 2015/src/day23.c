// Day 23 - Opening the Turing Lock
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "day23.h"

#define MAX_INSTRUCTIONS 100
#define MAX_LENGTH 20

// Part 1 and Part 2

int execute_instructions(int registers[2], int num_instructions, char instructions[MAX_INSTRUCTIONS][MAX_LENGTH], int initial_value_a) {
    int ip = 0;
    registers[0] = initial_value_a;  // Set register a to initial value for the part
    registers[1] = 0;  // Register b always starts at 0

    while (ip < num_instructions) {
        char instruction[MAX_LENGTH];
        strcpy(instruction, instructions[ip]);
        char *token = strtok(instruction, " ");

        if (strcmp(token, "hlf") == 0 || strcmp(token, "tpl") == 0 || strcmp(token, "inc") == 0) {
            // Handle hlf, tpl, and inc (modifying a register)
            token = strtok(NULL, " ");
            int reg = token[0] - 'a';

            if (strcmp(instruction, "hlf") == 0) {
                registers[reg] /= 2;
            } else if (strcmp(instruction, "tpl") == 0) {
                registers[reg] *= 3;
            } else if (strcmp(instruction, "inc") == 0) {
                registers[reg] += 1;
            }
            ip += 1;
        } else if (strcmp(token, "jmp") == 0) {
            token = strtok(NULL, " ");
            ip += atoi(token);
        } else if (strcmp(token, "jie") == 0 || strcmp(token, "jio") == 0) {
            // Handle jie and jio (conditional jumps)
            token = strtok(NULL, ", ");
            int reg = token[0] - 'a';
            token = strtok(NULL, " ");
            int offset = atoi(token);

            if ((strcmp(instruction, "jie") == 0 && registers[reg] % 2 == 0) || 
                (strcmp(instruction, "jio") == 0 && registers[reg] == 1)) {
                ip += offset;
            } else {
                ip += 1;
            }
        }
    }

    return registers[1];
}

void run_day23() {
    FILE *file = fopen("data/23.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char instructions[MAX_INSTRUCTIONS][MAX_LENGTH];
    int num_instructions = 0;

    // Read input instructions from input data
    while (fgets(instructions[num_instructions], MAX_LENGTH, file) != NULL) {
        // Remove newline character if present
        instructions[num_instructions][strcspn(instructions[num_instructions], "\n")] = '\0';
        num_instructions++;
    }

    fclose(file);

    // Measure execution time
    clock_t start = clock();

    // Execute Part 1 (initializing register a to 0)
    int registers[2] = {0, 0};
    int result_part_1 = execute_instructions(registers, num_instructions, instructions, 0);
    printf("Part 1 - Value in register b: %d\n", result_part_1);

    // Execute Part 2 (initializing register a to 1)
    int result_part_2 = execute_instructions(registers, num_instructions, instructions, 1);
    printf("Part 2 - Value in register b 2: %d\n", result_part_2);

    // Measure and print total execution time
    clock_t end = clock();
    printf("Execution time: %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

