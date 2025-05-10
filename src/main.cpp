#include "../include/huffman.h"
#include <iostream>

int main() {
    const char *inputFile = "test_files/sample.txt";
    const char *compressedFile = "output/compressed.txt";
    const char *decompressedFile = "output/decompressed.txt";

    std::map<unsigned char, std::string> codes;
    compressFile(inputFile, compressedFile, codes);
    decompressFile(compressedFile, decompressedFile);

    std::cout << "Compression and decompression completed successfully!" << std::endl;
    return 0;
}