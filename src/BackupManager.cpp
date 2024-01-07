#include "BackupManager.h"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <iostream>

const std::string ENCRYPTION_MARKER = "[*ENCRYPTED-DATA*]";

BackupManager::BackupManager() {
    // 构造函数的实现（如果需要的话）
}

void BackupManager::setSourcePath(const std::string& path) {
    sourcePath = path;
}

void BackupManager::setBackupPath(const std::string& path) {
    backupPath = path;
}

Result BackupManager::performBackup(std::string key) {
    try {
        // 读取原始数据
    auto files = fileManager.readPath(sourcePath);

    // 打包文件
    std::string packedData = packer.pack(files);
    spdlog::info("Packed complete");

    // 压缩打包后的数据
    auto compressedData = compressor.compress(packedData);
    spdlog::info("Compressed complete");

    auto encryptedData = compressedData;
    // 加密压缩后的数据
    encryptor.setKey(key);
    encryptedData = encryptor.encrypt(compressedData);
    spdlog::info("Encrypted complete");
    
    // 计算加密数据的CRC
    uint32_t crc = crcValidator.calculateCRC(encryptedData);
    spdlog::info("CRC calculated");

    // 将加密数据和CRC写入备份文件
    fileManager.writeBackupFile(backupPath, encryptedData, crc);
    spdlog::info("Backup file written");
        return Result::Success;
    } catch (const std::exception& e) {
        return Result::Failure;
    }
}

Result BackupManager::performBackup() {
    try {
        // 读取原始数据
    auto files = fileManager.readPath(sourcePath);

    // 打包文件
    std::string packedData = packer.pack(files);
    spdlog::info("Packed complete");

    // 压缩打包后的数据
    auto compressedData = compressor.compress(packedData);
    spdlog::info("Compressed complete");

    auto encryptedData = compressedData;
    
    // 计算加密数据的CRC
    uint32_t crc = crcValidator.calculateCRC(encryptedData);
    spdlog::info("CRC calculated");

    // 将加密数据和CRC写入备份文件
    fileManager.writeBackupFile(backupPath, encryptedData, crc);
    spdlog::info("Backup file written");
        return Result::Success;
    } catch (const std::exception& e) {
        return Result::Failure;
    }
}

Result BackupManager::performRestore(const std::string& backupFilePath, const std::string& targetPath) {
    // 读取备份文件
    std::string decompressedData;
    uint32_t storedCRC;
    std::tie(decompressedData, storedCRC) = fileManager.readBackupFile(backupFilePath);
    // 验证CRC
    if (crcValidator.calculateCRC(decompressedData) != storedCRC) {
        throw std::runtime_error("CRC validation failed");
    }
    spdlog::info("CRC validation passed");

    // 解压缩数据
    std::string unpackedData = compressor.decompress(decompressedData);
    spdlog::info("Decompressed complete");

    // 解包数据
    auto files = packer.unpack(unpackedData);
    spdlog::info("Unpacked complete");

    // 恢复文件到目标路径
    fileManager.restoreFiles(files, targetPath);
    spdlog::info("Restore complete");

    return Result::Success;
}

Result BackupManager::performRestore(const std::string& backupFilePath, const std::string& targetPath, std::string key) {
    // 读取备份文件
    std::string encryptedData;
    uint32_t storedCRC;
    std::tie(encryptedData, storedCRC) = fileManager.readBackupFile(backupFilePath);
    // 验证CRC
    if (crcValidator.calculateCRC(encryptedData) != storedCRC) {
        throw std::runtime_error("CRC validation failed");
    }
    spdlog::info("CRC validation passed");
    
    // 解密数据
    encryptor.setKey(key);
    size_t keyEndPos = encryptedData.find('\0', ENCRYPTION_MARKER.size());
    encryptedData = encryptedData.substr(keyEndPos + 1);
    std::string decompressedData = encryptor.decrypt(encryptedData);
    spdlog::info("Decrypted complete");

    // 解压缩数据
    std::string unpackedData = compressor.decompress(decompressedData);
    spdlog::info("Decompressed complete");

    // 解包数据
    auto files = packer.unpack(unpackedData);
    spdlog::info("Unpacked complete");

    // 恢复文件到目标路径
    fileManager.restoreFiles(files, targetPath);
    spdlog::info("Restore complete");

    return Result::Success;
}

Key BackupManager::isEncrypt(const std::string& backupFilePath) {
    // 读取备份文件
    std::string encryptedData;
    uint32_t storedCRC;
    std::tie(encryptedData, storedCRC) = fileManager.readBackupFile(backupFilePath);
    if (encryptedData.compare(0, ENCRYPTION_MARKER.size(), ENCRYPTION_MARKER) == 0) {
        // 提取密钥
        size_t keyEndPos = encryptedData.find('\0', ENCRYPTION_MARKER.size());
        std::string extractedKey = encryptedData.substr(ENCRYPTION_MARKER.size(), keyEndPos - ENCRYPTION_MARKER.size());
        return {extractedKey, true};
    } else {
        return {"", false};
    }
}

void BackupManager::setFilter(long long minSize, long long maxSize, std::time_t startTime, std::time_t endTime, std::vector<std::string> fileFilter, std::vector<std::string> directoryFilter){
    fileManager.setBackupFilter(minSize, maxSize, startTime, endTime, fileFilter, directoryFilter);
}
