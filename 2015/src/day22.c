// Day 22 - Wizard Simulator 20XX
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "day22.h"

typedef struct {
    int hp;
    int mana;
    int armor;
} Player;

typedef struct {
    int hp;
    int damage;
} Boss;

typedef struct {
    int shield_timer;
    int poison_timer;
    int recharge_timer;
} Effects;

typedef struct {
    int player_hp;
    int player_mana;
    int boss_hp;
    int shield_timer;
    int poison_timer;
    int recharge_timer;
    int mana_spent;
    int hard_mode;
} GameState;

#define MEMO_SIZE 1000000
static GameState memo[MEMO_SIZE];

unsigned long hash_state(const GameState *state) {
    return (state->player_hp * 31 + state->player_mana * 37 +
            state->boss_hp * 41 + state->shield_timer * 43 +
            state->poison_timer * 47 + state->recharge_timer * 53 +
            state->mana_spent * 59 + state->hard_mode) % MEMO_SIZE;
}

int is_memoized(const GameState *state) {
    unsigned long hash = hash_state(state);
    GameState *entry = &memo[hash];
    if (entry->player_hp == state->player_hp &&
        entry->player_mana == state->player_mana &&
        entry->boss_hp == state->boss_hp &&
        entry->shield_timer == state->shield_timer &&
        entry->poison_timer == state->poison_timer &&
        entry->recharge_timer == state->recharge_timer &&
        entry->mana_spent == state->mana_spent &&
        entry->hard_mode == state->hard_mode) {
        return 1;
    }
    *entry = *state;
    return 0;
}

void apply_effects(Player *player, Boss *boss, Effects *effects) {
    if (effects->shield_timer > 0) {
        player->armor = 7;
        effects->shield_timer--;
    } else {
        player->armor = 0;
    }

    if (effects->poison_timer > 0) {
        boss->hp -= 3;
        effects->poison_timer--;
    }

    if (effects->recharge_timer > 0) {
        player->mana += 101;
        effects->recharge_timer--;
    }
}

int player_turn(Player player, Boss boss, Effects effects, int mana_spent, int hard_mode);

int boss_turn(Player player, Boss boss, Effects effects, int mana_spent, int hard_mode) {
    apply_effects(&player, &boss, &effects);

    if (boss.hp <= 0) return mana_spent;

    int damage = boss.damage - player.armor;
    if (damage < 1) damage = 1;
    player.hp -= damage;

    if (player.hp <= 0) return INT_MAX;

    return player_turn(player, boss, effects, mana_spent, hard_mode);
}

int player_turn(Player player, Boss boss, Effects effects, int mana_spent, int hard_mode) {
    if (hard_mode) {
        player.hp--;
        if (player.hp <= 0) return INT_MAX;
    }

    apply_effects(&player, &boss, &effects);

    if (boss.hp <= 0) return mana_spent;

    GameState state = {player.hp, player.mana, boss.hp,
                       effects.shield_timer, effects.poison_timer, effects.recharge_timer,
                       mana_spent, hard_mode};

    if (is_memoized(&state)) return INT_MAX;

    int min_mana_spent = INT_MAX;

    if (player.mana >= 53) {
        Player new_player = player;
        Boss new_boss = boss;
        Effects new_effects = effects;
        new_player.mana -= 53;
        new_boss.hp -= 4;
        int result = boss_turn(new_player, new_boss, new_effects, mana_spent + 53, hard_mode);
        if (result < min_mana_spent) min_mana_spent = result;
    }

    if (player.mana >= 73) {
        Player new_player = player;
        Boss new_boss = boss;
        Effects new_effects = effects;
        new_player.mana -= 73;
        new_player.hp += 2;
        new_boss.hp -= 2;
        int result = boss_turn(new_player, new_boss, new_effects, mana_spent + 73, hard_mode);
        if (result < min_mana_spent) min_mana_spent = result;
    }

    if (player.mana >= 113 && effects.shield_timer == 0) {
        Player new_player = player;
        Boss new_boss = boss;
        Effects new_effects = effects;
        new_player.mana -= 113;
        new_effects.shield_timer = 6;
        int result = boss_turn(new_player, new_boss, new_effects, mana_spent + 113, hard_mode);
        if (result < min_mana_spent) min_mana_spent = result;
    }

    if (player.mana >= 173 && effects.poison_timer == 0) {
        Player new_player = player;
        Boss new_boss = boss;
        Effects new_effects = effects;
        new_player.mana -= 173;
        new_effects.poison_timer = 6;
        int result = boss_turn(new_player, new_boss, new_effects, mana_spent + 173, hard_mode);
        if (result < min_mana_spent) min_mana_spent = result;
    }

    if (player.mana >= 229 && effects.recharge_timer == 0) {
        Player new_player = player;
        Boss new_boss = boss;
        Effects new_effects = effects;
        new_player.mana -= 229;
        new_effects.recharge_timer = 5;
        int result = boss_turn(new_player, new_boss, new_effects, mana_spent + 229, hard_mode);
        if (result < min_mana_spent) min_mana_spent = result;
    }

    return min_mana_spent;
}

void run_day22() {
    FILE *file = fopen("data/22.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        return;
    }

    Boss boss;
    fscanf(file, "Hit Points: %d\nDamage: %d", &boss.hp, &boss.damage);
    fclose(file);

    Player player = {50, 500, 0};
    Effects effects = {0, 0, 0};

    clock_t start = clock();
    int part1 = player_turn(player, boss, effects, 0, 0);
    int part2 = player_turn(player, boss, effects, 0, 1);
    clock_t end = clock();

    printf("Part 1 - Least amount of mana: %d\n", part1);
    printf("Part 2 - Least amount of mana 2: %d\n", part2);
    printf("Execution time: %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

