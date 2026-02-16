#include "huffman.h"
#include <string.h>

// Helper Functions
static MinHeapNode* new_node(char data, unsigned freq) {
    MinHeapNode* temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// Function implementations
bool compress_file(const char *input_file, const char *output_file) {
    return true; 
}

bool decompress_file(const char *input_file, const char *output_file) {
    return true;
}

long get_file_size(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

bool verify_integrity(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");
    
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return false;
    }
    
    bool match = true;
    int ch1, ch2;
    do {
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
        if (ch1 != ch2) {
            match = false;
            break;
        }
    } while (ch1 != EOF && ch2 != EOF);
    
    fclose(f1);
    fclose(f2);
    return match;
}
