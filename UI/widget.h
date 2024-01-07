#ifndef WIDGET_H
#define WIDGET_H

#include "BackupConfig.h"
#include "BackupManager.h"
#include <QWidget>
#include <QGroupBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileInfoList>
#include <QDir>
#include <QDirIterator>
#include <QFormLayout>
#include <QFile>
#include <QDebug>
#include <string>
#include <iostream>
#include <sstream>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private:
        QStackedWidget *sw;
        QGroupBox *modemenu;
        QGroupBox *gb1, *gb2, *gb3;
        QPushButton *ps1, *ps2, *ps3;
        QLineEdit *backupPath, *dataPath, *password, *backupedPath, *restorePath, *nameInput;
        QDateTimeEdit *startTime, *endTime;
        QSpinBox *maxSize, *minSize;
        QTableWidget *fileTable, *dirTable;
        QPushButton *selectBackupPath, *selectDataPath, *backup, *selectBackupedPath,
             *selectRestorePath, *restore, *changeBackupPath, *remove,
             *addfile, *remfile;
        QCheckBox *encrypt, *filebackup, *dirbackup;
        QTableView *restoreTable;
        QDir restoreSrcDir;
        QModelIndexList selectedIndexes;
        QStandardItemModel* model;
        QSet<QString> selectedFileSet;
        std::string backupConfigPath;  //设置备份目标路径
        BackupManager *backupmanager;

        void RefreshTable();
        void Refresh();
        void RefreshFileTable(QString path);
private slots:
        void GroupBoxChange(QWidget* gb);
        void BackupPathChange(QLineEdit* le);
        void DataPathChange(QLineEdit* le);
        void PasswordInputState(int state);
        void SelectBackupedFile(QLineEdit* le);
	    void ChangeConfig();
        void Fbackup();
        void Frestore();
        void Fremove();
        void UpdateSize();
        // void FaddFile();
        // void FremFile();
};
#endif // WIDGET_H
