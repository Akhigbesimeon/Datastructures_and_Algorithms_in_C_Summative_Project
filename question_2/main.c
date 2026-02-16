#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h" 

// Main Function
int main() {
    FILE* file = fopen(APPROVED_FILE, "r");
    if (!file) {
        printf("Error: Could not open %s. Please create it with valid commands.\n", APPROVED_FILE);
        return EXIT_FAILURE;
    }

    char cmd_array[MAX_CMDS][MAX_CMD_LEN];
    int count = 0;
    char buffer[MAX_CMD_LEN];

    // Load commands
    while (fgets(buffer, sizeof(buffer), file) && count < MAX_CMDS) {
        strip_newline(buffer);
        if (strlen(buffer) > 0) {
            strcpy(cmd_array[count], buffer);
            count++;
        }
    }
    fclose(file);

    if (count == 0) {
        printf("Error: No commands loaded. Terminating.\n");
        return EXIT_FAILURE;
    }

    // Sort array to ensure balanced BST creation
    qsort(cmd_array, count, sizeof(cmd_array[0]), compare_strings);
    
    // Build tree
    Node* root = build_balanced_bst(cmd_array, 0, count - 1);
    printf("-- Control Terminal Initialized --\n");
    printf("Loaded %d approved commands securely.\n\n", count);

    char input[MAX_CMD_LEN];
    
    // Command verification loop
    while (true) {
        printf("Operator Input > ");
        if (!fgets(input, sizeof(input), stdin)) break;
        strip_newline(input);

        if (strcmp(input, "EXIT") == 0) {
            printf("Shutting down terminal...\n");
            break;
        }

        if (strlen(input) == 0) continue;

        // Check for exact match
        if (search_exact(root, input)) {
            printf(">> [OK] Executing command: %s\n", input);
            continue;
        }

        // Exact match failed. Find the closest match.
        int min_dist = 999;
        char best_match[MAX_CMD_LEN] = "";
        find_closest(root, input, best_match, &min_dist);

        // Evaluate threshold
        if (min_dist <= TYPO_THRESHOLD) {
            printf(">> [WARNING] Command '%s' not recognized.\n", input);
            printf(">> Did you mean: '%s'?\n", best_match);
        } else {
            log_unrecognized(input);
        }
    }

    // Deallocate memory before exiting
    free_tree(root);
    return EXIT_SUCCESS;
}
