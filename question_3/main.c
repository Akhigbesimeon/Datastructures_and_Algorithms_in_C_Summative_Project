#include <stdio.h>
#include "graph.h"

// Main function
int main() {
    InteractionGraph *graph = create_graph(4);

    const char* users[] = {"U101", "U102", "U103", "U104", "U105", "U106", "U107", "U108"};
    for (int i = 0; i < 8; i++) {
        add_user(graph, users[i]);
    }

    add_interaction(graph, "U101", "U102");
    add_interaction(graph, "U101", "U103");
    add_interaction(graph, "U102", "U104");
    add_interaction(graph, "U103", "U105");
    add_interaction(graph, "U104", "U105");
    add_interaction(graph, "U104", "U106");
    add_interaction(graph, "U105", "U107");
    add_interaction(graph, "U106", "U108");

    // Function to generate and display the adjacency matrix
    display_matrix(graph);

    // Function to test query functionality 
    query_user(graph, "U104"); 
    query_user(graph, "U999"); 

    // Function to test dynamic updates 
    remove_user(graph, "U105"); 
    add_user(graph, "U109");   
    add_interaction(graph, "U101", "U109");

    // Display matrix again to verify dynamic updates
    display_matrix(graph);

    // Clean up memory
    free_graph(graph);

    return 0;
}
