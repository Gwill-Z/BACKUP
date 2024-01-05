#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>

BackupConfig& config = BackupConfig::getInstance();


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    config.loadConfig("../backup_config.txt");
    this->resize(800, 600);
    sw = new QStackedWidget;

    gb1 = new QGroupBox;
    gb2 = new QGroupBox;
    gb3 = new QGroupBox;

    QVBoxLayout *gbl1 = new QVBoxLayout;
    QVBoxLayout *gbl2 = new QVBoxLayout;
    QHBoxLayout *gbl3 = new QHBoxLayout;


// 备份设置
    QLabel *settingpathlabel = new QLabel("备份数据路径:");
    gbl3->addWidget(settingpathlabel);
    backupPath = new QLineEdit;
    backupPath->setReadOnly(true);
    gbl3->addWidget(backupPath);
    selectBackupPath = new QPushButton("选择");
    connect(selectBackupPath, &QPushButton::clicked, this, [=]() {
        BackupPathChange(backupPath);
    });
    gbl3->addWidget(selectBackupPath);
    changeBackupPath = new QPushButton("确定");
    gbl3->addWidget(changeBackupPath);
    connect(changeBackupPath, &QPushButton::clicked, this, [=]() {
        ChangeConfig();
    });

//备份
    QHBoxLayout *dataPathSelectBoxL = new QHBoxLayout;
    QGroupBox *dataPathSelectBox = new QGroupBox;
    QLabel *datapathlabel = new QLabel("待备份数据路径:");
    dataPathSelectBoxL->addWidget(datapathlabel);
    dataPath = new QLineEdit;
    dataPath->setReadOnly(true);
    dataPathSelectBoxL->addWidget(dataPath);
    selectDataPath = new QPushButton("选择");
    connect(selectDataPath, &QPushButton::clicked, this, [=]() {
        DataPathChange(dataPath);
    });
    dataPathSelectBoxL->addWidget(selectDataPath);
    dataPathSelectBox->setLayout(dataPathSelectBoxL);
    gbl1->addWidget(dataPathSelectBox);

    QHBoxLayout *nameInputBoxL = new QHBoxLayout;
    QGroupBox *nameInputBox = new QGroupBox;
    QLabel *nameInputlabel = new QLabel("备份文件名称:");
    nameInputBoxL->addWidget(nameInputlabel);
    nameInput = new QLineEdit;
    nameInputBoxL->addWidget(nameInput);
    nameInputBox->setLayout(nameInputBoxL);
    gbl1->addWidget(nameInputBox);

    QHBoxLayout *selectEncryptL = new QHBoxLayout;
    QGroupBox *selectEncryptBox = new QGroupBox;
    QLabel *passwordlabel = new QLabel("密码:");
    encrypt = new QCheckBox("加密");
    password = new QLineEdit;
    connect(encrypt, &QCheckBox::stateChanged, this, &Widget::PasswordInputState);
    password->setDisabled(true);
    selectEncryptL->addWidget(encrypt);
    selectEncryptL->addWidget(passwordlabel);
    selectEncryptL->addWidget(password);
    selectEncryptBox->setLayout(selectEncryptL);
    gbl1->addWidget(selectEncryptBox);

    backup = new QPushButton("备份");
    gbl1->addWidget(backup);
    backupmanager = new BackupManager;
    connect(backup, &QPushButton::clicked, this, [=]() {
        Fbackup();
    });

//还原
    QHBoxLayout *backupPathSelectBoxL = new QHBoxLayout;
    QGroupBox *backupPathSelectBox = new QGroupBox;
    QLabel *backuppathlabel = new QLabel("待还原文件:");
    backupPathSelectBoxL->addWidget(backuppathlabel);
    backupedPath = new QLineEdit;
    backupedPath->setReadOnly(true);
    backupPathSelectBoxL->addWidget(backupedPath);
    selectBackupedPath = new QPushButton("选择");
    connect(selectBackupedPath, &QPushButton::clicked, this, [=]() {
        SelectBackupedFile(backupedPath);
    });
    backupPathSelectBoxL->addWidget(selectBackupedPath);
    backupPathSelectBox->setLayout(backupPathSelectBoxL);
    gbl2->addWidget(backupPathSelectBox);

    QHBoxLayout *restorePathSelectBoxL = new QHBoxLayout;
    QGroupBox *restorePathSelectBox = new QGroupBox;
    QLabel *restorepathlabel = new QLabel("还原路径:");
    restorePathSelectBoxL->addWidget(restorepathlabel);
    restorePath = new QLineEdit;
    restorePath->setReadOnly(true);
    restorePathSelectBoxL->addWidget(restorePath);
    selectRestorePath = new QPushButton("选择");
    connect(selectRestorePath, &QPushButton::clicked, this, [=]() {
        BackupPathChange(restorePath);
    });
    restorePathSelectBoxL->addWidget(selectRestorePath);
    restorePathSelectBox->setLayout(restorePathSelectBoxL);
    gbl2->addWidget(restorePathSelectBox);

    restore = new QPushButton("还原");
    connect(restore, &QPushButton::clicked, this, [=]() {
        Frestore();
    });
    gbl2->addWidget(restore);
    

//主界面
    gb1->setLayout(gbl1);
    gb2->setLayout(gbl2);
    gb3->setLayout(gbl3);

    sw->addWidget(gb1);
    sw->addWidget(gb2);
    sw->addWidget(gb3);

    QHBoxLayout *modebox = new QHBoxLayout;
    ps1 = new QPushButton("备份");
    ps2 = new QPushButton("还原");
    ps3 = new QPushButton("备份设置");
    ps1->setStyleSheet("background-color: gray");
    modebox->addWidget(ps1);
    modebox->addWidget(ps2);
    modebox->addWidget(ps3);
    modemenu = new QGroupBox;
    modemenu->setLayout(modebox);

    QVBoxLayout *mainbox = new QVBoxLayout;
    mainbox->addWidget(modemenu);
    mainbox->addWidget(sw);
    this->setLayout(mainbox);

    connect(ps1, &QPushButton::clicked, this, [=]() {
        GroupBoxChange(gb1);
    });
    connect(ps2, &QPushButton::clicked, this, [=]() {
        GroupBoxChange(gb2);
    });
    connect(ps3, &QPushButton::clicked, this, [=]() {
        GroupBoxChange(gb3);
    });

}

Widget::~Widget()
{
}

void Widget::GroupBoxChange(QWidget* gb){
    sw->setCurrentWidget(gb);

    ps1->setStyleSheet("");
    ps2->setStyleSheet("");
    ps3->setStyleSheet("");

        // 根据当前显示的GroupBox设置对应按钮的样式表
    if (gb == gb1) {
        ps1->setStyleSheet("background-color: gray");
    } else if (gb == gb2) {
        ps2->setStyleSheet("background-color: gray");
    } else if (gb == gb3) {
        ps3->setStyleSheet("background-color: gray");
    }
}

void Widget::BackupPathChange(QLineEdit* le){
    QString path = QFileDialog::getExistingDirectory(
                nullptr, "选择目录", QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (!path.isEmpty()) {
                le->setText(path);
                backupConfigPath = path.toStdString();
            }
}

void Widget::PasswordInputState(int state){
    password->setDisabled(state != Qt::Checked);
}

void Widget::DataPathChange(QLineEdit* le){
    QString path = QFileDialog::getExistingDirectory(
                nullptr, "选择目录", QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (!path.isEmpty()) {
                le->setText(path);
            }
}

void Widget::SelectBackupedFile(QLineEdit* le){
    QString filename = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath(), tr("已备份文件 (*.zth)"));
    if (!filename.isEmpty()) {
        le->setText(filename);
    }
}

void Widget::ChangeConfig(){
    if(backupConfigPath.length() == 0){
        QMessageBox::information(this, "提示", "请选择路径");\
        return;
    }
    config.setBackupPath(backupConfigPath);
    config.saveConfig("../backup_config.txt");
    QMessageBox::information(this, "提示", "修改成功");
}

void Widget::Fbackup(){
    if(dataPath->text().isEmpty()){
        QMessageBox::information(this, "提示", "请选择待备份数据路径");
        return;
    }
    if(nameInput->text().isEmpty()){
        QMessageBox::information(this, "提示", "请输入名称");
        return;
    }
    if(encrypt->isChecked() && password->text().isEmpty()){
        QMessageBox::information(this, "提示", "请输入密码");
        return;
    }
    backupmanager->setSourcePath(dataPath->text().toStdString());
    backupmanager->setBackupPath(config.getBackupPath() + "/" + nameInput->text().toStdString() + ".zth");
    if(encrypt->isChecked()){
        try{
            backupmanager->performBackup(password->text().toStdString());
            QMessageBox::information(this, "提示", "备份成功");
        } catch(const std::exception& e){
            QMessageBox::information(this, "提示", "备份失败");
        }
    }else{
        try{
            backupmanager->performBackup();
            QMessageBox::information(this, "提示", "备份成功");
        } catch(const std::exception& e){
            QMessageBox::information(this, "提示", "备份失败");
        }
    }
}

void Widget::Frestore(){
    if(backupedPath->text().isEmpty()){
        QMessageBox::information(this, "提示", "请选择待还原文件");
        return;
    }
    if(restorePath->text().isEmpty()){
        QMessageBox::information(this, "提示", "请选择还原路径");
        return;
    }
    if(backupmanager->isEncrypt(backupedPath->text().toStdString()).isEncrypt){
        bool ok;
        QString Qpsw = QInputDialog::getText(nullptr, "输入密码", "请输入密码:", QLineEdit::Password, "", &ok);
        std::string psw = Qpsw.toStdString();
        std::string psw_bak = backupmanager->isEncrypt(backupedPath->text().toStdString()).key;
        if(psw != psw_bak){
            QMessageBox::information(this, "提示", "密码错误");
            return;
        }
        if(!ok){
            return;
        } else if(psw.length() == 0){
            QMessageBox::information(this, "提示", "密码不能为空");
            return;
        } 
        else{
            try{
                backupmanager->performRestore(backupedPath->text().toStdString(), restorePath->text().toStdString(), psw);
                QMessageBox::information(this, "提示", "还原成功");
            } catch(const std::exception& e){
                QMessageBox::information(this, "提示", "还原失败");
            }
        }
    } else{
        try{
            backupmanager->performRestore(backupedPath->text().toStdString(), restorePath->text().toStdString());
            QMessageBox::information(this, "提示", "还原成功");
        } catch(const std::exception& e){
            QMessageBox::information(this, "提示", "还原失败");
        }
    }
    
}