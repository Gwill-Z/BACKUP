#include <string>
#include <mutex>

class BackupConfig {
public:
    static BackupConfig& getInstance();  // 获取单例实例的静态方法

    void setBackupPath(const std::string& path);
    std::string getBackupPath() const;
    void loadConfig(const std::string& configFilePath);  // 加载配置
    void saveConfig(const std::string& configFilePath) const;  // 保存配置


private:
    BackupConfig() = default;  // 私有构造函数
    BackupConfig(const BackupConfig&) = delete;  // 阻止复制
    BackupConfig& operator=(const BackupConfig&) = delete;  // 阻止赋值

    std::string backupPath;
    mutable std::mutex mutex;  // 用于线程安全的互斥锁
};
