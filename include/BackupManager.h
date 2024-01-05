#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

#include <string>
#include "FileManager.h"
#include "Packer.h"
#include "Compressor.h"
#include "Encryptor.h"
#include "CRCValidator.h"
#include "Result.h"

struct Key {
    std::string key;
    bool isEncrypt;
};

class BackupManager {
private:
    std::string sourcePath;
    std::string backupPath;
    FileManager fileManager;
    Packer packer;
    Compressor compressor;
    Encryptor encryptor;
    CRCValidator crcValidator;

public:
    BackupManager();
    void setSourcePath(const std::string& path);
    void setBackupPath(const std::string& path);
    Result performBackup();
    Result performBackup(std::string key);
    Result performRestore(const std::string& backupFilePath, const std::string& targetPath);
    Result performRestore(const std::string& backupFilePath, const std::string& targetPath, std::string key);
    Key isEncrypt(const std::string& backupFilePath);
};

#endif // BACKUP_MANAGER_H
