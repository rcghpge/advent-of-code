// Main script
<<<<<<< HEAD
#include <unistd.h>
=======
>>>>>>> origin/main
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "day1.h"

// Include other day headers as needed
// #include "day2.h"
// #include "day3.h"
// ...

void run_day(int day) {
    printf("Running solution for Day %d...\n", day);

    // Map days to their respective solution functions
    switch (day) {
        case 1:
            run_day1(); // Function for Day 1 solution
            break;
        // Add other cases as needed
        // case 2:
        //     run_day2();
        //     break;
        default:
            fprintf(stderr, "Error: No solution implemented for Day %d\n", day);
            break;
    }

    printf("Completed solution for Day %d.\n", day);
}

void run_all_days() {
    printf("Running solutions for all implemented days...\n");

    static int call_count = 0; // Track how many times this function is called
    call_count++;
    printf("Debug: run_all_days() called %d times.\n", call_count);

    // Array of implemented days
    int implemented_days[] = {1}; // Add more days as they are implemented
    int num_days = sizeof(implemented_days) / sizeof(implemented_days[0]);

    for (int i = 0; i < num_days; i++) {
        int day = implemented_days[i];
        printf("Calling run_day(%d)\n", day);
        run_day(day);
    }

    printf("Completed all implemented days.\n");
}

int main(int argc, char *argv[]) {
    // Print program start and arguments for debugging
    printf("Debug: Starting main() with PID: %d, Parent PID: %d, Arguments: ", getpid(), getppid());
    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    // Validate input arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <day> | all\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Handle "all" argument
    if (strcmp(argv[1], "all") == 0) {
        run_all_days();
        printf("Debug: Exiting main after run_all_days().\n");
        return EXIT_SUCCESS;
    }

    // Parse and validate day argument
    char *endptr;
    long day = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || day < 1 || day > 25) {
        fprintf(stderr, "Error: Invalid day '%s'. Please enter a day between 1 and 25.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Run solution for the specified day
    run_day((int)day);
    printf("Debug: Exiting main after run_day(%d).\n", (int)day);

    return EXIT_SUCCESS;
}

