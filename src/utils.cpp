#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "../include/utils.h"

using namespace std;

// Convert byte to binary string
string toBinary(unsigned char a) {
    string output = "";
    while (a != 0) {
        string bit = a % 2 == 0 ? "0" : "1";
        output += bit;
        a /= 2;
    }
    if (output.size() < 8) {
        int deficit = 8 - output.size();
        for (int i = 0; i < deficit; i++) {
            output += "0";
        }
    }
    reverse(output.begin(), output.end());
    return output;
}

// Read file into buffer
unsigned char *readFileIntoBuffer(const char *path, int &sz) {
    FILE *fp = fopen(path, "rb");
    if (fp == nullptr) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    fseek(fp, 0, SEEK_END);  
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *) malloc(sz);
    fread(buffer, 1, sz, fp);
    fclose(fp);
    return buffer;
}

// Write buffer to file
void writeFileFromBuffer(const char *path, unsigned char *buffer, int sz, int flag) {
    FILE *fp;
    if (flag == 0) {
        fp = fopen(path, "wb");
    } else {
        fp = fopen(path, "ab");
    }
    if (fp == nullptr) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    fwrite(buffer, 1, sz, fp);
    fclose(fp);
}

// Convert map to vector
vector<pair<unsigned char, int>> convertToVector(map<unsigned char, int> codes) {
    vector<pair<unsigned char, int>> codesV;
    for (map<unsigned char, int>::iterator i = codes.begin(); i != codes.end(); i++) {
        codesV.push_back(make_pair(i->first, i->second));
    }
    return codesV;
}

// Convert bitstring to buffer
unsigned char *getBufferFromString(string bitstring, vector<unsigned char> &outputBuffer, int &sz) {
    int interval = 0;
    unsigned char bit = 0;
    for (int i = 0; i < sz; i++) {
        bit = (bit << 1) | (bitstring[i] - '0');
        interval++;
        if (interval == 8) {
            interval = 0;
            outputBuffer.push_back(bit);
            bit = 0;
        }
    }
    sz = outputBuffer.size();
    return outputBuffer.data();
}

// Convert buffer to bitstring
string getStringFromBuffer(unsigned char *buffer, int sz) {
    string bitstring = "";
    for (int i = 0; i < sz; i++) {
        bitstring += toBinary(buffer[i]);
    }
    return bitstring;
}

// Decode buffer from Huffman-encoded bitstring
unsigned char *getDecodedBuffer(string bitstring, vector<unsigned char> &buffer, map<unsigned char, string> codes,
                                int &sz, int paddedBits) {
    string bit = "";
    map<string, unsigned char> reversecodes;
    for (map<unsigned char, string>::iterator i = codes.begin(); i != codes.end(); i++) {
        reversecodes[i->second] = i->first;
    }
    for (int i = 0; i < bitstring.size() - paddedBits; i++) {
        bit += string(1, bitstring[i]);
        if (reversecodes.find(bit) != reversecodes.end()) {
            buffer.push_back(reversecodes[bit]);
            bit = "";
        }
    }
    sz = buffer.size();
    return buffer.data();
}

// Write header to compressed file
void writeHeader(const char *path, map<unsigned char, string> codes, int paddedBits) {
    int size = codes.size();
    writeFileFromBuffer(path, (unsigned char *) &paddedBits, sizeof(int), 0);
    writeFileFromBuffer(path, (unsigned char *) &size, sizeof(int), 1);
    for (map<unsigned char, string>::iterator i = codes.begin(); i != codes.end(); i++) {
        unsigned char key = i->first;
        int len = i->second.size();
        writeFileFromBuffer(path, (unsigned char *) &key, 1, 1);
        writeFileFromBuffer(path, (unsigned char *) &len, sizeof(int), 1);
        writeFileFromBuffer(path, (unsigned char *) i->second.c_str(), len, 1);
    }
}

// Read header from compressed file
unsigned char *readHeader(unsigned char *buffer, map<unsigned char, string> &codes, int &paddedBits, int &sz) {
    paddedBits = *((int *) buffer);
    buffer = buffer + 4;
    sz -= 4;
    int size = *((int *) buffer);
    buffer = buffer + 4;
    sz -= 4;
    for (int i = 0; i < size; i++) {
        unsigned char key = buffer[0];
        buffer++;
        sz--;
        int len = *((int *) buffer);
        buffer += 4;
        sz -= 4;
        char *value = (char *) malloc(len + 1);
        for (int j = 0; j < len; j++) {
            value[j] = buffer[j];
        }
        buffer += len;
        sz -= len;
        value[len] = '\0';
        codes[key] = value;
    }
    return buffer;
}