// Day 24 - It Hangs in the Balance
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "main.h"
#include "day24.h"

#define MAX_PACKAGES 100

long long calculateQuantumEntanglement(int *packages, int size) {
    long long product = 1;
    for (int i = 0; i < size; i++) {
        product *= packages[i];
    }
    return product;
}

void backtrack(int idx, int groupWeight, int groupSize, int *currentGroup, int *packages, int numPackages, int targetWeight, long long *minQuantumEntanglement, int *minPackages) {
    if (groupWeight == targetWeight) {
        long long quantumEntanglement = calculateQuantumEntanglement(currentGroup, groupSize);
        if (groupSize < *minPackages || (groupSize == *minPackages && quantumEntanglement < *minQuantumEntanglement)) {
            *minPackages = groupSize;
            *minQuantumEntanglement = quantumEntanglement;
        }
        return;
    }

    if (groupWeight > targetWeight || idx == numPackages) {
        return;
    }

    for (int i = idx; i < numPackages; i++) {
        currentGroup[groupSize] = packages[i];
        backtrack(i + 1, groupWeight + packages[i], groupSize + 1, currentGroup, packages, numPackages, targetWeight, minQuantumEntanglement, minPackages);
    }
}

long long findIdealConfiguration(int *packages, int numPackages, int numGroups) {
    int totalWeight = 0;
    for (int i = 0; i < numPackages; i++) {
        totalWeight += packages[i];
    }

    if (totalWeight % numGroups != 0) {
        return -1;
    }

    int targetWeight = totalWeight / numGroups;
    int minPackages = numPackages;
    long long minQuantumEntanglement = LLONG_MAX;
    int currentGroup[MAX_PACKAGES];

    backtrack(0, 0, 0, currentGroup, packages, numPackages, targetWeight, &minQuantumEntanglement, &minPackages);

    return minQuantumEntanglement;
}

void run_day24(void) {
    FILE *file = fopen("data/24.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    int packages[MAX_PACKAGES];
    int numPackages = 0;

    while (fscanf(file, "%d", &packages[numPackages]) != EOF) {
        numPackages++;
    }
    fclose(file);

    clock_t start, end;

    start = clock();
    long long part1QuantumEntanglement = findIdealConfiguration(packages, numPackages, 3);
    if (part1QuantumEntanglement != -1) {
        printf("Part 1 - Quantum entanglement: %lld\n", part1QuantumEntanglement);
    }

    long long part2QuantumEntanglement = findIdealConfiguration(packages, numPackages, 4);
    if (part2QuantumEntanglement != -1) {
        printf("Part 2 - Quantum entanglement 2: %lld\n", part2QuantumEntanglement);
    }
    end = clock();

    printf("Execution time: %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

