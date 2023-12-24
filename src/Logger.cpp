#include "Logger.h"

void Logger::log(const std::string& message) {
    // 输出日志到控制台，也可以选择输出到文件
    std::cout << "Log: " << message << std::endl;
}
