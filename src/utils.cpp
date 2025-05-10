#include "../include/utils.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

unsigned char* readFileIntoBuffer(const char *path, int &sz) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(sz);
    fread(buffer, 1, sz, fp);
    fclose(fp);
    return buffer;
}

void writeFileFromBuffer(const char *path, unsigned char *buffer, int sz, int flag) {
    FILE *fp = fopen(path, flag == 0 ? "wb" : "ab");
    if (!fp) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    fwrite(buffer, 1, sz, fp);
    fclose(fp);
}

std::string toBinary(unsigned char a) {
    std::string output;
    for (int i = 7; i >= 0; --i) output += ((a >> i) & 1) ? '1' : '0';
    return output;
}

unsigned char* getBufferFromString(std::string bitstring, std::vector<unsigned char> &outputBuffer, int &sz) {
    int interval = 0;
    unsigned char bit = 0;
    for (int i = 0; i < sz; i++) {
        bit = (bit << 1) | (bitstring[i] - '0');
        interval++;
        if (interval == 8) {
            outputBuffer.push_back(bit);
            interval = 0;
            bit = 0;
        }
    }
    sz = outputBuffer.size();
    return outputBuffer.data();
}

std::string getStringFromBuffer(unsigned char *buffer, int sz) {
    std::string bitstring;
    for (int i = 0; i < sz; i++) bitstring += toBinary(buffer[i]);
    return bitstring;
}