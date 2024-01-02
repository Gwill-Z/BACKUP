#include "Encryptor.h"
#include <iostream>
#include <spdlog/spdlog.h>
// 定义一个加密标记
const std::string ENCRYPTION_MARKER = "[*ENCRYPTED-DATA*]";

void Encryptor::setKey(const std::string& key) {
    _key = key;
}

std::string Encryptor::encrypt(const std::string& data) const {
    return ENCRYPTION_MARKER + xorEncryptDecrypt(data);
}

std::string Encryptor::decrypt(const std::string& data) {
    if (data.compare(0, ENCRYPTION_MARKER.size(), ENCRYPTION_MARKER) == 0) {
        std::cout << "已检测到加密数据\n" << "请输入密钥: ";
        std::string key;
        std::cin >> key;
        setKey(key);
        // 去除加密标记后解密
        return xorEncryptDecrypt(data.substr(ENCRYPTION_MARKER.size()));
    } else {
        // 如果数据没有加密标记，则原样返回
        return data;
    }
}

std::string Encryptor::xorEncryptDecrypt(const std::string& data) const {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ _key[i % _key.size()];
        spdlog::info("result[i]: {}", result[i]);
    }
    return result;
}
