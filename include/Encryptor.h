#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <string>
#include <vector>

class Encryptor {
public:
    std::vector<std::pair<std::string, std::string>>& encrypt(std::vector<std::pair<std::string, std::string>>& data);
    std::vector<std::pair<std::string, std::string>>& decrypt(std::vector<std::pair<std::string, std::string>>& data);

private:
    // 可以添加私有成员来支持加密和解密功能
};

#endif // ENCRYPTOR_H
