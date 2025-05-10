#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

unsigned char* readFileIntoBuffer(const char *path, int &sz);
void writeFileFromBuffer(const char *path, unsigned char *buffer, int sz, int flag);
std::string toBinary(unsigned char a);
unsigned char* getBufferFromString(std::string bitstring, std::vector<unsigned char> &outputBuffer, int &sz);
std::string getStringFromBuffer(unsigned char *buffer, int sz);

#endif