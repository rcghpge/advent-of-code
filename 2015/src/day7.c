// Day 7 - Some Assembly Required
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "day7.h"
#include "main.h"

#define MAX_MAP_SIZE 1024

typedef struct {
    char data[3];
} identifier;

typedef enum {
    OP_NOT,
    OP_ASSIGN,
    OP_AND,
    OP_OR,
    OP_LSHIFT,
    OP_RSHIFT
} opcode;

typedef union {
    identifier reg;
    unsigned short imm;
} operand;

typedef struct {
    opcode op;
    operand op1;
    operand op2;
    int is_imm1;
    int is_imm2;
} instruction;

typedef struct {
    identifier key;
    instruction value;
} map_entry;

typedef struct {
    map_entry entries[MAX_MAP_SIZE];
    size_t size;
} simple_map;

void simple_map_create(simple_map *map) {
    map->size = 0;
}

void simple_map_put(simple_map *map, identifier key, instruction value) {
    for (size_t i = 0; i < map->size; i++) {
        if (strcmp(map->entries[i].key.data, key.data) == 0) {
            map->entries[i].value = value;
            return;
        }
    }
    if (map->size < MAX_MAP_SIZE) {
        map->entries[map->size].key = key;
        map->entries[map->size].value = value;
        map->size++;
    } else {
        fprintf(stderr, "Error: Map capacity exceeded\n");
        exit(EXIT_FAILURE);
    }
}

int simple_map_get(simple_map *map, identifier key, instruction *value) {
    for (size_t i = 0; i < map->size; i++) {
        if (strcmp(map->entries[i].key.data, key.data) == 0) {
            *value = map->entries[i].value;
            return 1;
        }
    }
    return 0;
}

unsigned short evaluate(simple_map *map, identifier key) {
    instruction instr;
    if (!simple_map_get(map, key, &instr)) {
        fprintf(stderr, "Error: Key '%s' not found during evaluation.\n", key.data);
        exit(EXIT_FAILURE);
    }

    unsigned short value = 0;
    switch (instr.op) {
        case OP_NOT:
            value = ~evaluate(map, instr.op1.reg);
            break;
        case OP_ASSIGN:
            value = instr.is_imm1 ? instr.op1.imm : evaluate(map, instr.op1.reg);
            break;
        case OP_AND:
            value = (instr.is_imm1 ? instr.op1.imm : evaluate(map, instr.op1.reg)) &
                    (instr.is_imm2 ? instr.op2.imm : evaluate(map, instr.op2.reg));
            break;
        case OP_OR:
            value = (instr.is_imm1 ? instr.op1.imm : evaluate(map, instr.op1.reg)) |
                    (instr.is_imm2 ? instr.op2.imm : evaluate(map, instr.op2.reg));
            break;
        case OP_LSHIFT:
            value = evaluate(map, instr.op1.reg) << instr.op2.imm;
            break;
        case OP_RSHIFT:
            value = evaluate(map, instr.op1.reg) >> instr.op2.imm;
            break;
    }

    simple_map_put(map, key, (instruction){.op = OP_ASSIGN, .op1.imm = value, .is_imm1 = 1});
    return value;
}

void parse_instruction(char *line, identifier *key, instruction *instr) {
    char op1[10], op2[10], dest[10];
    if (sscanf(line, "NOT %s -> %s", op1, dest) == 2) {
        instr->op = OP_NOT;
        strcpy(instr->op1.reg.data, op1);
        instr->is_imm1 = 0;
    } else if (sscanf(line, "%s AND %s -> %s", op1, op2, dest) == 3) {
        instr->op = OP_AND;
        instr->is_imm1 = isdigit(op1[0]);
        if (instr->is_imm1) instr->op1.imm = atoi(op1);
        else strcpy(instr->op1.reg.data, op1);
        instr->is_imm2 = 0;
        strcpy(instr->op2.reg.data, op2);
    } else if (sscanf(line, "%s OR %s -> %s", op1, op2, dest) == 3) {
        instr->op = OP_OR;
        instr->is_imm1 = isdigit(op1[0]);
        if (instr->is_imm1) instr->op1.imm = atoi(op1);
        else strcpy(instr->op1.reg.data, op1);
        instr->is_imm2 = 0;
        strcpy(instr->op2.reg.data, op2);
    } else if (sscanf(line, "%s LSHIFT %s -> %s", op1, op2, dest) == 3) {
        instr->op = OP_LSHIFT;
        strcpy(instr->op1.reg.data, op1);
        instr->op2.imm = atoi(op2);
        instr->is_imm1 = 0;
        instr->is_imm2 = 1;
    } else if (sscanf(line, "%s RSHIFT %s -> %s", op1, op2, dest) == 3) {
        instr->op = OP_RSHIFT;
        strcpy(instr->op1.reg.data, op1);
        instr->op2.imm = atoi(op2);
        instr->is_imm1 = 0;
        instr->is_imm2 = 1;
    } else if (sscanf(line, "%s -> %s", op1, dest) == 2) {
        instr->op = OP_ASSIGN;
        instr->is_imm1 = isdigit(op1[0]);
        if (instr->is_imm1) instr->op1.imm = atoi(op1);
        else strcpy(instr->op1.reg.data, op1);
    }
    strcpy(key->data, dest);
}

void run_day7() {
    simple_map map, backup_map;
    simple_map_create(&map);
    simple_map_create(&backup_map);

    FILE *file = fopen("data/7.txt", "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    clock_t start_time = clock();

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        identifier key;
        instruction instr;
        parse_instruction(line, &key, &instr);
        simple_map_put(&map, key, instr);
        simple_map_put(&backup_map, key, instr); // Save original map
    }
    fclose(file);

    unsigned short part1 = evaluate(&map, (identifier){{"a"}});

    // Reset map for Part 2
    map = backup_map;
    simple_map_put(&map, (identifier){{"b"}}, (instruction){.op = OP_ASSIGN, .op1.imm = part1, .is_imm1 = 1});
    unsigned short part2 = evaluate(&map, (identifier){{"a"}});

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Part 1 - Wire a signal: %u\n", part1);
    printf("Part 2 - Wire a new signal: %u\n", part2);
    printf("Execution time: %.6f seconds\n", execution_time);
}

