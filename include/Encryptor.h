#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <string>

class Encryptor {
public:
    Encryptor() = default;

    void setKey(const std::string& key);
    std::string encrypt(const std::string& data) const;
    std::string decrypt(const std::string& data) ;

private:
    std::string _key;
    std::string xorEncryptDecrypt(const std::string& data) const;
};

#endif // ENCRYPTOR_H
