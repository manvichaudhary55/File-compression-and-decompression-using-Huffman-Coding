#include <vector>
#include <queue>
#include <map>
#include <string>
#include <cstdlib>
#include <iostream>

struct Tree {
    int frequency;
    unsigned char character;
    Tree *left = nullptr;
    Tree *right = nullptr;
};

class TreeComparator {
public:
    bool operator()(Tree *a, Tree *b) {
        return a->frequency > b->frequency;
    }
};

Tree* buildHuffmanTree(std::vector<std::pair<unsigned char, int>> freqtable) {
    std::priority_queue<Tree*, std::vector<Tree*>, TreeComparator> huffqueue;
    for (auto &pair : freqtable) {
        Tree *node = new Tree{pair.second, pair.first};
        huffqueue.push(node);
    }
    while (huffqueue.size() > 1) {
        Tree *a = huffqueue.top(); huffqueue.pop();
        Tree *b = huffqueue.top(); huffqueue.pop();
        Tree *c = new Tree{a->frequency + b->frequency, 0, a, b};
        huffqueue.push(c);
    }
    return huffqueue.top();
}

void traverseHuffmanTree(Tree *root, std::string prev, std::string toAppend, std::map<unsigned char, std::string> &codemap) {
    prev += toAppend;
    if (!root->left && !root->right) codemap[root->character] = prev;
    if (root->right) traverseHuffmanTree(root->right, prev, "1", codemap);
    if (root->left) traverseHuffmanTree(root->left, prev, "0", codemap);
}

std::vector<std::pair<unsigned char, int>> convertToVector(std::map<unsigned char, int> codes) {
    std::vector<std::pair<unsigned char, int>> codesV;
    for (auto &entry : codes) codesV.push_back(entry);
    return codesV;
}

std::string getHuffmanBitstring(unsigned char *buffer, std::map<unsigned char, std::string> codes, int sz, int &paddedBits);
unsigned char* getDecodedBuffer(std::string bitstring, std::vector<unsigned char> &buffer, std::map<unsigned char, std::string> codes, int &sz, int paddedBits);

void writeHeader(const char *path, std::map<unsigned char, std::string> codes, int paddedBits);
unsigned char* readHeader(unsigned char *buffer, std::map<unsigned char, std::string> &codes, int &paddedBits, int &sz);

void compressFile(const char *path, const char *output_path, std::map<unsigned char, std::string> &codes);
void decompressFile(const char *inputPath, const char *outputPath);