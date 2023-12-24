#include "BackupManager.h"
#include <stdexcept>

BackupManager::BackupManager() {
    // 构造函数的实现（如果需要的话）
}

void BackupManager::setSourcePath(const std::string& path) {
    sourcePath = path;
}

void BackupManager::setBackupPath(const std::string& path) {
    backupPath = path;
}

Result BackupManager::performBackup() {
    try {
        // 1. 读取源数据
        auto data = fileManager.readPath(sourcePath);

        // 2. 压缩数据
        auto compressedData = compressor.compress(data);

        // 3. 加密数据
        auto encryptedData = encryptor.encrypt(compressedData);

        // 4. 计算CRC
        auto crc = crcValidator.calculateCRC(encryptedData);

        // 5. 写入备份文件
        fileManager.writeFile(backupPath, encryptedData);

        // 6. 记录日志
        Logger::log("Backup completed successfully.");

        return Result::Success;
    } catch (const std::exception& e) {
        Logger::log(std::string("Backup failed: ") + e.what());
        return Result::Failure;
    }
}

Result BackupManager::performRestore() {
    try {
        // 1. 读取备份数据
        auto encryptedData = fileManager.readPath(backupPath);

        // 2. 解密数据
        auto decryptedData = encryptor.decrypt(encryptedData);

        // 3. 解压缩数据
        auto decompressedData = compressor.decompress(decryptedData);

        // 4. 校验CRC
        auto isValidCRC = crcValidator.validateCRC(decompressedData, /* 预期的CRC值 */1);
        if (!isValidCRC) {
            throw std::runtime_error("CRC validation failed");
        }

        // 5. 写入恢复数据
        fileManager.writeFile(sourcePath, decompressedData);
        Logger::log("Restore completed successfully.");

        return Result::Success;
    } catch (const std::exception& e) {
        Logger::log(std::string("Restore failed: ") + e.what());
        return Result::Failure;
    }
}
