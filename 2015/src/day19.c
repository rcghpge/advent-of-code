// Day 19 - Medicine for Rudolph
#include "day19.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REPLACEMENTS 50
#define MAX_MOLECULE_LENGTH 1000

typedef struct {
    char source[10];
    char target[10];
} Replacement;

typedef struct Node {
    char molecule[MAX_MOLECULE_LENGTH];
    int steps;
    struct Node *next;
} Node;

void insert(Node **head, const char *molecule, int steps) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->molecule, molecule);
    new_node->steps = steps;
    new_node->next = *head;
    *head = new_node;
}

int contains(Node *head, const char *molecule) {
    Node *current = head;
    while (current != NULL) {
        if (strcmp(current->molecule, molecule) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void free_list(Node *head) {
    Node *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void generate_molecules(Replacement *replacements, int rep_count, const char *molecule, Node **molecules) {
    int mol_len = strlen(molecule);

    for (int i = 0; i < mol_len; i++) {
        for (int j = 0; j < rep_count; j++) {
            int src_len = strlen(replacements[j].source);
            if (strncmp(&molecule[i], replacements[j].source, src_len) == 0) {
                char new_molecule[MAX_MOLECULE_LENGTH];
                strncpy(new_molecule, molecule, i);
                new_molecule[i] = '\0';
                strcat(new_molecule, replacements[j].target);
                strcat(new_molecule, &molecule[i + src_len]);
                if (!contains(*molecules, new_molecule)) {
                    insert(molecules, new_molecule, 0);
                }
            }
        }
    }
}

int bfs(Replacement *replacements, int rep_count, const char *target) {
    Node *queue = NULL;
    insert(&queue, target, 0);

    while (queue != NULL) {
        Node *current = queue;
        queue = queue->next;

        if (strcmp(current->molecule, "e") == 0) {
            int steps = current->steps;
            free_list(queue);
            free(current);
            return steps;
        }

        int mol_len = strlen(current->molecule);
        for (int i = 0; i < mol_len; i++) {
            for (int j = 0; j < rep_count; j++) {
                int tgt_len = strlen(replacements[j].target);
                if (strncmp(&current->molecule[i], replacements[j].target, tgt_len) == 0) {
                    char new_molecule[MAX_MOLECULE_LENGTH];
                    strncpy(new_molecule, current->molecule, i);
                    new_molecule[i] = '\0';
                    strcat(new_molecule, replacements[j].source);
                    strcat(new_molecule, &current->molecule[i + tgt_len]);
                    if (!contains(queue, new_molecule)) {
                        insert(&queue, new_molecule, current->steps + 1);
                    }
                }
            }
        }

        free(current);
    }

    return -1; // No solution found
}

void run_day19(void) {
    FILE *file = fopen("data/19.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    Replacement replacements[MAX_REPLACEMENTS];
    int rep_count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file)) {
        if (strchr(line, '=') == NULL) {
            break;
        }
        sscanf(line, "%s => %s", replacements[rep_count].source, replacements[rep_count].target);
        rep_count++;
    }

    char molecule[MAX_MOLECULE_LENGTH];
    fgets(molecule, sizeof(molecule), file);
    molecule[strcspn(molecule, "\n")] = 0; // Remove newline character

    fclose(file);

    clock_t start_time = clock();

    // Part 1
    Node *molecules = NULL;
    generate_molecules(replacements, rep_count, molecule, &molecules);
    int distinct_molecules_count = 0;
    Node *current = molecules;
    while (current != NULL) {
        distinct_molecules_count++;
        current = current->next;
    }

    free_list(molecules);

    // Part 2
    int steps = bfs(replacements, rep_count, molecule);

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Part 1 - Distinct molecules: %d\n", distinct_molecules_count);
    printf("Part 2 - Fewest number of steps: %d\n", steps);
    printf("Execution time: %.2f seconds\n", execution_time);
}

