#include <stdio.h>
#include <string.h>
#include "routing.h"

// Node names mapping 
const char* node_names[MAX_NODES] = {"S1", "S2", "S3", "S4", "S5", "S6", "Switch X"};

// Function to initialize the adjacency matrix
void initGraph(int graph[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph[i][j] = 0;
        }
    }

    // Populate bidirectional edges
    graph[0][1] = 8;  graph[1][0] = 8;  
    graph[0][3] = 20; graph[3][0] = 20;  
    graph[1][2] = 7;  graph[2][1] = 7;   
    graph[2][5] = 12; graph[5][2] = 12;  
    graph[3][4] = 4;  graph[4][3] = 4;   
    graph[4][5] = 6;  graph[5][4] = 6;   
    graph[1][6] = 3;  graph[6][1] = 3;   
    graph[6][4] = 5;  graph[4][6] = 5; 
}

// Function to convert string names to matrix indices
int getNodeIndex(const char* name) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (strcmp(node_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

const char* getNodeName(int index) {
    if (index >= 0 && index < MAX_NODES) {
        return node_names[index];
    }
    return "Unknown";
}

// Function to find the vertex with minimum distance value
int minDistance(int dist[], bool sptSet[]) {
    int min = INF, min_index = -1;
    for (int v = 0; v < MAX_NODES; v++) {
        if (sptSet[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Function to print constructed path
void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%s", getNodeName(j));
        return;
    }
    printPath(parent, parent[j]);
    printf(" -> %s", getNodeName(j));
}

// Core Dijkstra implementation
void dijkstra(int graph[MAX_NODES][MAX_NODES], int src, int dest) {
    int dist[MAX_NODES];     
    bool sptSet[MAX_NODES];  
    int parent[MAX_NODES];   

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < MAX_NODES; i++) {
        parent[0] = -1;
        dist[i] = INF;
        sptSet[i] = false;
    }

    dist[src] = 0;
    parent[src] = -1;

    // Find shortest path for all vertices
    for (int count = 0; count < MAX_NODES - 1; count++) {
        int u = minDistance(dist, sptSet);
        if (u == -1) break; 
        
        sptSet[u] = true; 

        // Update dist value of the adjacent vertices
        for (int v = 0; v < MAX_NODES; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Output
    if (dist[dest] == INF) {
        printf("No path exists between %s and %s.\n", getNodeName(src), getNodeName(dest));
    } else {
        printf("Optimal routing path: ");
        printPath(parent, dest);
        printf("\nTotal latency: %d\n", dist[dest]);
    }
}
