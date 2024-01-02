#include "BackupManager.h"
#include "Encryptor.h"
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>

int main() {
    BackupManager backupManager;
    int choice = 0;

    while (true) {
        std::cout << "请选择操作：\n";
        std::cout << "1. 备份\n";
        std::cout << "2. 还原\n";
        std::cout << "3. 退出\n";
        std::cout << "输入选择（1-3）: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string sourcePath, backupPath, key;
            std::cout << "输入要备份的源路径: ";
            std::cin >> sourcePath;
            std::cout << "输入备份文件保存路径: ";
            std::cin >> backupPath;

            // 加密选项
            std::cout << "是否加密备份？(yes/no): ";
            std::string encryptChoice = "no";
            std::cin >> encryptChoice;
            if (encryptChoice == "yes") {
                std::cout << "输入加密密钥: ";
                // 以字符串形式读入密钥
                std::cin >> key;
            }
            backupManager.setSourcePath(sourcePath);
            backupManager.setBackupPath(backupPath);
            try {
                // 执行备份操作
                backupManager.performBackup(encryptChoice, key);
                std::cout << "备份完成。\n";
            } catch (const std::exception& e) {
                std::cerr << "备份失败: " << e.what() << std::endl;
            }
        } else if (choice == 2) {
            std::string backupPath, restorePath, key;
            std::cout << "输入要还原的备份文件路径: ";
            std::cin >> backupPath;
            std::cout << "输入还原数据的目标路径: ";
            std::cin >> restorePath;

            try {
                // 执行还原操作
                backupManager.performRestore(backupPath, restorePath);
                std::cout << "还原完成。\n";
            } catch (const std::exception& e) {
                std::cerr << "还原失败: " << e.what() << std::endl;
            }
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "无效选择，请重新输入。\n";
        }
    }

    std::cout << "程序退出。\n";
    return 0;
}
