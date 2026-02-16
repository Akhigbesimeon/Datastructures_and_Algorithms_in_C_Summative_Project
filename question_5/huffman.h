#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_TREE_HT 256

// Data structures
typedef struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
} MinHeapNode;

// Min Heap data structure 
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode **array;
} MinHeap;

// Function Prototypes
void compressLogFile(const char *inputFilename, const char *outputFilename);
void decompressLogFile(const char *inputFilename, const char *outputFilename);

// Utility Functions
long getFileSize(const char *filename);
int compareFiles(const char *file1, const char *file2);

#endif
