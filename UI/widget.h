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
#include <QButtonGroup>
#include <QFile>
#include <string>
#include <sstream>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private:
        QStackedWidget *sw, *dataFilter;
        QGroupBox *modemenu;
        QGroupBox *gb1, *gb2, *gb3, *timeFilter, *sizeFilter, *fileFilter, *dirFilter;
        QPushButton *ps1, *ps2, *ps3;
        QLineEdit *backupPath, *dataPath, *password, *backupedPath, *restorePath, *nameInput;
        QDateTimeEdit *startTime, *endTime;
        QSpinBox *maxSize, *minSize;
        QTableWidget *fileTable, *dirTable;
        QPushButton *selectBackupPath, *selectDataPath, *backup, *selectBackupedPath,
             *selectRestorePath, *restore, *changeBackupPath, *remove, *pstime, *pssize, *psfile, *psdir,
             *addfile, *remfile;
        QCheckBox *encrypt, *filebackup, *dirbackup;
        QTableView *restoreTable;
        QDir restoreSrcDir;
        QSet<int> fileIndex, dirIndex;
        QModelIndexList selectedIndexes;
        QStandardItemModel* model;
        QSet<QString> selectedFileSet;
        std::string backupConfigPath;  //设置备份目标路径
        BackupManager *backupmanager;

        void RefreshTable();
        void Refresh();
        void RefreshFileTable(QString path);
        void RefreshDirTable(QString path);
private slots:
        void GroupBoxChange(QWidget* gb);
        void FilterBoxChange(QWidget* filter);
        void BackupPathChange(QLineEdit* le);
        void DataPathChange(QLineEdit* le);
        void PasswordInputState(int state);
        void SelectBackupedFile(QLineEdit* le);
	    void ChangeConfig();
        void Fbackup();
        void Frestore();
        void Fremove();
        void UpdateSize();
        void UpdateFileIndex(int state);
        void UpdateDirIndex(int state);
        // void FaddFile();
        // void FremFile();
};
#endif // WIDGET_H
