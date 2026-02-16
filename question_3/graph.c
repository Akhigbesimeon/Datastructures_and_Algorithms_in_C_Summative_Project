#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

// Graph initialization 
InteractionGraph* create_graph(int initial_capacity) {
    InteractionGraph *g = (InteractionGraph*)malloc(sizeof(InteractionGraph));
    g->num_users = 0;
    g->capacity = initial_capacity;
    
    g->user_ids = (char**)malloc(g->capacity * sizeof(char*));
    g->matrix = (int**)malloc(g->capacity * sizeof(int*));
    
    for (int i = 0; i < g->capacity; i++) {
        g->matrix[i] = (int*)calloc(g->capacity, sizeof(int));
    }
    return g;
}

// Function to find user index
int get_user_index(InteractionGraph *g, const char *user_id) {
    for (int i = 0; i < g->num_users; i++) {
        if (strcmp(g->user_ids[i], user_id) == 0) {
            return i;
        }
    }
    return -1; 
}

// Function to resize matrix dynamically 
static void resize_graph(InteractionGraph *g) {
    int new_cap = g->capacity * 2;
    g->user_ids = (char**)realloc(g->user_ids, new_cap * sizeof(char*));
    g->matrix = (int**)realloc(g->matrix, new_cap * sizeof(int*));
    
    for (int i = 0; i < g->num_users; i++) {
        g->matrix[i] = (int*)realloc(g->matrix[i], new_cap * sizeof(int));
        for (int j = g->capacity; j < new_cap; j++) {
            g->matrix[i][j] = 0;
        }
    }
    for (int i = g->num_users; i < new_cap; i++) {
        g->matrix[i] = (int*)calloc(new_cap, sizeof(int));
    }
    g->capacity = new_cap;
}

// Dynamic user updates
void add_user(InteractionGraph *g, const char *user_id) {
    if (get_user_index(g, user_id) != -1) return;
    
    if (g->num_users >= g->capacity) {
        resize_graph(g);
    }
    
    g->user_ids[g->num_users] = strdup(user_id);
    g->num_users++;
}

void remove_user(InteractionGraph *g, const char *user_id) {
    int idx = get_user_index(g, user_id);
    if (idx == -1) {
        printf("Error: Cannot remove. User '%s' not found.\n", user_id);
        return;
    }
    
    free(g->user_ids[idx]);
    
    for (int i = idx; i < g->num_users - 1; i++) {
        g->user_ids[i] = g->user_ids[i + 1];
    }
    
    int *row_to_delete = g->matrix[idx];
    for (int i = idx; i < g->num_users - 1; i++) {
        g->matrix[i] = g->matrix[i + 1];
    }
    g->matrix[g->num_users - 1] = row_to_delete; 
    
    for (int i = 0; i < g->num_users; i++) {
        for (int j = idx; j < g->num_users - 1; j++) {
            g->matrix[i][j] = g->matrix[i][j + 1];
        }
        g->matrix[i][g->num_users - 1] = 0; 
    }
    
    g->num_users--;
    printf("\n[System] User '%s' successfully removed.\n", user_id);
}

// Dynamic interaction updates
void add_interaction(InteractionGraph *g, const char *from_id, const char *to_id) {
    int u = get_user_index(g, from_id);
    int v = get_user_index(g, to_id);
    
    if (u != -1 && v != -1) {
        g->matrix[u][v] = 1; 
    } else {
        printf("Error: Cannot add interaction. One or both users not found (%s -> %s).\n", from_id, to_id);
    }
}

void remove_interaction(InteractionGraph *g, const char *from_id, const char *to_id) {
    int u = get_user_index(g, from_id);
    int v = get_user_index(g, to_id);
    
    if (u != -1 && v != -1) {
        g->matrix[u][v] = 0;
    }
}

// Query function
void query_user(InteractionGraph *g, const char *user_id) {
    int idx = get_user_index(g, user_id);
    if (idx == -1) {
        printf("\nQuery Error: User '%s' is unknown or invalid.\n", user_id);
        return;
    }
    
    printf("\n-- Query Results for %s --\n", user_id);
    
    printf("Interacts with (Outgoing): ");
    int out_count = 0;
    for (int i = 0; i < g->num_users; i++) {
        if (g->matrix[idx][i] == 1) {
            printf("%s ", g->user_ids[i]);
            out_count++;
        }
    }
    if (out_count == 0) printf("None");
    printf("\n");
    
    printf("Interacted by (Incoming): ");
    int in_count = 0;
    for (int i = 0; i < g->num_users; i++) {
        if (g->matrix[i][idx] == 1) {
            printf("%s ", g->user_ids[i]);
            in_count++;
        }
    }
    if (in_count == 0) printf("None");
    printf("\n----------------------------\n");
}

// Display adjacency matrix
void display_matrix(InteractionGraph *g) {
    printf("\n-- Adjacency Matrix --\n");
    printf("%-6s", "");
    for (int i = 0; i < g->num_users; i++) {
        printf("%-6s", g->user_ids[i]);
    }
    printf("\n");
    
    for (int i = 0; i < g->num_users; i++) {
        printf("%-6s", g->user_ids[i]);
        for (int j = 0; j < g->num_users; j++) {
            printf("%-6d", g->matrix[i][j]);
        }
        printf("\n");
    }
    printf("------------------------\n");
}

// Cleanup
void free_graph(InteractionGraph *g) {
    for (int i = 0; i < g->num_users; i++) {
        free(g->user_ids[i]);
    }
    free(g->user_ids);
    for (int i = 0; i < g->capacity; i++) {
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g);
}
