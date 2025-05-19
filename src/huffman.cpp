#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include "../include/huffman.h"
#include "../include/utils.h"

using namespace std;

bool TreeComparator::operator()(Tree *a, Tree *b) {
    return a->frequency > b->frequency;
}

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

void traverseHuffmanTree(Tree *root, string prev, string toAppend, map<unsigned char, string> &codemap) {
    prev += toAppend;
    if (root->right == nullptr && root->left == nullptr) {
        codemap[root->character] = prev; 
    }
    if (root->right != nullptr) {
        traverseHuffmanTree(root->right, prev, "1", codemap);
    }
    if (root->left != nullptr) {
        traverseHuffmanTree(root->left, prev, "0", codemap);
    }
}

string getHuffmanBitstring(unsigned char *buffer, map<unsigned char, string> codes, int sz, int &paddedBits) {
    string outputBuffer = "";
    for (int i = 0; i < sz; i++) {
        outputBuffer = outputBuffer + codes[buffer[i]];
    }
    if (outputBuffer.size() % 8 != 0) {
        int deficit = 8 * ((outputBuffer.size() / 8) + 1) - outputBuffer.size();
        paddedBits = deficit;
        for (int i = 0; i < deficit; i++) {
            outputBuffer += "0";
        }
    }
    return outputBuffer; 
}

void compressFile(const char *path, const char *output_path, map<unsigned char, string> &codes) {
    int sz = 0;
    int paddedBits = 0;
    map<unsigned char, int> freqtable;
    unsigned char *buffer = readFileIntoBuffer(path, sz);

    for (int i = 0; i < sz; i++) {
        freqtable[buffer[i]]++;
    }
    Tree *root = buildHuffmanTree(convertToVector(freqtable));

    traverseHuffmanTree(root, "", "", codes);

    string outputString = getHuffmanBitstring(buffer, codes, sz, paddedBits);
    sz = outputString.size();

    vector<unsigned char> outputBufferV;
    getBufferFromString(outputString, outputBufferV, sz);
    unsigned char *outputBuffer = outputBufferV.data();

    writeHeader(output_path, codes, paddedBits);

    writeFileFromBuffer(output_path, outputBuffer, sz, 1);
}

void decompressFile(const char *inputPath, const char *outputPath) {
    int sz = 0;
    map<unsigned char, string> codes;
    int paddedBits = 0;
    unsigned char *fileBuffer = readFileIntoBuffer(inputPath, sz);
    fileBuffer = readHeader(fileBuffer, codes, paddedBits, sz);
    string fileBitString = getStringFromBuffer(fileBuffer, sz);

    vector<unsigned char> outputBufferV;
    unsigned char *outputBuffer;
    getDecodedBuffer(fileBitString, outputBufferV, codes, sz, paddedBits);
    outputBuffer = outputBufferV.data();

    writeFileFromBuffer(outputPath, outputBuffer, sz, 0);
}