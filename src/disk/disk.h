//
// Created by 兰海英 on 2022/10/20.
//

#ifndef BLADEBIT_DISK_H
#define BLADEBIT_DISK_H
#include <cstdio>
#include<fstream>
#include "util/Log.h"
#include "b3/blake3.h"

bool FileExists(std::string fileName);
void saveToFile(std::string fileName, char* data, size_t  size , bool re_write=false);
size_t readFromFile(std::string fileName, char* data, size_t  size );
bool endsWith(const std::string& str, const std::string suffix);
bool startsWith(const std::string& str, const std::string prefix);
void saveHashToFile(std::string fileName, char* data, size_t  size);
inline int bytesToHexStr( const byte* src, size_t srcSize,
                          char* dst, size_t dstSize,
                          size_t& numEncoded,
                          bool uppercase = false );
#endif //BLADEBIT_DISK_H

