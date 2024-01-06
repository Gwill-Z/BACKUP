#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <tuple>
#include <climits>
#include <ctime>

struct BackupFilter {
    // 文件大小过滤条件
    struct SizeRange {
        long long minSize; // 最小文件大小（字节）
        long long maxSize; // 最大文件大小（字节）
    };

    // 时间过滤条件
    struct TimeRange {
        std::time_t startTime; // 开始时间
        std::time_t endTime;   // 结束时间
    };

    SizeRange sizeFilter;       // 用户设置的大小过滤条件
    TimeRange timeFilter;       // 用户设置的时间过滤条件
    std::vector<std::string> fileFilter; // 用户选择过滤的文件路径
    std::vector<std::string> directoryFilter; // 用户选择过滤的目录路径

    // 构造函数，用于初始化结构体
    BackupFilter() : sizeFilter({0, LLONG_MAX}), timeFilter({0, LLONG_MAX}) {}
};

class FileManager {
public:
    BackupFilter backupFilter;
    std::vector<std::pair<std::string, std::string>> readPath(const std::string& path);
    void writeBackupFile(const std::string& backupFilePath, const std::string& data, uint32_t crc);
    void createDirectory(const std::string& directoryPath);
    void restoreFiles(const std::vector<std::pair<std::string, std::string>>& files, const std::string& targetPath);
    std::tuple<std::string, uint32_t> readBackupFile(const std::string& backupFilePath);
    bool isDirectory(const std::string& path);
    void readDirectory(const std::string& rootPath, const std::string& directoryPath, std::vector<std::pair<std::string, std::string>>& files);
    std::string readFile(const std::string& filePath);
    void setBackupFilter(long long minSize, long long maxSize, std::time_t startTime, std::time_t endTime, std::vector<std::string> fileFilter, std::vector<std::string> directoryFilter);
};

#endif // FILE_MANAGER_H
