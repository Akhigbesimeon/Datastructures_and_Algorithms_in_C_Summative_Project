#include "huffman.h"
#include <string.h>
#include <stdint.h>

// Data structures
typedef struct Node {
    unsigned char data;
    unsigned int freq;
    struct Node *left, *right;
} Node;

// Structure to hold binary codes
typedef struct {
    unsigned int code;
    int length;
} HuffCode;

// Function to get file size in bytes
long getFileSize(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return size;
}

// Function to compare two files for data integrity verification
int compareFiles(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");
    if (!f1 || !f2) {
        if(f1) fclose(f1);
        if(f2) fclose(f2);
        return 0; 
    }

    int ch1, ch2;
    do {
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
        if (ch1 != ch2) {
            fclose(f1); fclose(f2);
            return 0; 
        }
    } while (ch1 != EOF && ch2 != EOF);

    fclose(f1); fclose(f2);
    return 1; 
}

// Function to create a new tree node
static Node* createNode(unsigned char data, unsigned int freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// Function to build the Huffman Tree from frequency array
static Node* buildHuffmanTree(unsigned int freq[256]) {
    Node* nodes[512];
    int nodeCount = 0;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[nodeCount++] = createNode((unsigned char)i, freq[i]);
        }
    }

    if (nodeCount == 0) return NULL;

    int activeNodes = nodeCount;
    while (activeNodes > 1) {
        int min1 = -1, min2 = -1;
        
        // Find two minimums
        for (int i = 0; i < nodeCount; i++) {
            if (nodes[i] != NULL) {
                if (min1 == -1 || nodes[i]->freq < nodes[min1]->freq) {
                    min2 = min1;
                    min1 = i;
                } else if (min2 == -1 || nodes[i]->freq < nodes[min2]->freq) {
                    min2 = i;
                }
            }
        }

        Node* parent = createNode(0, nodes[min1]->freq + nodes[min2]->freq);
        parent->left = nodes[min1];
        parent->right = nodes[min2];

        nodes[min1] = parent; 
        nodes[min2] = NULL;  
        activeNodes--;
    }

    // Return the root of the tree
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i] != NULL) return nodes[i];
    }
    return NULL;
}

// Function to recursively generate bit codes from the tree
static void generateCodes(Node* root, HuffCode codes[256], unsigned int currentCode, int length) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        codes[root->data].code = currentCode;
        codes[root->data].length = length;
        return;
    }

    generateCodes(root->left, codes, currentCode << 1, length + 1);
    generateCodes(root->right, codes, (currentCode << 1) | 1, length + 1);
}

// Free tree memory
static void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Implementations
void compressLogFile(const char *inputFilename, const char *outputFilename) {
    FILE *in = fopen(inputFilename, "rb");
    if (!in) { perror("Error opening input file"); return; }

    unsigned int freq[256] = {0};
    int ch;
    unsigned int totalChars = 0;
    while ((ch = fgetc(in)) != EOF) {
        freq[ch]++;
        totalChars++;
    }

    // Build Huffman tree and generate codes
    Node* root = buildHuffmanTree(freq);
    HuffCode codes[256] = {0};
    if (root) generateCodes(root, codes, 0, 0);

    // Store compressed output
    FILE *out = fopen(outputFilename, "wb");
    if (!out) { perror("Error creating output file"); fclose(in); return; }

    fwrite(&totalChars, sizeof(unsigned int), 1, out);
    fwrite(freq, sizeof(unsigned int), 256, out);

    // Rewind input and write compressed bits
    fseek(in, 0, SEEK_SET);
    uint8_t buffer = 0;
    int bitsInBuffer = 0;

    while ((ch = fgetc(in)) != EOF) {
        HuffCode c = codes[ch];
        for (int i = c.length - 1; i >= 0; i--) {
            int bit = (c.code >> i) & 1;
            buffer = (buffer << 1) | bit;
            bitsInBuffer++;

            if (bitsInBuffer == 8) {
                fputc(buffer, out);
                buffer = 0;
                bitsInBuffer = 0;
            }
        }
    }

    // Write any remaining bits in the buffer
    if (bitsInBuffer > 0) {
        buffer = buffer << (8 - bitsInBuffer); // pad with zeros
        fputc(buffer, out);
    }

    freeTree(root);
    fclose(in);
    fclose(out);
}

void decompressLogFile(const char *inputFilename, const char *outputFilename) {
    FILE *in = fopen(inputFilename, "rb");
    if (!in) { perror("Error opening compressed file"); return; }

    // Read header to reconstruct the tree
    unsigned int totalChars = 0;
    unsigned int freq[256] = {0};
    fread(&totalChars, sizeof(unsigned int), 1, in);
    fread(freq, sizeof(unsigned int), 256, in);

    Node* root = buildHuffmanTree(freq);
    if (!root) { fclose(in); return; }

    FILE *out = fopen(outputFilename, "wb");
    if (!out) { perror("Error creating decompressed file"); fclose(in); freeTree(root); return; }

    // Read bits and traverse the tree to reconstruct original file
    Node* current = root;
    unsigned int charsWritten = 0;
    int ch;

    while ((ch = fgetc(in)) != EOF && charsWritten < totalChars) {
        for (int i = 7; i >= 0; i--) {
            int bit = (ch >> i) & 1;
            
            if (bit == 0) current = current->left;
            else current = current->right;

            if (!current->left && !current->right) {
                fputc(current->data, out);
                charsWritten++;
                current = root;
                if (charsWritten == totalChars) break; 
            }
        }
    }

    freeTree(root);
    fclose(in);
    fclose(out);
}
