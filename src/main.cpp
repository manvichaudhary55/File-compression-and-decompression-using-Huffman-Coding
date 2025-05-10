#include <iostream>
#include <string>
#include "../include/huffman.h"
#include "../include/utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cout << "Usage: " << endl;
        cout << "For compression: " << argv[0] << " -c input_file output_file" << endl;
        cout << "For decompression: " << argv[0] << " -d input_file output_file" << endl;
        return 1;
    }

    string mode = argv[1];
    const char *inputFile = argv[2];
    const char *outputFile = argv[3];
    
    if (mode == "-c") {
        // Compression mode
        map<unsigned char, string> codes;
        compressFile(inputFile, outputFile, codes);
        cout << "Compression completed successfully!" << endl;
    } 
    else if (mode == "-d") {
        // Decompression mode
        decompressFile(inputFile, outputFile);
        cout << "Decompression completed successfully!" << endl;
    }
    else {
        cout << "Invalid mode. Use -c for compression or -d for decompression." << endl;
        return 1;
    }
    
    return 0;
}