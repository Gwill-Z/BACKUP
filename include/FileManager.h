#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <tuple>

class FileManager {
public:
    std::vector<std::pair<std::string, std::string>> readPath(const std::string& path);
    void writeBackupFile(const std::string& backupFilePath, const std::string& data, uint32_t crc);
    void createDirectory(const std::string& directoryPath);
    void restoreFiles(const std::vector<std::pair<std::string, std::string>>& files, const std::string& targetPath);
    std::tuple<std::string, uint32_t> readBackupFile(const std::string& backupFilePath);

private:
    bool isDirectory(const std::string& path);
    void readDirectory(const std::string& directoryPath, std::vector<std::pair<std::string, std::string>>& files);
    std::string readFile(const std::string& filePath);
};

#endif // FILE_MANAGER_H
