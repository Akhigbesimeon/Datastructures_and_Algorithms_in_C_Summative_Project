#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

// Constants
#define MAX_CMDS 40
#define MAX_CMD_LEN 50
#define TYPO_THRESHOLD 2
#define APPROVED_FILE "approved_commands.txt"
#define LOG_FILE "unrecognized_commands.log"

// Data Structures
typedef struct Node {
    char command[MAX_CMD_LEN];
    struct Node* left;
    struct Node* right;
} Node;

// Utility Functions
void strip_newline(char* str);
int compare_strings(const void* a, const void* b);
void log_unrecognized(const char* cmd);

// BST Operations 
Node* create_node(const char* cmd);
Node* build_balanced_bst(char arr[][MAX_CMD_LEN], int start, int end);
bool search_exact(Node* root, const char* target);
void find_closest(Node* root, const char* target, char* best_match, int* min_dist);
void free_tree(Node* root);

#endif // AUTH_H
