#include <stdio.h>
#include "huffman.h"

int main() {
    const char *original_log = "machine_logs.txt";
    const char *compressed_log = "compressed.log";
    const char *decompressed_log = "decompressed.log";

    printf("== Industrial Log Compressor ==\n\n");

    // Compression Program
    printf("[*] Starting Compression...\n");
    
    compressLogFile(original_log, compressed_log);

    // Verify success by checking file sizes
    long size_orig = getFileSize(original_log);
    long size_comp = getFileSize(compressed_log);

    if (size_orig > 0 && size_comp > 0) {
        printf("    Original Size:   %ld bytes\n", size_orig);
        printf("    Compressed Size: %ld bytes\n", size_comp);
        
        double efficiency = 100.0 * (1.0 - ((double)size_comp / size_orig));
        printf("    Space Saved:     %.2f%%\n", efficiency);
    } else {
        printf("    [!] Error: Could not read file sizes or compression failed.\n");
        printf("        Ensure '%s' exists in the current directory.\n", original_log);
        return 1;
    }

    // Decompression program
    printf("\n[*] Starting Decompression...\n");
   
    decompressLogFile(compressed_log, decompressed_log);

    if (getFileSize(decompressed_log) > 0) {
        printf("    Decompression complete.\n");
    } else {
        printf("    [!] Decompression failed.\n");
        return 1;
    }

    // Data integrity verification 
    printf("\n[*] Verifying Data Integrity...\n");
    
    // Compare the original and decompressed files byte-by-byte
    if (compareFiles(original_log, decompressed_log)) {
        printf("    [SUCCESS] Data integrity verified. Files match exactly.\n");
    } else {
        printf("    [FAILURE] Integrity warning! Decompressed file differs from original.\n");
    }

    return 0;
}
