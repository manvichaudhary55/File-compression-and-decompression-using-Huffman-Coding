#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <vector>
#include <map>
#include <string>

struct Tree {
    int frequency;
    unsigned char character;
    Tree *left = nullptr;
    Tree *right = nullptr;
};

class TreeComparator {
public:
    bool operator()(Tree *a, Tree *b);
};

Tree* buildHuffmanTree(std::vector<std::pair<unsigned char, int>> freqtable);
void traverseHuffmanTree(Tree *root, std::string prev, std::string toAppend, std::map<unsigned char, std::string> &codemap);
std::string getHuffmanBitstring(unsigned char *buffer, std::map<unsigned char, std::string> codes, int sz, int &paddedBits);
unsigned char* getDecodedBuffer(std::string bitstring, std::vector<unsigned char> &buffer, std::map<unsigned char, std::string> codes, int &sz, int paddedBits);
std::vector<std::pair<unsigned char, int>> convertToVector(std::map<unsigned char, int> codes);
void writeHeader(const char *path, std::map<unsigned char, std::string> codes, int paddedBits);
unsigned char* readHeader(unsigned char *buffer, std::map<unsigned char, std::string> &codes, int &paddedBits, int &sz);
void compressFile(const char *path, const char *output_path, std::map<unsigned char, std::string> &codes);
void decompressFile(const char *inputPath, const char *outputPath);

#endif