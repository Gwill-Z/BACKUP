#include "BackupManager.h"
#include "BackupConfig.h"
#include "Encryptor.h"
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>

int main() {
    BackupManager backupManager;
    BackupConfig& config = BackupConfig::getInstance();
    int choice = 0;

    // 加载配置
    config.loadConfig("../backup_config.txt");

    while (true) {
        std::cout << "请选择操作：\n";
        std::cout << "1. 设置备份路径\n";
        std::cout << "2. 备份\n";
        std::cout << "3. 还原\n";
        std::cout << "4. 退出\n";
        std::cout << "输入选择（1-4）: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string backupPath;
            std::cout << "输入备份文件保存路径: ";
            std::cin >> backupPath;
            config.setBackupPath(backupPath);
            config.saveConfig("../backup_config.txt");
        } else if (choice == 2) {
            std::string sourcePath,backupfileName, key;
            std::cout << "输入要备份的源路径: ";
            std::cin >> sourcePath;
            std::cout << "输入备份文件的名字:" ;
            std::cin >> backupfileName;

            // 加密选项
            std::cout << "是否加密备份？(yes/no): ";
            std::string encryptChoice;
            std::cin >> encryptChoice;
            if (encryptChoice == "yes") {
                std::cout << "输入加密密钥: ";
                std::cin >> key;
            }

            backupManager.setSourcePath(sourcePath);
            backupManager.setBackupPath(config.getBackupPath() + "/" + backupfileName + ".zth");
            try {
                backupManager.performBackup(encryptChoice, key);  // 适当修改 performBackup 以接受加密选项和密钥
                std::cout << "备份完成。\n";
            } catch (const std::exception& e) {
                std::cerr << "备份失败: " << e.what() << std::endl;
            }
        } else if (choice == 3) {
            std::string targetPath, fileName;
            std::cout << "输入要还原的备份文件名（.zth）: ";
            std::cin >> fileName;
            std::cout << "输入还原目标路径: ";
            std::cin >> targetPath;
            std::string fullBackupPath = config.getBackupPath() + "/" + fileName;   
            try {
                backupManager.performRestore(fullBackupPath, targetPath);  // 假设还原到备份路径下
                std::cout << "还原完成。\n";
            } catch (const std::exception& e) {
                std::cerr << "还原失败: " << e.what() << std::endl;
            }
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "无效选择，请重新输入。\n";
        }
    }

    std::cout << "程序退出。\n";
    return 0;
}
