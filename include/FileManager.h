#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

class FileManager {
public:
    std::vector<std::pair<std::string, std::string>> readPath(const std::string& path);
    void writeFile(const std::string& backupFilePath, std::vector<std::pair<std::string, std::string>>& files);
    void createDirectory(const std::string& directoryPath);
    void restoreFiles(const std::string& backupFilePath, const std::string& targetPath);
private:
    bool isDirectory(const std::string& path);
    void readDirectory(const std::string& directoryPath, std::vector<std::pair<std::string, std::string>>& files);
    std::string readFile(const std::string& filePath);
};

#endif // FILE_MANAGER_H
