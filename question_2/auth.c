#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"

// Helper Functions
static int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

// Function to calculate Levenshtein Distance
static int levenshtein(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int matrix[len1 + 1][len2 + 1];

    for (int i = 0; i <= len1; i++) matrix[i][0] = i;
    for (int j = 0; j <= len2; j++) matrix[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            matrix[i][j] = min3(
                matrix[i - 1][j] + 1,       
                matrix[i][j - 1] + 1,    
                matrix[i - 1][j - 1] + cost 
            );
        }
    }
    return matrix[len1][len2];
}

// Callback function used by qsort
int compare_strings(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

// Safely removes the trailing newline character '\n' from a string if it exist
void strip_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Function to append unrecognized commands to a secure log file for auditing
void log_unrecognized(const char* cmd) {
    FILE* logFile = fopen(LOG_FILE, "a");
    if (logFile) {
        fprintf(logFile, "REJECTED: %s\n", cmd);
        fclose(logFile);
        printf(">> [ERROR] Command unrecognized. Execution rejected and logged.\n");
    } else {
        printf(">> [ERROR] Could not open log file.\n");
    }
}
 
// Function to allocate memory for a new BST node
Node* create_node(const char* cmd) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->command, cmd, MAX_CMD_LEN);
    newNode->command[MAX_CMD_LEN - 1] = '\0';
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to build a balanced BST from a pre-sorted array of commands
Node* build_balanced_bst(char arr[][MAX_CMD_LEN], int start, int end) {
    if (start > end) return NULL;
    
    int mid = (start + end) / 2;
    Node* root = create_node(arr[mid]);
    
    root->left = build_balanced_bst(arr, start, mid - 1);
    root->right = build_balanced_bst(arr, mid + 1, end);
    
    return root;
}

// Function to find an exact match in the BST by traversing
bool search_exact(Node* root, const char* target) {
    if (root == NULL) return false;
    
    int cmp = strcmp(target, root->command);
    if (cmp == 0) return true;
    if (cmp < 0) return search_exact(root->left, target);
    return search_exact(root->right, target);
}

// Performs a full tree traversal to calculate the edit distance
void find_closest(Node* root, const char* target, char* best_match, int* min_dist) {
    if (root == NULL) return;

    int dist = levenshtein(root->command, target);
    if (dist < *min_dist) {
        *min_dist = dist;
        strcpy(best_match, root->command);
    }

    find_closest(root->left, target, best_match, min_dist);
    find_closest(root->right, target, best_match, min_dist);
}

// Function to free all allocated memory
void free_tree(Node* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
