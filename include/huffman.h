#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <vector>
#include <queue>
#include <map>
#include <string>

using namespace std;

// Node in the Huffman tree
struct Tree {
    int frequency;
    unsigned char character;
    Tree *left = nullptr;
    Tree *right = nullptr;
};

// Custom comparator for the priority queue
class TreeComparator {
public:
    bool operator()(Tree *a, Tree *b);
};

// Huffman functions
Tree *buildHuffmanTree(vector<pair<unsigned char, int>> freqtable);
void traverseHuffmanTree(Tree *root, string prev, string toAppend, map<unsigned char, string> &codemap);
string getHuffmanBitstring(unsigned char *buffer, map<unsigned char, string> codes, int sz, int &paddedBits);
void compressFile(const char *path, const char *output_path, map<unsigned char, string> &codes);
void decompressFile(const char *inputPath, const char *outputPath);

#endif