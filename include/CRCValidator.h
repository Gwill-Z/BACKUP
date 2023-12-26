#ifndef CRCVALIDATOR_H
#define CRCVALIDATOR_H

#include <string>

class CRCValidator {
public:
    uint32_t calculateCRC(const std::string& data);
};

#endif // CRCVALIDATOR_H
