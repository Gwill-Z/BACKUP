#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <string>

class Encryptor {
public:
    std::string encrypt(const std::string& data);
    std::string decrypt(const std::string& data);
};

#endif // ENCRYPTOR_H
