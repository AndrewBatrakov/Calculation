#include <QtWidgets>
#include <QtSql>

#include "viewlisttable.h"
#include "organization.h"
#include "searchform.h"
#include "locationform.h"

ViewListTable::ViewListTable(QString idTable, QString nameTable, QWidget *parent) :
    QDialog(parent)
{
    tableName = nameTable;
    iDTemp = idTable;
    labelName = "";

    stBar = new QStatusBar;
    addButton = new QPushButton(trUtf8("Добавить"));
    QPixmap pixAdd(":/add.png");
    addButton->setIcon(pixAdd);
    connect(addButton,SIGNAL(clicked()),this,SLOT(addRecord()));

    deleteButton = new QPushButton(trUtf8("Удалить"));
    QPixmap pixDel(":/delete.png");
    deleteButton->setIcon(pixDel);
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteRecord()));

    editButton = new QPushButton(trUtf8("Редактировать"));
    QPixmap pixEdit(":/edit.png");
    editButton->setIcon(pixEdit);
    connect(editButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    searchButton = new QPushButton(trUtf8("Поиск (по наименованию)"));
    connect(searchButton,SIGNAL(clicked()),this,SLOT(searchProcedure()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addStretch();

    tableView = new QTableView;
    tableView->clearSpans();

    templateModel = new QSqlRelationalTableModel(this);

    viewTemplateTable(tableName);
    if(iDTemp != ""){
        for(int row = 0; row  < templateModel->rowCount(); ++row){
            QSqlRecord record = templateModel->record(row);
            QModelIndex index = templateModel->index(row,1);
            if(record.value(0).toString() == iDTemp){
                tableView->setCurrentIndex(index);
                break;
            }
        }
    }else{
        tableView->setCurrentIndex(templateModel->index(0,0));
    }

    //tableView->resizeColumnsToContents();
    tableView->setAlternatingRowColors(true);
    QHeaderView *header = tableView->horizontalHeader();
    header->setStretchLastSection(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(tableView);
    setLayout(mainLayout);

    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));

    setWindowTitle(trUtf8("List of %1").arg(labelName));
    resize(tableView->size().width()+200,tableView->size().height());

    createContextMenu();
    readSettings();

    searchString = new QLabel(this);
    int wL = this->width() - 204;
    searchString->setGeometry(wL,4,200,30);
    searchString->setMargin(4);
    searchString->setStyleSheet("QLabel{font: bold; background-color : #FFFF99; color : blue; }" );
    searchString->setVisible(false);
}

void ViewListTable::viewTemplateTable(QString)
{
    templateModel->setTable(tableName);

    if(tableName == "organization"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        labelName = trUtf8("Организации");
        if(setFilter){
            templateModel->setFilter(QString("organizationname LIKE '%%1%'").arg(filterTable));
        }
    }else if(tableName == "location"){
        templateModel->setHeaderData(1,Qt::Horizontal,trUtf8("Name"));
        labelName = trUtf8("Местонахождение");
        if(setFilter){
            templateModel->setFilter(QString("locationname LIKE '%%1%'").arg(filterTable));
        }
    }

    templateModel->setSort(1,Qt::AscendingOrder);
    templateModel->select();

    tableView->setModel(templateModel);

    tableView->setColumnHidden(0,true);
    tableView->setColumnWidth(1,300);
    tableView->setItemDelegate(new QSqlRelationalDelegate(this));
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //tableView->setRowHeight();
}

void ViewListTable::done(int res)
{
    writeSettings();
    if(res == QDialog::Accepted){
        QModelIndex index = tableView->currentIndex();
        if(index.isValid()){
            QSqlRecord record = templateModel->record(index.row());
            QString per = tableName;
            per += "id";
            iDTemp = record.value(per).toString();
        }else{
            iDTemp = "";
        }
    }
    QDialog::done(res);
}

void ViewListTable::addRecord()
{
    QString nameList;
    if(tableName == "organization"){
        OrganizationForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }else if(tableName == "location"){
        LocationForm listForm("",this,false);
        listForm.exec();
        nameList = listForm.returnValue();
    }
    templateModel->select();
    for(int row = 0; row < templateModel->rowCount(); ++row){
        QSqlRecord record = templateModel->record(row);
        QModelIndex index = templateModel->index(row,1);
        if(record.value(0).toString() == nameList){
            tableView->setCurrentIndex(index);
            updatePanel(index);
            break;
        }
    }
}

void ViewListTable::deleteRecord()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(trUtf8("Attention!!!"));
    msgBox.setText(trUtf8("Really delete?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(QMessageBox::Yes);
    QPushButton *noButton = msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(noButton);

    noButton->setStyleSheet("QPushButton:hover {color: red}");
    int k = msgBox.exec();
    if(k == QMessageBox::Yes){
        QModelIndex index = tableView->currentIndex();
        QSqlRecord record = templateModel->record(index.row());
        QString idList = record.value(0).toString();
        if(tableName == "organization"){
            OrganizationForm listForm(idList,this,false);
            listForm.deleteRecord();
        }else if(tableName == "location"){
            LocationForm listForm(idList,this,false);
            listForm.deleteRecord();
        }
        updatePanel(index);
    }
}

void ViewListTable::editRecord()
{
    QModelIndex index = tableView->currentIndex();
    QSqlRecord record = templateModel->record(index.row());
    QString idList = record.value(0).toString();
    if(tableName == "organization"){
        OrganizationForm listFrom(idList,this,false);
        listFrom.exec();
    }else if(tableName == "location"){
        LocationForm listFrom(idList,this,false);
        listFrom.exec();
    }
    updatePanel(index);
}

void ViewListTable::updatePanel(QModelIndex index)
{
    templateModel->select();
    tableView->setCurrentIndex(index);
}

void ViewListTable::createContextMenu()
{
    addAction = new QAction(trUtf8("Добавить"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecord()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(trUtf8("Удалить"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecord()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(trUtf8("Редактировать"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecord()));

    tableView->addAction(addAction);
    tableView->addAction(deleteAction);
    tableView->addAction(editAction);
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void ViewListTable::searchProcedure()
{
    QString valueTempModel = templateModel->tableName();

    SearchForm searchForm(valueTempModel, this);
    searchForm.exec();
    filterTable = searchForm.returnValue();
    setFilter = true;

    viewTemplateTable(valueTempModel);
}

void ViewListTable::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    restoreGeometry(settings.value("ViewListTable").toByteArray());
}

void ViewListTable::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    settings.setValue("ViewListTable", saveGeometry());
}

void ViewListTable::keyReleaseEvent(QKeyEvent *event)
{
    QString tm = templateModel->tableName();
    if( event->key() == Qt::Key_Escape ){
        searchString->setText("");
        if(searchString->isVisible()){
            searchString->setVisible(false);
            //statusBar()->setVisible(false);
        }
        setFilter = false;
        viewTemplateTable(tm);
    }
    if((event->key() >= 0x0410 && event->key() < 0x044f) || event->key() == Qt::Key_Space || event->key() == Qt::Key_Backspace){
        if(!searchString->isVisible()){
//            statusBar()->addWidget(searchString);
//            statusBar()->show();
            searchString->show();
            searchString->setFocus();
        }
        QString txt = searchString->text();
        if(event->key() == Qt::Key_Backspace){
            txt.chop(1);
        }else{
            txt += event->text();
        }
        searchString->setText(txt);
        QString rr = searchString->text().toUpper();
        filterTable = rr;
        setFilter = true;
        viewTemplateTable(tm);
    }
}
