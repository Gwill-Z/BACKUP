#include "widget.h"


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
    connect(changeBackupPath, &QPushButton::clicked, this, [=]() {
        ChangeConfig();
    });
    backupConfigPath = config.getBackupPath();
    backupPath->setText(QString::fromStdString(backupConfigPath));
    gbl3->addWidget(changeBackupPath);
    

//备份
    QHBoxLayout *dataClassSelectBoxL = new QHBoxLayout;
    QGroupBox *dataClassSelectBox = new QGroupBox;
    QLabel *dataclasslabel = new QLabel("备份类型:");
    dataClassSelectBoxL->addWidget(dataclasslabel);
    filebackup = new QCheckBox("文件");
    dirbackup = new QCheckBox("目录");
    filebackup->setChecked(true);
    filebackup->setDisabled(true);
    dataClassSelectBoxL->addWidget(filebackup);
    dataClassSelectBoxL->addWidget(dirbackup);
    dataClassSelectBox->setLayout(dataClassSelectBoxL);
    connect(filebackup, &QCheckBox::stateChanged, this, [=](int state) {
    if (state == Qt::Checked) {
        dirbackup->setChecked(false);
        filebackup->setDisabled(true);
        dirbackup->setDisabled(false);
        Refresh();
        }
    });
    connect(dirbackup, &QCheckBox::stateChanged, this, [=](int state) {
    if (state == Qt::Checked) {
        filebackup->setChecked(false);
        dirbackup->setDisabled(true);
        filebackup->setDisabled(false);
        Refresh();
        }
    });
    gbl1->addWidget(dataClassSelectBox);

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

// 过滤

    // QHBoxLayout *dataFilterBoxL = new QHBoxLayout;
    // QGroupBox *dataFilterBox = new QGroupBox;
    QGroupBox *filterWidget = new QGroupBox;
    QVBoxLayout *filterWidgetL = new QVBoxLayout;
    dataFilter = new QStackedWidget();
    
// 切换stack按钮
    pstime = new QPushButton("时间");
    pssize = new QPushButton("大小");
    psfile = new QPushButton("排除文件");
    psdir = new QPushButton("排除目录");
    pstime->setStyleSheet("background-color: gray");
    QButtonGroup *filterGroup = new QButtonGroup(this);
    filterGroup->addButton(pstime);
    filterGroup->addButton(pssize);
    filterGroup->addButton(psfile);
    filterGroup->addButton(psdir);
    QHBoxLayout *filterGroupL = new QHBoxLayout;
    filterGroupL->addWidget(pstime);
    filterGroupL->addWidget(pssize);
    filterGroupL->addWidget(psfile);
    filterGroupL->addWidget(psdir);
    filterGroup->setExclusive(true);
    QWidget *pscontainer = new QWidget(this);
    pscontainer->setLayout(filterGroupL);
    connect(pstime, &QPushButton::clicked, this, [=]() {
        FilterBoxChange(timeFilter);
    });
    connect(pssize, &QPushButton::clicked, this, [=]() {
        FilterBoxChange(sizeFilter);
    });
    connect(psfile, &QPushButton::clicked, this, [=]() {
        FilterBoxChange(fileFilter);
    });
    connect(psdir, &QPushButton::clicked, this, [=]() {
        FilterBoxChange(dirFilter);
    });
    filterWidgetL->addWidget(pscontainer);

// 时间
    timeFilter = new QGroupBox;
    QLabel *timefilterlabel = new QLabel("时间范围（可选）:");
    QFormLayout *timeFilterL = new QFormLayout(timeFilter);
    timeFilterL->addWidget(timefilterlabel);
    QLabel *startlabel = new QLabel("最早时间:");
    startTime = new QDateTimeEdit();
    startTime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    startTime->setCalendarPopup(true);
    QLabel *endlabel = new QLabel("最晚时间:");
    endTime = new QDateTimeEdit();
    endTime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    endTime->setDateTime(QDateTime::currentDateTime());
    endTime->setCalendarPopup(true);
    timeFilterL->addRow(startlabel, startTime);
    timeFilterL->addRow(endlabel, endTime);
    // 连接开始时间编辑框的dateTimeChanged信号
    QObject::connect(startTime, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime& startDateTime) {
        // 获取当前的结束时间
        QDateTime currentEndDateTime = endTime->dateTime();

        // 检查结束时间是否小于开始时间
        if (currentEndDateTime <= startDateTime) {
            // 如果结束时间小于或等于开始时间，则将结束时间设置为开始时间的下一秒
            QDateTime newEndDateTime = startDateTime.addSecs(1);
            endTime->setDateTime(newEndDateTime);
        }
    });
    // 连接结束时间编辑框的dateTimeChanged信号
    QObject::connect(endTime, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime& endDateTime) {
        // 获取当前的开始时间
        QDateTime currentStartDateTime = startTime->dateTime();

        // 检查结束时间是否小于开始时间
        if (endDateTime <= currentStartDateTime) {
            // 如果结束时间小于或等于开始时间，则将开始时间设置为结束时间的前一秒
            QDateTime newStartDateTime = endDateTime.addSecs(-1);
            startTime->setDateTime(newStartDateTime);
        }
    });
    dataFilter->addWidget(timeFilter);

// 文件大小
    sizeFilter = new QGroupBox;
    QLabel *sizefilterlabel = new QLabel("文件大小范围（可选）:");
    QFormLayout *sizeFilterL = new QFormLayout(sizeFilter);
    sizeFilterL->addWidget(sizefilterlabel);
    QLabel *minlabel = new QLabel("最小文件大小（Byte）:");
    minSize = new QSpinBox;
    minSize->setRange(0, std::numeric_limits<int>::max());
    minSize->setSingleStep(10);
    QLabel *maxlabel = new QLabel("最大文件大小（Byte）:");
    maxSize = new QSpinBox;
    maxSize->setRange(0, std::numeric_limits<int>::max());
    maxSize->setValue(std::numeric_limits<int>::max());
    maxSize->setSingleStep(10);
    sizeFilterL->addRow(minlabel, minSize);
    sizeFilterL->addRow(maxlabel, maxSize);
    connect(minSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &Widget::UpdateSize);
    connect(maxSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &Widget::UpdateSize);
    dataFilter->addWidget(sizeFilter);
// 排除文件
    // QHBoxLayout *sysFilterBoxL = new QHBoxLayout;
    fileFilter = new QGroupBox;

    // 创建一个 QTableWidget，并设置列数和表头
    fileTable = new QTableWidget;
    fileTable->setColumnCount(2);
    fileTable->setHorizontalHeaderLabels(QStringList() << "文件名" << "选择");
    fileTable->resizeColumnToContents(0);

    // 创建一个垂直布局，并将表格添加进去
    QVBoxLayout* fileFilterL = new QVBoxLayout;
    fileFilterL->addWidget(fileTable);
    fileFilter->setLayout(fileFilterL);
    dataFilter->addWidget(fileFilter);

//排除目录
    dirFilter = new QGroupBox;

    // 创建一个 QTableWidget，并设置列数和表头
    dirTable = new QTableWidget;
    dirTable->setColumnCount(2);
    dirTable->setHorizontalHeaderLabels(QStringList() << "目录名" << "选择");
    dirTable->resizeColumnToContents(0);

    // 创建一个垂直布局，并将表格添加进去
    QVBoxLayout* dirFilterL = new QVBoxLayout;
    dirFilterL->addWidget(dirTable);
    dirFilter->setLayout(dirFilterL);
    dataFilter->addWidget(dirFilter);

    filterWidgetL->addWidget(dataFilter);
    filterWidget->setLayout(filterWidgetL);
    gbl1->addWidget(filterWidget);
    

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

    // QLabel *backuppathlabel = new QLabel("待还原文件:");
    // backupPathSelectBoxL->addWidget(backuppathlabel);
    // backupedPath = new QLineEdit;
    // backupedPath->setReadOnly(true);
    // backupPathSelectBoxL->addWidget(backupedPath);
    // selectBackupedPath = new QPushButton("选择");
    // connect(selectBackupedPath, &QPushButton::clicked, this, [=]() {
    //     SelectBackupedFile(backupedPath);
    // });
    // backupPathSelectBoxL->addWidget(selectBackupedPath);

    // 创建一个 QStandardItemModel 对象
    restoreTable = new QTableView();
    
    model = new QStandardItemModel(this);
    model->setColumnCount(3);

    // 设置列标题
    model->setHeaderData(0, Qt::Horizontal, "文件名");
    model->setHeaderData(1, Qt::Horizontal, "大小");
    model->setHeaderData(2, Qt::Horizontal, "最后修改时间");

    // 获取目录中特定后缀的文件列表
    QStringList filters;
    filters << "*.zth";
    restoreSrcDir.setPath(QString::fromStdString(backupConfigPath));
    QFileInfoList restoreList = restoreSrcDir.entryInfoList(filters, QDir::Files);

    // 将文件列表添加到表格模型中
    foreach (const QFileInfo& fileInfo, restoreList) {
        QString fileName = fileInfo.fileName();
        QString fileSize = QString::number(fileInfo.size()) + "bytes";
        QString fileModified = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");

        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(fileName);
        rowItems << new QStandardItem(fileSize);
        rowItems << new QStandardItem(fileModified);
        model->appendRow(rowItems);
    }

    // 设置表格模型
    restoreTable->setModel(model);
    restoreTable->resizeRowsToContents();
    restoreTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //连接
    connect(restoreTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection& selected, const QItemSelection& deselected) {
        Q_UNUSED(deselected);

        selectedIndexes = restoreTable->selectionModel()->selectedIndexes();
        if (!selectedIndexes.isEmpty()) {
            QSet<int> selectedRows;  // 用于存储选中行的唯一行号

            // 提取选中行的唯一行号
            for (const QModelIndex& index : selectedIndexes) {
                selectedRows.insert(index.row());
            }

            // 遍历选中行
            for (int row : selectedRows) {
                QString selectedFileName = model->index(row, 0).data().toString();
                QString selectedFilePath = restoreSrcDir.absoluteFilePath(selectedFileName);
                qDebug() << "Selected File: " << selectedFilePath;
            }
        }
    });

    backupPathSelectBoxL->addWidget(restoreTable);

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

    QHBoxLayout *restoreFuncL = new QHBoxLayout;
    QGroupBox *restoreFunc = new QGroupBox;
    restore = new QPushButton("还原");
    connect(restore, &QPushButton::clicked, this, [=]() {
        Frestore();
    });
    remove = new QPushButton("删除");
    connect(remove, &QPushButton::clicked, this, [=]() {
        Fremove();
    });
    restoreFuncL->addWidget(restore);
    restoreFuncL->addWidget(remove);
    restoreFunc->setLayout(restoreFuncL);
    gbl2->addWidget(restoreFunc);
    

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
        backupPath->setText(QString::fromStdString(backupConfigPath));
    }
}

void Widget::FilterBoxChange(QWidget* filter){
    dataFilter->setCurrentWidget(filter);
    pstime->setStyleSheet("");
    pssize->setStyleSheet("");
    psfile->setStyleSheet("");
    psdir->setStyleSheet("");

        // 根据当前显示的GroupBox设置对应按钮的样式表
    if (filter == timeFilter) {
        pstime->setStyleSheet("background-color: gray");
    } else if (filter == sizeFilter) {
        pssize->setStyleSheet("background-color: gray");
    } else if (filter == fileFilter) {
        psfile->setStyleSheet("background-color: gray");
    } else if (filter == dirFilter) {
        psdir->setStyleSheet("background-color: gray");
    }
}

void Widget::BackupPathChange(QLineEdit* le){
    QString path = QFileDialog::getExistingDirectory(
                nullptr, "选择目录", QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (!path.isEmpty()) {
                le->setText(path);
            }
}

void Widget::PasswordInputState(int state){
    password->setDisabled(state != Qt::Checked);
}

void Widget::DataPathChange(QLineEdit* le) {
    QString path;
    if(filebackup->isChecked()){
        path = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath());
    } else if(dirbackup->isChecked()){
        path = QFileDialog::getExistingDirectory(
            nullptr, "选择目录", QDir::homePath(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        
    } else{
        QMessageBox::information(this, "提示", "系统出错");
    }
    
    if (!path.isEmpty()) {
        le->setText(path);
        if(dirbackup->isChecked()){
            RefreshFileTable(path);
            RefreshDirTable(path);
        }
    }
}

void Widget::SelectBackupedFile(QLineEdit* le){
    QString filename = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath(), tr("已备份文件 (*.zth)"));
    if (!filename.isEmpty()) {
        le->setText(filename);
    }
}

void Widget::ChangeConfig(){
    if(backupPath->text().isEmpty()){
        QMessageBox::information(this, "提示", "请选择路径");
        return;
    } 
    config.setBackupPath(backupPath->text().toStdString());
    config.saveConfig("../backup_config.txt");
    backupConfigPath = backupPath->text().toStdString();
    qDebug() << backupPath->text();
    QMessageBox::information(this, "提示", "修改成功");
    RefreshTable();
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
    long long sminSize = minSize->value();
    long long smaxSize = maxSize->value();
    std::time_t sstartTime = startTime->dateTime().toSecsSinceEpoch();
    std::time_t sendTime = endTime->dateTime().toSecsSinceEpoch();
    std::vector<std::string> fileV;
    for (QSet<int>::const_iterator it = fileIndex.constBegin(); it != fileIndex.constEnd(); ++it) {
        int value = *it;
        QTableWidgetItem* item1 = fileTable->item(value, 0);
        if(item1){
            QString path = item1->text();
            fileV.push_back(path.toStdString());
        }
    }
    std::vector<std::string> dirV;
    for (QSet<int>::const_iterator it = dirIndex.constBegin(); it != dirIndex.constEnd(); ++it) {
        int value = *it;
        QTableWidgetItem* item1 = dirTable->item(value, 0);
        if(item1){
            QString path = item1->text();
            dirV.push_back(path.toStdString());
            qDebug() << path;
        }
    }
    backupmanager->setFilter(sminSize, smaxSize, sstartTime, sendTime, fileV, dirV);
    if(encrypt->isChecked()){
        try{
            backupmanager->performBackup(password->text().toStdString());
            QMessageBox::information(this, "提示", "备份成功");
            Refresh();
        } catch(const std::exception& e){
            QMessageBox::information(this, "提示", "备份失败");
        }
    }else{
        try{
            backupmanager->performBackup();
            QMessageBox::information(this, "提示", "备份成功");
            Refresh();
        } catch(const std::exception& e){
            QMessageBox::information(this, "提示", "备份失败");
        }
    }
}

void Widget::Frestore(){
    if(selectedIndexes.isEmpty()){
        QMessageBox::information(this, "提示", "请选择待还原文件");
        return;
    }
    if(restorePath->text().isEmpty()){
        QMessageBox::information(this, "提示", "请选择还原路径");
        return;
    }
    int success = 0;
    int failed = 0;
    QSet<int> selectedRows;  // 用于存储选中行的唯一行号

        // 提取选中行的唯一行号
    for (const QModelIndex& index : selectedIndexes) {
        selectedRows.insert(index.row());
    }
    for (int row : selectedRows) {
        QString selectedFileName = model->index(row, 0).data().toString();
        QString selectedFilePath = restoreSrcDir.absoluteFilePath(selectedFileName);
        
        if(backupmanager->isEncrypt(selectedFilePath.toStdString()).isEncrypt){
        bool ok;
        QString Qpsw = QInputDialog::getText(nullptr, "输入密码", selectedFileName + "已加密，请输入密码:", QLineEdit::Password, "", &ok);
        std::string psw = Qpsw.toStdString();
        std::string psw_bak = backupmanager->isEncrypt(selectedFilePath.toStdString()).key;
        if(!ok){
            failed ++;
            continue;
        } else if(psw.length() == 0){
            QMessageBox::information(this, "提示", selectedFileName + "密码不能为空");
            failed ++;
            continue;
        } else if(psw != psw_bak){
            QMessageBox::information(this, "提示", selectedFileName + "密码错误");
            failed ++;
            continue;
        }
        else{
            try{
                backupmanager->performRestore(selectedFilePath.toStdString(), restorePath->text().toStdString(), psw);
                success ++;
                //QMessageBox::information(this, "提示", selectedFileName + "还原成功");
            } catch(const std::exception& e){
                failed ++;
                QMessageBox::information(this, "提示", selectedFileName + "还原失败");
            }
        }
    } else{
        try{
            backupmanager->performRestore(selectedFilePath.toStdString(), restorePath->text().toStdString());
            success ++;
            //QMessageBox::information(this, "提示", selectedFileName + "还原成功");
        } catch(const std::exception& e){
            failed ++;
            QMessageBox::information(this, "提示", selectedFileName + "还原失败");
        }
    }
    }
    std::ostringstream oss;
    oss << "共处理" << selectedRows.size() << "项，其中还原成功" << success << "项，还原失败" << failed << "项";
    //std::string restoreinfo = "共处理" + selectedIndexes.size() + "项，其中还原成功" + success + "项，还原失败" + failed + "项";
    std::string restoreinfo = oss.str();
    QString qrestoreinfo = QString::fromStdString(restoreinfo);
    QMessageBox::information(this, "提示", qrestoreinfo);
    Refresh();
}


void Widget::Fremove(){
    if(selectedIndexes.isEmpty()){
        QMessageBox::information(this, "提示", "请选择可还原文件");
        return;
    }
    qDebug() << "1";
    QMessageBox::StandardButton reply = QMessageBox::question(this, "删除文件", "确定要删除选中的文件吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        QSet<int> selectedRows;  // 用于存储选中行的唯一行号

        // 提取选中行的唯一行号
        for (const QModelIndex& index : selectedIndexes) {
            selectedRows.insert(index.row());
        }
        for (int row : selectedRows) {
            QString selectedFileName = model->index(row, 0).data().toString();
            QString selectedFilePath = restoreSrcDir.absoluteFilePath(selectedFileName);
            QFile rfile(selectedFilePath);
            if (rfile.remove()) {
                    qDebug() << "File removed successfully: " << selectedFilePath;
            } else {
                qDebug() << "Failed to remove file: " << selectedFilePath;
            }
        }
    }
    RefreshTable();
        
}


void Widget::UpdateSize(){
    if (minSize->value() >= maxSize->value())
    {
        // 更新 minSize 和 maxSize 的值
        minSize->setValue(maxSize->value() - minSize->singleStep());
    }
}

void Widget::UpdateFileIndex(int state){
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    int index = fileTable->indexAt(checkBox->parentWidget()->pos()).row();

    if (state == Qt::Checked) {
        fileIndex.insert(index);
    } else {
        fileIndex.remove(index);
    }
}

void Widget::UpdateDirIndex(int state){
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    int index = fileTable->indexAt(checkBox->parentWidget()->pos()).row();

    if (state == Qt::Checked) {
        dirIndex.insert(index);
    } else {
        dirIndex.remove(index);
    }
}

// 刷新
void Widget::RefreshTable()
{
    // 清除旧数据
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(restoreTable->model());
    if (model)
    {
        model->removeRows(0, model->rowCount());
    }

    // 获取目录中特定后缀的文件列表
    QStringList filters;
    filters << "*.zth";
    restoreSrcDir.setPath(QString::fromStdString(backupConfigPath));
    QFileInfoList fileList = restoreSrcDir.entryInfoList(filters, QDir::Files);

    // 将文件列表添加到表格模型中
    foreach (const QFileInfo& fileInfo, fileList) {
        QString fileName = fileInfo.fileName();
        QString fileSize = QString::number(fileInfo.size()) + " bytes";
        QString fileModified = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");

        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(fileName);
        rowItems << new QStandardItem(fileSize);
        rowItems << new QStandardItem(fileModified);
        model->appendRow(rowItems);
    }

    // 设置表格
    restoreTable->resizeColumnsToContents();
    restoreTable->resizeRowsToContents();
}

void Widget::Refresh(){
    RefreshTable();
    restorePath->setText("");
    nameInput->setText("");
    dataPath->setText("");
    password->setText("");
}

void Widget::RefreshFileTable(QString path){
    // 指定的目录路径
    QString directoryPath = path;
    // 遍历指定目录和子目录，获取所有文件
    QFileInfoList fileList;
    QDirIterator it(directoryPath, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo = it.fileInfo();
        if (fileInfo.isFile()) {
            fileList.append(fileInfo);
        }
    }
    fileTable->clearContents();
    fileTable->setRowCount(0);
    // 在表格中插入行，并设置文件名和复选框
    for (int i = 0; i < fileList.size(); ++i) {
        const QFileInfo& fileInfo = fileList.at(i);
        QString filePath = fileInfo.filePath();

        fileTable->insertRow(i);
        fileTable->setItem(i, 0, new QTableWidgetItem(filePath));

        QWidget* checkboxContainer = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(checkboxContainer);
        layout->setAlignment(Qt::AlignCenter);
        QCheckBox* checkBox = new QCheckBox;
        layout->addWidget(checkBox);
        layout->setContentsMargins(0, 0, 0, 0);
        fileTable->setCellWidget(i, 1, checkboxContainer);
         // 连接复选框的状态变化信号槽
        connect(checkBox, &QCheckBox::stateChanged, this, &Widget::UpdateFileIndex);
    }
    fileTable->resizeColumnToContents(0);
}

void Widget::RefreshDirTable(QString path) {
    // 指定的目录路径
    QString directoryPath = path;
    // 遍历指定目录，获取所有子目录
    QStringList subdirectoryList;
    QDirIterator it(directoryPath, QDir::Dirs | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        it.next();
        QString subdirectoryPath = it.filePath();
        subdirectoryList.append(subdirectoryPath);
    }
    dirTable->clearContents();
    dirTable->setRowCount(0);
    // 在表格中插入行，并设置子目录路径和复选框
    for (int i = 0; i < subdirectoryList.size(); ++i) {
        const QString& subdirectoryPath = subdirectoryList.at(i);

        dirTable->insertRow(i);
        dirTable->setItem(i, 0, new QTableWidgetItem(subdirectoryPath));

        QWidget* checkboxContainer = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(checkboxContainer);
        layout->setAlignment(Qt::AlignCenter);
        QCheckBox* checkBox = new QCheckBox;
        layout->addWidget(checkBox);
        layout->setContentsMargins(0, 0, 0, 0);
        dirTable->setCellWidget(i, 1, checkboxContainer);
         // 连接复选框的状态变化信号槽
        connect(checkBox, &QCheckBox::stateChanged, this, &Widget::UpdateDirIndex);
    }
    dirTable->resizeColumnToContents(0);
}