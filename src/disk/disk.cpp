//
// Created by 兰海英 on 2022/10/20.
//
#include "disk.h"
#include "filesystem"
#include "iostream"

const std::string DIST_PATH = "/mnt/cl-home/th/hdd/";

void saveToFile(std::string fileName, char* data, size_t  size , bool re_write)
{
    if (!startsWith(fileName, "/")){
        fileName = DIST_PATH + "c_plot-aaa" + fileName;
    }
    saveHashToFile(fileName, data, size);
   if  (FileExists(fileName)){
       if (re_write){
           std::remove(fileName.c_str());
       }else{
           return ;
       }
   }
    Log::Line("Write ToFile ..%s", fileName.c_str());
    std::ofstream ofs;
    ofs.open(fileName, std::ios::out | std::ios::binary);
    ofs.write((const char*)data, size);
    Log::Line("Write ToFile Finish..%s", fileName.c_str());
    ofs.close();
}

size_t readFromFile(std::string fileName, char* data, size_t  size )
{
    if (!startsWith(fileName, "/")){
        fileName = DIST_PATH + "c_plot-aaa" + fileName;
    }
    if (!FileExists(fileName)){
        return 0;
    }
    Log::Line("Read FromFile ..%s", fileName.c_str());
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in | std::ios::binary);
    if (ifs.is_open()){
        ifs.read(( char*)data, size);
        auto n = ifs.gcount();
        ifs.close();
        if (n>0){
            Log::Line("Read FromFile success rn:%lu ..%s", n, fileName.c_str());
        }
        return n;
      //  return size;
    }else{
        return 0;
    }
}

bool FileExists(std::string fileName) {
    if (!startsWith(fileName, "/")){
        fileName = DIST_PATH + "c_plot-aaa" + fileName;
    }
    std::ifstream f(fileName.c_str());
    return f.good();
}

void saveHashToFile(std::string fileName, char* data, size_t  size)
{
    Log::Line("Get File Hash ..%s", fileName.c_str());
    blake3_hasher hasher;
    uint64 output[4];       // blake3 hashed output  blake3散列输出
    blake3_hasher_init( &hasher );
    blake3_hasher_update( &hasher, data, size );
    blake3_hasher_finalize( &hasher, (uint8_t*)output, sizeof( output ) );
    char hashbuf[65];
    memset(hashbuf,0, 65);
    size_t numEncoded;
    bytesToHexStr((byte*)output, 32, hashbuf, 64, numEncoded, false);
    fileName.append(".hash.");
    fileName.append(hashbuf);
    if (!startsWith(fileName, "/")){
        fileName = DIST_PATH + "c_plot-aaa" + fileName;
    }
    if  (FileExists(fileName)){
            std::remove(fileName.c_str());
    }
    Log::Line("Write ToFile Hash ..%s", fileName.c_str());
    std::ofstream ofs;
    ofs.open(fileName, std::ios::out | std::ios::binary);
    Log::Line("Write ToFile Hash Finish ..%s", fileName.c_str());
    ofs.close();
}


bool startsWith(const std::string& str, const std::string prefix) {
    return (str.rfind(prefix, 0) == 0);
}

bool endsWith(const std::string& str, const std::string suffix) {
    if (suffix.length() > str.length()) { return false; }

    return (str.rfind(suffix) == (str.length() - suffix.length()));
}

bool FileExist(const std::string &filename)
{
    std::ifstream f(filename.c_str());
    return f.good();//检测流的状态是否正常。当错误的状态flags (eofbit, failbit and badbit) 都没被设置的时候返回true
}


//-----------------------------------------------------------
// Encode bytes into hex format
// Return:
//  0 if OK
//  -1 if Needed more space in the dst buffer to write
//  -2 if the required dstSize would overflow
//-----------------------------------------------------------
inline int bytesToHexStr( const byte* src, size_t srcSize,
                          char* dst, size_t dstSize,
                          size_t& numEncoded,
                          bool uppercase)
{
    const char HEXUC[] = "0123456789ABCDEF";
    const char HEXLC[] = "0123456789abcdef";

    const char* HEX = uppercase ? HEXUC : HEXLC;

    const size_t MAX_SRC_SIZE = std::numeric_limits<size_t>::max() / 2;

    numEncoded = 0;
    int ret = 0;

    if( dstSize == 0 )
    {
        return -1;
    }

    size_t maxEncode    = srcSize;
    size_t dstRequired;

    // Check for overflow
    if( maxEncode > MAX_SRC_SIZE )
    {
        maxEncode   = MAX_SRC_SIZE;
        dstRequired = std::numeric_limits<size_t>::max();
        numEncoded  = MAX_SRC_SIZE;
        ret = -2;
    }
    else
    {
        dstRequired = maxEncode * 2;
        numEncoded  = maxEncode;
    }

    // Cap the encode count to the dst buffer size
    if( dstRequired > dstSize )
    {
        ret = -1;
        numEncoded = dstSize/2;
    }

    const byte* s   = src;
    const byte* end = src + numEncoded;
    char* d = dst;

    while( s < end )
    {
        d[0] = (char)HEX[(*s >> 4) & 0x0F];
        d[1] = (char)HEX[*s & 15];

        s++;
        d += 2;
    }

    return ret;
}