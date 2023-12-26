#ifndef PACKER_H
#define PACKER_H

#include <string>
#include <vector>

class Packer {
public:
    std::string pack(std::vector<std::pair<std::string, std::string>> files);
    std::vector<std::pair<std::string, std::string>> unpack(std::string packedData);
};

#endif // PACKER_H
