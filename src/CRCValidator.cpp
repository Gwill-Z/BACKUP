#include "CRCValidator.h"

uint32_t CRCValidator::calculateCRC(const std::string& data) {
    // 实现CRC计算逻辑
    uint32_t crc = 0xFFFFFFFF;
    for (char c : data) {
        crc ^= static_cast<uint8_t>(c);
        for (int i = 0; i < 8; i++) {
            crc = (crc >> 1) ^ ((crc & 1) ? 0xEDB88320 : 0);
        }
    }
    return ~crc;
}
