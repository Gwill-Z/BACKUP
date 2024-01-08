#include "BackupConfig.h"
#include <fstream>
#include <iostream>

BackupConfig& BackupConfig::getInstance() {
    static BackupConfig instance;
    return instance;
}

void BackupConfig::setBackupPath(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex);
    backupPath = path;
}

std::string BackupConfig::getBackupPath() const {
    std::lock_guard<std::mutex> lock(mutex);
    return backupPath;
}

void BackupConfig::loadConfig(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);
    if (configFile) {
        std::getline(configFile, backupPath);
    }

    // 如果backupPath不存在，则创建该目录
    if (!std::filesystem::exists(backupPath)) {
        std::filesystem::create_directory(backupPath);
    }
}

void BackupConfig::saveConfig(const std::string& configFilePath) const {
    std::ofstream configFile(configFilePath);
    if (configFile) {
        configFile << backupPath;
    } else {
        std::cerr << "Unable to save config to file: " << configFilePath << std::endl;
    }
}
