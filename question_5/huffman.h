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
bool compress_file(const char *input_file, const char *output_file);

bool decompress_file(const char *input_file, const char *output_file);

// Utility Functions
long get_file_size(const char *filename);

bool verify_integrity(const char *file1, const char *file2);

#endif 
