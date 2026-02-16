#include <stdio.h>
#include <string.h>
#include "routing.h"

// Main function
int main() {
    int graph[MAX_NODES][MAX_NODES];
    initGraph(graph);

    char startServer[50];
    const char* destServer = "S6"; 

    printf("Enter starting server (e.g., S1, S2, Switch X): ");
    
    // Read input, using fgets to handle potential spaces in strings
    if (fgets(startServer, sizeof(startServer), stdin) != NULL) {
        startServer[strcspn(startServer, "\n")] = 0; 
    }

    int srcIndex = getNodeIndex(startServer);
    int destIndex = getNodeIndex(destServer);

    // Handles invalid or unknown server names
    if (srcIndex == -1) {
        printf("Error: Invalid or unknown server name '%s'.\n", startServer);
        return 1;
    }

    printf("\nComputing minimum-latency path from %s to %s...\n", startServer, destServer);
    dijkstra(graph, srcIndex, destIndex);

    return 0;
}
