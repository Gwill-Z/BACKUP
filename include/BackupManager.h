#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

#include <string>
#include "FileManager.h"
#include "Packer.h"
#include "Compressor.h"
#include "Encryptor.h"
#include "CRCValidator.h"
#include "Result.h"

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
    Result performBackup(std::string encryptChoice, std::string key);
    Result performRestore(const std::string& backupFilePath, const std::string& targetPath);
};

#endif // BACKUP_MANAGER_H
