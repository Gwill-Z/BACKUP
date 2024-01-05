#include "Encryptor.h"
#include <iostream>
#include <spdlog/spdlog.h>
// 定义一个加密标记
const std::string ENCRYPTION_MARKER = "[*ENCRYPTED-DATA*]";

void Encryptor::setKey(const std::string& key) {
    _key = key;
}

std::string Encryptor::encrypt(const std::string& data) const {
    // return ENCRYPTION_MARKER + xorEncryptDecrypt(data);
    return ENCRYPTION_MARKER + _key + '\0' + xorEncryptDecrypt(data);
}

std::string Encryptor::decrypt(const std::string& data) {
    return xorEncryptDecrypt(data);
}

std::string Encryptor::xorEncryptDecrypt(const std::string& data) const {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ _key[i % _key.size()];
    }
    return result;
}
