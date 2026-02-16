#include <stdio.h>
#include "huffman.h"

// Main function
int main() {
    const char *original_log = "machine_logs.txt";
    const char *compressed_log = "compressed.log";
    const char *decompressed_log = "decompressed.log";

    printf("== Industrial Firmware Log Compressor == \n\n");

    // Compression program 
    printf("[*] Starting Compression...\n");
    if (compress_file(original_log, compressed_log)) {
        long size_orig = get_file_size(original_log);
        long size_comp = get_file_size(compressed_log);

        if (size_orig > 0 && size_comp > 0) {
            printf("    Original Size:   %ld bytes\n", size_orig);
            printf("    Compressed Size: %ld bytes\n", size_comp);
            
            double efficiency = 100.0 * (1.0 - ((double)size_comp / size_orig));
            printf("    Space Saved:     %.2f%%\n", efficiency);
        } else {
            printf("    [!] Error: Could not read file sizes.\n");
        }
    } else {
        printf("    [!] Compression failed.\n");
        return 1;
    }

    // Decompression program
    printf("\n[*] Starting Decompression...\n");
    if (decompress_file(compressed_log, decompressed_log)) {
        printf("    Decompression complete.\n");
    } else {
        printf("    [!] Decompression failed.\n");
        return 1;
    }

    // Data integrity verification
    printf("\n[*] Verifying Data Integrity...\n");
    if (verify_integrity(original_log, decompressed_log)) {
        printf("    [SUCCESS] Data integrity verified. Files match exactly.\n");
    } else {
        printf("    [FAILURE] Integrity warning! Decompressed file differs from original.\n");
    }

    return 0;
}
