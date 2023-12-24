#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

class Logger {
public:
    static void log(const std::string& message);
};

#endif // LOGGER_H
