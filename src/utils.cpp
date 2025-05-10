#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <stdexcept>

// Converts an unsigned char to its binary representation as a string
std::string toBinary(unsigned char a) {
    std::string output;
    for (int i = 7; i >= 0; --i) output += ((a >> i) & 1) ? '1' : '0';
    return output;
}

// Reads a file into a buffer and returns it
std::vector<unsigned char> readFileIntoBuffer(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        throw std::runtime_error("Error opening file for reading");
    }
    fseek(fp, 0, SEEK_END);
    int sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::vector<unsigned char> buffer(sz);
    fread(buffer.data(), 1, sz, fp);
    fclose(fp);
    return buffer;
}

// Writes a buffer to a file
void writeFileFromBuffer(const char *path, const unsigned char *buffer, int sz, int flag) {
    FILE *fp = fopen(path, flag == 0 ? "wb" : "ab");
    if (!fp) {
        throw std::runtime_error("Error opening file for writing");
    }
    fwrite(buffer, 1, sz, fp);
    fclose(fp);
}

// Converts a bitstring to a buffer
std::vector<unsigned char> getBufferFromString(const std::string &bitstring) {
    std::vector<unsigned char> outputBuffer;
    unsigned char bit = 0;
    int interval = 0;

    for (char c : bitstring) {
        bit = (bit << 1) | (c - '0');
        interval++;
        if (interval == 8) {
            outputBuffer.push_back(bit);
            interval = 0;
            bit = 0;
        }
    }
    return outputBuffer;
}

// Converts a buffer to a bitstring
std::string getStringFromBuffer(const unsigned char *buffer, int sz) {
    std::string bitstring;
    for (int i = 0; i < sz; i++) {
        bitstring += toBinary(buffer[i]);
    }
    return bitstring;
}

// Encodes a buffer into a Huffman bitstring
std::string getHuffmanBitstring(const unsigned char *buffer, const std::map<unsigned char, std::string> &codes, int sz, int &paddedBits) {
    std::string outputBuffer;
    for (int i = 0; i < sz; i++) {
        outputBuffer += codes.at(buffer[i]);
    }
    if (outputBuffer.size() % 8 != 0) {
        paddedBits = 8 - (outputBuffer.size() % 8);
        outputBuffer.append(paddedBits, '0');
    }
    return outputBuffer;
}

// Decodes a Huffman bitstring into a buffer
std::vector<unsigned char> getDecodedBuffer(const std::string &bitstring, const std::map<unsigned char, std::string> &codes, int paddedBits) {
    std::map<std::string, unsigned char> reverseCodes;
    for (const auto &entry : codes) {
        reverseCodes[entry.second] = entry.first;
    }

    std::vector<unsigned char> buffer;
    std::string bit;
    for (size_t i = 0; i < bitstring.size() - paddedBits; i++) {
        bit += bitstring[i];
        if (reverseCodes.count(bit)) {
            buffer.push_back(reverseCodes[bit]);
            bit.clear();
        }
    }
    return buffer;
}