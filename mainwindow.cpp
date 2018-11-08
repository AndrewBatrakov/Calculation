#include "mainwindow.h"
#include "logistikaclass.h"
#include "clientform.h"
#include <QtSql>
#include "organization.h"
#include"boolitemdelegate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);

    readSettings();
    createActions();
    createMenus();
    createPanel();

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(panelWidget);
    splitter->setStretchFactor(1,1);
    setCentralWidget(splitter);

    QString programmName = trUtf8("Калькуляции v.");
    QSettings iniSettings("CalculationEmployeeClient.ini",QSettings::IniFormat);
    QString version = iniSettings.value("Version").toString();
    programmName += version;
    setWindowTitle(programmName);

    QPixmap pixWater(":/client.png");
    setWindowIcon(pixWater);
    viewOrganization();

    fileExchange.setFileName("Message.txt");
    fileExchange.open(QIODevice::ReadWrite);
}

MainWindow::~MainWindow()
{

}

void MainWindow::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    restoreGeometry(settings.value("MainWindow").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    settings.setValue("MainWindow", saveGeometry());
}

void MainWindow::createActions()
{
    //File Action
    exitAction = new QAction(tr("Выход..."),this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    //Reference Action
    clientAction = new QAction(tr("Клиенты"),this);
    connect(clientAction,SIGNAL(triggered()),this,SLOT(viewClient()));
    organizationAction = new QAction(tr("Организации"),this);
    connect(organizationAction,SIGNAL(triggered()),this,SLOT(viewOrganization()));

    //Documents Action
    logistikaAction = new QAction(tr("Калькуляция логистики"));
    connect(logistikaAction,SIGNAL(triggered()),this,SLOT(viewLogistika()));

    //Help Action
    aboutQtAction = new QAction(trUtf8("О Qt..."),this);
    connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    aboutProgAction = new QAction(trUtf8("О программе..."),this);
    connect(aboutProgAction,SIGNAL(triggered()),this,SLOT(aboutProgProcedure()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(trUtf8("Файл"));
    fileMenu->addAction(exitAction);

    menuBar()->addSeparator();
    referenceMenu = menuBar()->addMenu(trUtf8("Справочники"));
    referenceMenu->addAction(clientAction);
    referenceMenu->addAction(organizationAction);

    menuBar()->addSeparator();
    calculationMenu = menuBar()->addMenu(trUtf8("Калькуляции"));
    calculationMenu->addAction(logistikaAction);

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(trUtf8("Помощь"));
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(aboutProgAction);
}

void MainWindow::aboutProgProcedure()
{
    QSettings iniSettings("EmployeeClient.ini",QSettings::IniFormat);
    QString version = iniSettings.value("Version").toString();
    //    int fontId = iniSettings.value("Font").toInt();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(trUtf8("Калькуляции"));
    QPixmap pixWater(":/client.png");
    msgBox.setIconPixmap(pixWater);
    QString textMessage = trUtf8("<p style=\"color:darkblue\"; style=\"font-family:Times New Roman\"; "
                                 "style=\"font-size :20pt\">"
                                 "Калькуляции "
                                 "<br/> (SQLite version) ");
    textMessage += version;
    textMessage += trUtf8("</p>"
                          "<p style=\"color:darkgreen\"; style=\"font-family:Times New Roman\"; "
                          "style=\"font-size :12pt\">Copyright 2011-18 A+O Batrakov Inc.</p>"
                          "<p style=\"font-size :12pt\">"
                          "Программа предназначена для расчета калькуляций на предприятии.</p>"
                          "<p style=\"font-family:Times New Roman\"; style=\"font-size :10pt\">"
                          "По вопросам улучшения или изменения (расширения) функционала программы, пишите на почту: "
                          "<a href=\"www.mail.ru\">hothit@list.ru</a>;</p>");
    msgBox.setText(textMessage);
    //msgBox.setFont(QFont(QFontDatabase::applicationFontFamilies(fontId).first()));
    QPushButton *yesButton = msgBox.addButton(QMessageBox::Yes);
    yesButton->setText(tr("Да"));
    msgBox.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(trUtf8("Калькуляции"));
    msgBox.setText(trUtf8("Закрыть программу?"));
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *yesButton = msgBox.addButton(QMessageBox::Yes);
    yesButton->setText(tr("Да"));

    QPushButton *noButton = msgBox.addButton(QMessageBox::No);
    noButton->setText(tr("Отмена"));
    msgBox.setDefaultButton(noButton);

    noButton->setStyleSheet("QPushButton:hover {color: red}");
    int r = msgBox.exec();
    if(r == QMessageBox::Yes){
        MainWindow::hide();
        writeSettings();
        event->accept();

        qApp->quit();
        //removeFiles();
    }else if(r == QMessageBox::No){
        event->ignore();
    }
}

void MainWindow::createPanel()
{
    panelWidget = new QWidget(this);

    tableView = new QTableView(this);
    tableView->setStyleSheet("QTableView::item {selection-color: black; selection-background-color: rgba(30, 144, 255, 30%);}");
    templateModel = new QSqlRelationalTableModel(this);
    templateModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    //queryModel = new QSqlQueryModel(this);
    tableLabel = new QLabel;
    QPixmap pixAdd(":/add.png");
    addRecordTable = new QPushButton(trUtf8("Добавить"));
    addRecordTable->setIcon(pixAdd);
    connect(addRecordTable,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPixmap pixDelete(":/delete.png");
    deleteRecordTable = new QPushButton(trUtf8("Удалить"));
    deleteRecordTable->setIcon(pixDelete);
    connect(deleteRecordTable,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QPixmap pixEdit(":/edit.png");
    editRecordTable = new QPushButton(trUtf8("Редактировать"));
    editRecordTable->setIcon(pixEdit);
    connect(editRecordTable,SIGNAL(clicked()),this,SLOT(editRecordOfTable()));

    QHBoxLayout *buttonBox = new QHBoxLayout;

    buttonBox->addWidget(addRecordTable);
    buttonBox->addWidget(deleteRecordTable);
    buttonBox->addWidget(editRecordTable);
    buttonBox->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(buttonBox);
    rightLayout->addWidget(tableLabel);
    rightLayout->addWidget(tableView);
    panelWidget->setLayout(rightLayout);
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editRecordOfTable()));
}

void MainWindow::viewTemplateTable(QString tempTable)
{
    if(tableView->model())
        for(int i=0; i<tableView->model()->columnCount();i++){
            QAbstractItemDelegate* delegate = tableView->itemDelegateForColumn( i );
            //Remove the delegate from the view
            tableView->setItemDelegateForColumn( i, NULL );
            delete delegate;
        }
    tableView->clearSpans();
    templateModel->clear();
    templateModel->setTable(tempTable);

    QString strivgValue; bool delAll = false;
    if(tempTable == "organization"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
        templateModel->setHeaderData(2,Qt::Horizontal,trUtf8("Местоположение"));
        templateModel->setHeaderData(3,Qt::Horizontal,trUtf8("Продавец"));
        templateModel->setHeaderData(4,Qt::Horizontal,trUtf8("Покупатель"));
        if(setFilter){
            templateModel->setFilter(QString("organizationname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = trUtf8("Организация");
    }else if(tempTable == "client"){
//        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Наименование"));
//        if(setFilter){
//            templateModel->setFilter(QString("organizationname LIKE '%%1%'").arg(filterTable));
//        }
        strivgValue = trUtf8("Клиент");
    }
    else{
        tableView->setModel(0);
        tableLabel->clear();
        delAll = true;
    }
    if(!delAll){
        templateModel->select();
        while(templateModel->canFetchMore())
            templateModel->fetchMore();
        QHeaderView *head = tableView->horizontalHeader();
        connect(head,SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));

        tableView->setModel(templateModel);
        tableView->setWordWrap(true);
        if(tempTable == "users" || tempTable == "trainingsafety" || tempTable == "contractdoc" ||
                tempTable == "siz"){
            tableView->setColumnHidden(0, false);
        }else{
            tableView->setColumnHidden(0, true);
        }

        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        tableView->setAlternatingRowColors(true);
        if(tempTable == "post"){
            tableView->setColumnWidth(1,600);
        }else{
            tableView->setColumnWidth(1,300);
            tableView->setColumnWidth(2,300);
        }
        if(tempTable == "organization"){
            tableView->setItemDelegateForColumn(3, new BoolItemDelegate);
            tableView->setItemDelegateForColumn(4, new BoolItemDelegate);
        }
        head->setStretchLastSection(true);
        tableLabel->clear();
        tableLabel->setText(trUtf8("Наименование таблицы: %1").arg(strivgValue));
        tableLabel->setStyleSheet("font: bold; color: darkblue;");
        setFilter = false;
        sortTable(1);
    }
}

void MainWindow::editRecordOfTable()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    QString userName = settings.value("CurrentUser").toString();
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE userid = :id");
    query.bindValue(":id",userName);
    query.exec();
    query.next();

    QString valueTemp, iDTemp;
    valueTemp = templateModel->tableName();
    QModelIndex index = tableView->currentIndex();
    if(index.isValid()){
        QSqlRecord record =templateModel->record(index.row());
        if(valueTemp == "client"){
            iDTemp = record.value("clientid").toString();
            if(query.value(0).toInt() == 1){
                ClientForm openForm(iDTemp, this,false);
                openForm.exec();
            }else{
                ClientForm openForm(iDTemp, this,true);
                openForm.exec();
            }
        }else if(valueTemp == "organization"){
            iDTemp = record.value("organizationid").toString();
            if(query.value(0).toInt() == 1){
                OrganizationForm openForm(iDTemp, this,false);
                openForm.exec();
            }else{
                OrganizationForm openForm(iDTemp, this,true);
                openForm.exec();
            }
        }
    }
    updateRightPanel(index);
}

void MainWindow::addRecordOfTable()
{
    QString idTable;
    QString valueTemp = templateModel->tableName();
//    QSettings settings("AO_Batrakov_Inc.", "Calculation");
//    QString userName = settings.value("CurrentUser").toString();
//    QSqlQuery query;
//    query.prepare("SELECT role FROM users WHERE userid = :id");
//    query.bindValue(":id",userName);
//    query.exec();
//    query.next();
//    if(query.value(0).toInt() == 1){
        if(valueTemp == "client"){
            ClientForm anyForm("",this,false);
            anyForm.exec();
            idTable = anyForm.returnValue();
        }else if(valueTemp == "organization"){
            OrganizationForm orgForm("",this,false);
            orgForm.exec();
            idTable = orgForm.returnValue();
        }

        QSqlQuery query;
        QString val1 = "SELECT * FROM ";
        val1 += valueTemp;
        val1 += " WHERE ";
        val1 += valueTemp;
        val1 += "id = :id";
        query.prepare(val1);

        query.bindValue(":id",idTable);
        query.exec();
        query.next();

        filterTable = query.value(2).toString();
        setFilter = true;
        viewTemplateTable(valueTemp);
        QModelIndex modIndex = tableView->currentIndex();
        MainWindow::updateRightPanel(modIndex);
//    }else{
//        QMessageBox::warning(this,tr("Внимание!!!"),tr("Вы не можете добавлять запись, у Вас нет прав!"));
//    }
}

void MainWindow::deleteRecordOfTable()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    QString userName = settings.value("CurrentUser").toString();
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE userid = :id");
    query.bindValue(":id",userName);
    query.exec();
    query.next();
    if(query.value(0).toInt() == 1){
        QModelIndex index = tableView->currentIndex();
        if(index.isValid()){
            QMessageBox msgBox(this);
            msgBox.setWindowTitle(trUtf8("Внимание!!!"));
            msgBox.setText(trUtf8("Действительно удалить?"));

            msgBox.setIcon(QMessageBox::Question);
            msgBox.addButton(QMessageBox::Yes);
            QPushButton *noButton = msgBox.addButton(QMessageBox::No);

            msgBox.setDefaultButton(noButton);

            noButton->setStyleSheet("QPushButton:hover {color: red}");
            int k = msgBox.exec();
            if(k == QMessageBox::Yes){
                QString valueTemp, iDValue;
                valueTemp = templateModel->tableName();
                QSqlRecord record = templateModel->record(index.row());
                if(valueTemp == "client"){
                    iDValue = record.value("clientid").toString();
                    ClientForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }else if(valueTemp == "organization"){
                    iDValue = record.value("organizationid").toString();
                    OrganizationForm openForm(iDValue,this,false);
                    openForm.deleteRecord();
                }
            }
        }
        updateRightPanel(index);
    }else{
        QMessageBox::warning(this,"Attention!!!","You don't add record, Seed Role!");
    }
}

void MainWindow::updateRightPanel(QModelIndex inDex)
{
    templateModel->select();
    QHeaderView *head = tableView->horizontalHeader();
    head->setStretchLastSection(true);
    while(templateModel->canFetchMore())
        templateModel->fetchMore();
    tableView->setCurrentIndex(inDex);
    //tableView->set
}

void MainWindow::sortTable(int index)
{
    templateModel->setSort(index,Qt::AscendingOrder);
    templateModel->select();
    while(templateModel->canFetchMore())
        templateModel->fetchMore();
}

void MainWindow::viewClient()
{
    viewTemplateTable("client");
}

void MainWindow::viewOrganization()
{
    viewTemplateTable("organization");
}

void MainWindow::viewLogistika()
{
    viewTemplateTable("logistika");
}
