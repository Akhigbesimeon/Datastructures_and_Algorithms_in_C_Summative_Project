#ifndef GRAPH_H
#define GRAPH_H

// Data Structure
typedef struct {
    char **user_ids;       
    int **matrix;          
    int num_users;         
    int capacity; 
} InteractionGraph;

// Graph initialization and memory management
InteractionGraph* create_graph(int initial_capacity);
void free_graph(InteractionGraph *g);

// Dynamic user updates 
void add_user(InteractionGraph *g, const char *user_id);
void remove_user(InteractionGraph *g, const char *user_id);

// Dynamic interaction updates
void add_interaction(InteractionGraph *g, const char *from_id, const char *to_id);
void remove_interaction(InteractionGraph *g, const char *from_id, const char *to_id);

// Query and display
void query_user(InteractionGraph *g, const char *user_id);
void display_matrix(InteractionGraph *g);

// Helper function
int get_user_index(InteractionGraph *g, const char *user_id);

#endif
