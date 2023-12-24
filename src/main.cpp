#include "BackupManager.h"
#include <iostream>
#include <string>

int main() {
    try {
        BackupManager backupManager;

        // 设置源路径和备份路径
        std::string sourcePath = "/path/to/source"; // 指定源路径
        std::string backupPath = "/path/to/backup/file.backup"; // 指定备份文件路径

        backupManager.setSourcePath(sourcePath);
        backupManager.setBackupPath(backupPath);

        // 执行备份操作
        Result backupResult = backupManager.performBackup();
        if (backupResult == Result::Success) {
            std::cout << "Backup completed successfully." << std::endl;
        } else {
            std::cerr << "Backup failed." << std::endl;
            // 可以根据不同的Result值提供更具体的错误信息
        }

        // 如果需要执行还原操作，可以调用performRestore
        // Result restoreResult = backupManager.performRestore();
        // ...

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
