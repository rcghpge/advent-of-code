// Main script
#include <limits.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"
#include "day7.h"
#include "day8.h"
#include "day9.h"
#include "day10.h"
#include "day11.h"
#include "day12.h"
#include "day13.h"
#include "day14.h"
#include "day15.h"
#include "day16.h"
#include "day17.h"
#include "day18.h"
#include "day19.h"
#include "day20.h"
#include "day21.h"
#include "day22.h"
#include "day23.h"
#include "day24.h"
#include "day25.h"

void run_day(int day) {
    printf("Running solution for Day %d...\n", day);

    // Map days to their respective solution functions
    switch (day) {
        case 1: run_day1(); break;
        case 2: run_day2(); break;
        case 3: run_day3(); break;
        case 4: run_day4(); break;
        case 5: run_day5(); break;
        case 6: run_day6(); break;
        case 7: run_day7(); break;
        case 8: run_day8(); break;
        case 9: run_day9(); break;
        case 10: run_day10(); break;
        case 11: run_day11(); break;
        case 12: run_day12(); break;
        case 13: run_day13(); break;
        case 14: run_day14(); break;
        case 15: run_day15(); break;
        case 16: run_day16(); break;
        case 17: run_day17(); break;
        case 18: run_day18(); break;
        case 19: run_day19(); break;
        case 20: run_day20(); break;
        case 21: run_day21(); break;
        case 22: run_day22(); break;
        case 23: run_day23(); break;
        case 24: run_day24(); break;
        case 25: run_day25(); break;
        default:
            fprintf(stderr, "Error: No solution implemented for Day %d\n", day);
            break;
    }

    printf("Completed solution for Day %d.\n", day);
}

void run_all_days() {
    printf("Running solutions for all implemented days...\n");

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    int implemented_days[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25
    };
    int num_days = sizeof(implemented_days) / sizeof(implemented_days[0]);

    for (int i = 0; i < num_days; i++) {
        int day = implemented_days[i];
        printf("Running solution for Day %d (%d/%d)...\n", day, i + 1, num_days);
        run_day(day);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Completed all implemented days.\n");
    printf("Total Execution Time: %.6f seconds\n", elapsed_time);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <day> | all\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "all") == 0) {
        run_all_days();
        return EXIT_SUCCESS;
    }

    char *endptr;
    long day = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || day < 1 || day > 25) {
        fprintf(stderr, "Error: Invalid day '%s'. Please enter a day between 1 and 25.\n", argv[1]);
        return EXIT_FAILURE;
    }

    run_day((int)day);
    return EXIT_SUCCESS;
}

