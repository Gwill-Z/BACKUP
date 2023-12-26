#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>

class Compressor {
public:
    std::string compress(const std::string& data);
    std::string decompress(const std::string& data);
};

#endif // COMPRESSOR_H
