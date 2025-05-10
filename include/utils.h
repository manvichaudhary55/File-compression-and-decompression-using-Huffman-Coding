#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// Utility functions
string toBinary(unsigned char a);
unsigned char *readFileIntoBuffer(const char *path, int &sz);
void writeFileFromBuffer(const char *path, unsigned char *buffer, int sz, int flag);
vector<pair<unsigned char, int>> convertToVector(map<unsigned char, int> codes);
unsigned char *getBufferFromString(string bitstring, vector<unsigned char> &outputBuffer, int &sz);
string getStringFromBuffer(unsigned char *buffer, int sz);
unsigned char *getDecodedBuffer(string bitstring, vector<unsigned char> &buffer, map<unsigned char, string> codes,
                                int &sz, int paddedBits);
void writeHeader(const char *path, map<unsigned char, string> codes, int paddedBits);
unsigned char *readHeader(unsigned char *buffer, map<unsigned char, string> &codes, int &paddedBits, int &sz);

#endif