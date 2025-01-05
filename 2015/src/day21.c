// Day 21 - RPG Simulator 20XX
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "day21.h"
#include "main.h"

#define NUM_WEAPONS 5
#define NUM_ARMORS 6
#define NUM_RINGS 7

typedef struct {
    char name[20];
    int cost;
    int damage;
    int armor;
} Item;

static Item weapons[NUM_WEAPONS] = {
    {"Dagger", 8, 4, 0},
    {"Shortsword", 10, 5, 0},
    {"Warhammer", 25, 6, 0},
    {"Longsword", 40, 7, 0},
    {"Greataxe", 74, 8, 0}
};

static Item armors[NUM_ARMORS] = {
    {"None", 0, 0, 0}, // No armor
    {"Leather", 13, 0, 1},
    {"Chainmail", 31, 0, 2},
    {"Splintmail", 53, 0, 3},
    {"Bandedmail", 75, 0, 4},
    {"Platemail", 102, 0, 5}
};

static Item rings[NUM_RINGS] = {
    {"None", 0, 0, 0}, // No ring
    {"Damage +1", 25, 1, 0},
    {"Damage +2", 50, 2, 0},
    {"Damage +3", 100, 3, 0},
    {"Defense +1", 20, 0, 1},
    {"Defense +2", 40, 0, 2},
    {"Defense +3", 80, 0, 3}
};

// Boss and player stats
static int boss_hit_points = 0;
static int boss_damage = 0;
static int boss_armor = 0;

static int player_hit_points = 100;

// Parse boss stats from input file
static void parse_boss_stats(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char line[50];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "Hit Points: %d", &boss_hit_points) == 1) continue;
        if (sscanf(line, "Damage: %d", &boss_damage) == 1) continue;
        if (sscanf(line, "Armor: %d", &boss_armor) == 1) continue;
    }

    fclose(file);
}

// Simulate fight and determine if player wins
static bool does_player_win(int player_damage, int player_armor) {
    int player_hp = player_hit_points;
    int boss_hp = boss_hit_points;

    while (true) {
        // Player's attack
        int damage_to_boss = (player_damage > boss_armor) ? player_damage - boss_armor : 1;
        boss_hp -= damage_to_boss;
        if (boss_hp <= 0) return true;

        // Boss's attack
        int damage_to_player = (boss_damage > player_armor) ? boss_damage - player_armor : 1;
        player_hp -= damage_to_player;
        if (player_hp <= 0) return false;
    }
}

void run_day21() {
    int min_gold_spent = 9999;  // Part 1
    int max_gold_spent = 0;     // Part 2

    clock_t start_time = clock();

    // Parse boss stats from file
    parse_boss_stats("data/21.txt");

    // Iterate through all combinations of equipment
    for (int w = 0; w < NUM_WEAPONS; w++) {
        for (int a = 0; a < NUM_ARMORS; a++) {
            for (int r1 = 0; r1 < NUM_RINGS; r1++) {
                for (int r2 = 0; r2 < NUM_RINGS; r2++) {
                    if (r1 == r2 && r1 != 0) continue; // Skip duplicate rings

                    int total_cost = weapons[w].cost + armors[a].cost + rings[r1].cost + rings[r2].cost;
                    int total_damage = weapons[w].damage + rings[r1].damage + rings[r2].damage;
                    int total_armor = armors[a].armor + rings[r1].armor + rings[r2].armor;

                    if (does_player_win(total_damage, total_armor)) {
                        if (total_cost < min_gold_spent) min_gold_spent = total_cost; // Part 1
                    } else {
                        if (total_cost > max_gold_spent) max_gold_spent = total_cost; // Part 2
                    }
                }
            }
        }
    }

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Part 1 - Least amount of gold you can spend and still win is: %d\n", min_gold_spent);
    printf("Part 2 - Most amount of gold you can spend and still lose is: %d\n", max_gold_spent);
    printf("Execution time: %.2f seconds\n", execution_time);
}

