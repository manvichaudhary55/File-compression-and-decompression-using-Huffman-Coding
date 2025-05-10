#include "../include/huffman.h"
#include "../include/utils.h"
#include <queue>
#include <cstdlib>

bool TreeComparator::operator()(Tree *a, Tree *b) {
    return a->frequency > b->frequency;
}

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

std::string getHuffmanBitstring(unsigned char *buffer, std::map<unsigned char, std::string> codes, int sz, int &paddedBits) {
    std::string outputBuffer;
    for (int i = 0; i < sz; i++) outputBuffer += codes[buffer[i]];
    if (outputBuffer.size() % 8 != 0) {
        paddedBits = 8 - (outputBuffer.size() % 8);
        outputBuffer.append(paddedBits, '0');
    }
    return outputBuffer;
}

unsigned char* getDecodedBuffer(std::string bitstring, std::vector<unsigned char> &buffer, std::map<unsigned char, std::string> codes, int &sz, int paddedBits) {
    std::map<std::string, unsigned char> reversecodes;
    for (auto &entry : codes) reversecodes[entry.second] = entry.first;
    std::string bit;
    for (int i = 0; i < bitstring.size() - paddedBits; i++) {
        bit += bitstring[i];
        if (reversecodes.count(bit)) {
            buffer.push_back(reversecodes[bit]);
            bit.clear();
        }
    }
    sz = buffer.size();
    return buffer.data();
}

void writeHeader(const char *path, std::map<unsigned char, std::string> codes, int paddedBits) {
    int size = codes.size();
    writeFileFromBuffer(path, (unsigned char*)&paddedBits, sizeof(int), 0);
    writeFileFromBuffer(path, (unsigned char*)&size, sizeof(int), 1);
    for (auto &entry : codes) {
        unsigned char key = entry.first;
        int len = entry.second.size();
        writeFileFromBuffer(path, &key, 1, 1);
        writeFileFromBuffer(path, (unsigned char*)&len, sizeof(int), 1);
        writeFileFromBuffer(path, (unsigned char*)entry.second.c_str(), len, 1);
    }
}

unsigned char* readHeader(unsigned char *buffer, std::map<unsigned char, std::string> &codes, int &paddedBits, int &sz) {
    paddedBits = *((int*)buffer); buffer += 4; sz -= 4;
    int size = *((int*)buffer); buffer += 4; sz -= 4;
    for (int i = 0; i < size; i++) {
        unsigned char key = *buffer++; sz--;
        int len = *((int*)buffer); buffer += 4; sz -= 4;
        std::string value((char*)buffer, len); buffer += len; sz -= len;
        codes[key] = value;
    }
    return buffer;
}

void compressFile(const char *path, const char *output_path, std::map<unsigned char, std::string> &codes) {
    int sz = 0, paddedBits = 0;
    unsigned char *buffer = readFileIntoBuffer(path, sz);
    std::map<unsigned char, int> freqtable;
    for (int i = 0; i < sz; i++) freqtable[buffer[i]]++;
    Tree *root = buildHuffmanTree(convertToVector(freqtable));
    traverseHuffmanTree(root, "", "", codes);
    std::string outputString = getHuffmanBitstring(buffer, codes, sz, paddedBits);
    sz = outputString.size();
    std::vector<unsigned char> outputBufferV;
    getBufferFromString(outputString, outputBufferV, sz);
    unsigned char *outputBuffer = outputBufferV.data();
    writeHeader(output_path, codes, paddedBits);
    writeFileFromBuffer(output_path, outputBuffer, sz, 1);
}

void decompressFile(const char *inputPath, const char *outputPath) {
    int sz = 0, paddedBits = 0;
    std::map<unsigned char, std::string> codes;
    unsigned char *fileBuffer = readFileIntoBuffer(inputPath, sz);
    fileBuffer = readHeader(fileBuffer, codes, paddedBits, sz);
    std::string fileBitString = getStringFromBuffer(fileBuffer, sz);
    std::vector<unsigned char> outputBufferV;
    unsigned char *outputBuffer = getDecodedBuffer(fileBitString, outputBufferV, codes, sz, paddedBits);
    writeFileFromBuffer(outputPath, outputBuffer, sz, 0);
}
