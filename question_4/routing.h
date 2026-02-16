#ifndef ROUTING_H
#define ROUTING_H

#include <limits.h>
#include <stdbool.h>

#define MAX_NODES 7
#define INF INT_MAX

// Function prototypes
void initGraph(int graph[MAX_NODES][MAX_NODES]);
int getNodeIndex(const char* name);
const char* getNodeName(int index);
void dijkstra(int graph[MAX_NODES][MAX_NODES], int src, int dest);

#endif
