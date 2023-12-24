#ifndef CRCVALIDATOR_H
#define CRCVALIDATOR_H

#include <string>
#include <vector>

class CRCValidator {
public:
    uint32_t calculateCRC(std::vector<std::pair<std::string, std::string>>& data);
    bool validateCRC(std::vector<std::pair<std::string, std::string>>& data, uint32_t expectedCRC);

private:
    // 可以添加私有成员来支持CRC校验功能
};

#endif // CRCVALIDATOR_H
