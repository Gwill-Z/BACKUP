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
        // 读取原始数据
    auto files = fileManager.readPath(sourcePath);

    // 打包文件
    std::string packedData = packer.pack(files);

    // 压缩打包后的数据
    auto compressedData = compressor.compress(packedData);

    // 加密压缩后的数据
    auto encryptedData = encryptor.encrypt(compressedData);

    // 计算加密数据的CRC
    uint32_t crc = crcValidator.calculateCRC(encryptedData);

    // 将加密数据和CRC写入备份文件
    fileManager.writeBackupFile(backupPath, encryptedData, crc);

    // 记录日志
    Logger::log("Backup completed successfully.");

        return Result::Success;
    } catch (const std::exception& e) {
        Logger::log(std::string("Backup failed: ") + e.what());
        return Result::Failure;
    }
}

Result BackupManager::performRestore(const std::string& backupFilePath, const std::string& targetPath) {
    // 读取备份文件
    std::string encryptedData;
    uint32_t storedCRC;
    std::tie(encryptedData, storedCRC) = fileManager.readBackupFile(backupFilePath);
    Logger::log("backup start");
    // 验证CRC
    if (crcValidator.calculateCRC(encryptedData) != storedCRC) {
        throw std::runtime_error("CRC validation failed");
    }

    Logger::log("crc验证通过");

    // 解密数据
    std::string decompressedData = encryptor.decrypt(encryptedData);

    Logger::log("解密完成");

    // 解压缩数据
    std::string unpackedData = compressor.decompress(decompressedData);

    Logger::log("解压缩完成");

    // 解包数据
    auto files = packer.unpack(unpackedData);

    Logger::log("解包完成");
    Logger::log(files[0].first);

    // 恢复文件到目标路径
    fileManager.restoreFiles(files, targetPath);

    Logger::log("恢复文件完成");

    return Result::Success;
}