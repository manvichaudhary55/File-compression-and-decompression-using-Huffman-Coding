#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include "../include/huffman.h"
#include "../include/utils.h"

using namespace std;

// TreeComparator implementation
bool TreeComparator::operator()(Tree *a, Tree *b) {
    return a->frequency > b->frequency;
}

// Build the Huffman tree using frequency table
Tree *buildHuffmanTree(vector<pair<unsigned char, int>> freqtable) {
    priority_queue<Tree *, vector<Tree *>, TreeComparator> huffqueue;
    for (int i = 0; i < freqtable.size(); i++) {
        Tree *node = new Tree();
        node->frequency = freqtable[i].second;
        node->character = freqtable[i].first;
        huffqueue.push(node);
    }
    while (huffqueue.size() > 1) {
        Tree *a, *b;
        a = huffqueue.top();
        huffqueue.pop();
        b = huffqueue.top();
        huffqueue.pop();
        Tree *c = new Tree();
        c->frequency = a->frequency + b->frequency;
        c->left = a;
        c->right = b;
        huffqueue.push(c);
    }
    return huffqueue.top();
}

// Traverse the Huffman tree to generate codes
void traverseHuffmanTree(Tree *root, string prev, string toAppend, map<unsigned char, string> &codemap) {
    prev += toAppend;
    if (root->right == nullptr && root->left == nullptr) {
        codemap[root->character] = prev; // if leaf node, store with the character as key
    }
    if (root->right != nullptr) {
        traverseHuffmanTree(root->right, prev, "1", codemap);
    }
    if (root->left != nullptr) {
        traverseHuffmanTree(root->left, prev, "0", codemap);
    }
}

// Generate Huffman bitstring from buffer using codes
string getHuffmanBitstring(unsigned char *buffer, map<unsigned char, string> codes, int sz, int &paddedBits) {
    string outputBuffer = "";
    for (int i = 0; i < sz; i++) {
        // Generate the Huffman bitstring
        outputBuffer = outputBuffer + codes[buffer[i]];
    }
    if (outputBuffer.size() % 8 != 0) {
        int deficit = 8 * ((outputBuffer.size() / 8) + 1) - outputBuffer.size();
        paddedBits = deficit;
        for (int i = 0; i < deficit; i++) {
            outputBuffer += "0";
        }
    }
    return outputBuffer; // Return the Huffman-encoded bitstring
}

// Compress file function
void compressFile(const char *path, const char *output_path, map<unsigned char, string> &codes) {
    int sz = 0;
    int paddedBits = 0;
    map<unsigned char, int> freqtable;
    // Read the file into a buffer and get the size
    unsigned char *buffer = readFileIntoBuffer(path, sz);

    // Build frequency table
    for (int i = 0; i < sz; i++) {
        freqtable[buffer[i]]++;
    }
    // Convert frequency table to a vector of pairs and build Huffman tree
    Tree *root = buildHuffmanTree(convertToVector(freqtable));

    // Traverse the Huffman tree to get codes for each character
    traverseHuffmanTree(root, "", "", codes);

    // Get the Huffman encoded bitstring for the file content
    string outputString = getHuffmanBitstring(buffer, codes, sz, paddedBits);
    // Update size to the size of the output bitstring
    sz = outputString.size();

    // Convert the bitstring to a buffer of bytes
    vector<unsigned char> outputBufferV;
    getBufferFromString(outputString, outputBufferV, sz);
    unsigned char *outputBuffer = outputBufferV.data();

    // Write the header (codes and padded bits) to the output file
    writeHeader(output_path, codes, paddedBits);

    // Write the compressed data to the output file
    writeFileFromBuffer(output_path, outputBuffer, sz, 1);
}

// Decompress file function
void decompressFile(const char *inputPath, const char *outputPath) {
    int sz = 0;
    map<unsigned char, string> codes;
    int paddedBits = 0;
    unsigned char *fileBuffer = readFileIntoBuffer(inputPath, sz);
    fileBuffer = readHeader(fileBuffer, codes, paddedBits, sz);
    //Convert the remaining buffer to a bitstring
    string fileBitString = getStringFromBuffer(fileBuffer, sz);

    // Decode the bitstring using the Huffman codes
    vector<unsigned char> outputBufferV;
    unsigned char *outputBuffer;
    getDecodedBuffer(fileBitString, outputBufferV, codes, sz, paddedBits);
    // Get the raw data pointer from the vector
    outputBuffer = outputBufferV.data();

    // Write the decompressed data to the output file
    writeFileFromBuffer(outputPath, outputBuffer, sz, 0);
}