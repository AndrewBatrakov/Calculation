#include <QtWidgets>
#include <QtSql>

#include "organization.h"
#include "lineedit.h"
#include "numprefix.h"
#include "locationform.h"
#include "viewlisttable.h"

OrganizationForm::OrganizationForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    QFile file(":/ToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());

    indexTemp = id;

    labelName = new QLabel(trUtf8("Наименование:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelName->setBuddy(editName);

    QVBoxLayout *nameLayout = new QVBoxLayout;
    QVBoxLayout *editLayout = new QVBoxLayout;
    nameLayout->addWidget(labelName);
    editLayout->addWidget(editName);

    labelLocation = new QLabel(trUtf8("Местоположение:"));
    editLocation = new LineEdit;
    editLocation->setReadOnly(onlyForRead);
    labelLocation->setBuddy(editLocation);
    nameLayout->addWidget(labelLocation);

    QSqlQueryModel *locModel = new QSqlQueryModel;
    locModel->setQuery("SELECT locationname FROM location");
    QCompleter *locCompleter = new QCompleter(locModel);
    locCompleter->setCompletionMode(QCompleter::PopupCompletion);
    locCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editLocation->setCompleter(locCompleter);

    QToolButton *addButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addButton->setIcon(addPix);
    addButton->setToolTip(trUtf8("Добавить новую запись"));
    connect(addButton,SIGNAL(clicked()),this,SLOT(addLocRecord()));
    addButton->setStyleSheet(styleSheetString);

    QToolButton *seeButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeButton->setIcon(seePix);
    seeButton->setToolTip(trUtf8("Посмотреть выбранную запись"));
    connect(seeButton,SIGNAL(clicked()),this,SLOT(seeLocRecord()));
    seeButton->setStyleSheet(styleSheetString);

    QToolButton *listButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listButton->setIcon(listPix);
    listButton->setToolTip(trUtf8("Просмотреть список записей"));
    connect(listButton,SIGNAL(clicked()),this,SLOT(listLocRecord()));
    listButton->setStyleSheet(styleSheetString);

    QHBoxLayout *editLocationLayout = new QHBoxLayout;
    //editLocationLayout->addWidget(labelLocation);
    editLocationLayout->addWidget(editLocation);
    if(!onlyForRead){
        editLocationLayout->addWidget(addButton);
        editLocationLayout->addWidget(seeButton);
        editLocationLayout->addWidget(listButton);
    }

    QGroupBox *editLocLayout = new QGroupBox;
    editLocLayout->setLayout(editLocationLayout);

    editLayout->addWidget(editLocLayout);

    labelBayer = new QLabel(trUtf8("Покупатель:"));
    editBayer = new QCheckBox;
    //editBayer->setReadOnly(onlyForRead);
    //labelBayer->setBuddy(editBayer);
    nameLayout->addWidget(labelBayer);
    editLayout->addWidget(editBayer);

    labelSeller = new QLabel(trUtf8("Продавец:"));
    editSeller = new QCheckBox;
    //editSeller->setReadOnly(onlyForRead);
    labelSeller->setBuddy(editSeller);
    editLayout->addWidget(editSeller);
    nameLayout->addWidget(labelSeller);

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Кнопка \"Записать и закрыть\""));

    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Кнопка Отмена"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);
    if(!onlyForRead){
        //editLayout->addWidget(buttonBox);
        editName->selectAll();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT org.organizationname, (SELECT loc.locationname FROM location AS loc "
                      "WHERE loc.locationid = org.locationid), org.buyer, org.seller FROM organization AS org WHERE org.organizationid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());
            editLocation->setText(query.value(1).toString());
            if(query.value(2).toBool() == true){
                editBayer->setCheckState(Qt::Checked);
            }else{
                editBayer->setCheckState(Qt::Unchecked);
            }
        }
    }else{
        editName->clear();
    }

    QHBoxLayout *personalLayout = new QHBoxLayout;
    personalLayout->addLayout(nameLayout);
    personalLayout->addLayout(editLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(personalLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Организация"));
    readSettings();
}

void OrganizationForm::editRecord()
{
    if(checkFill()){
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE organization SET organizationname = :name WHERE  organizationid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        line += "UPDATE organization SET organizationname = '";
        line += editName->text().toUtf8();
        line += "' WHERE  organizationid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM organization WHERE organizationname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("organization");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO organization (organizationid, organizationname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            line += "INSERT INTO organization (organizationid, organizationname) VALUES('";
            line += indexTemp;
            line += "', '";
            line += editName->text().simplified().toUtf8();
            line += "')";
            line += "\r\n";
            stream<<line;
        }else{
            QString tempString = editName->text();
            tempString += QObject::trUtf8(" существует!");
            QMessageBox::warning(this,QObject::trUtf8("Внимание!!!"),tempString);
        }
    }
    emit accept();
    close();
    }
}

void OrganizationForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSqlQuery query;
    query.prepare("DELETE FROM organization WHERE organizationid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();

    line += "DELETE FROM organization WHERE organizationid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void OrganizationForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void OrganizationForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("OrganizationForm").toByteArray());
}

void OrganizationForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    settings.setValue("OrganizationForm", saveGeometry());
}

void OrganizationForm::addLocRecord()
{
    LocationForm orgForm("",this,false);
    orgForm.exec();
    if(orgForm.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT locationname From location WHERE locationid = :id");
        query.bindValue(":id",orgForm.returnValue());
        query.exec();
        query.next();
        editName->setText(query.value(0).toString());
    }
}

void OrganizationForm::seeLocRecord()
{
    QSqlQuery query;
    query.prepare("SELECT locationid FROM location WHERE locationname = :name");
    query.bindValue(":name",editLocation->text());
    query.exec();
    while(query.next()){
        LocationForm orgForm(query.value(0).toString(),this,true);
        orgForm.exec();
    }
}

void OrganizationForm::listLocRecord()
{
    QSqlQuery query;
    query.prepare("SELECT locationid FROM location WHERE locationname = :name");
    query.bindValue(":name",editLocation->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"location",this);
    listTemp.exec();
    if(listTemp.returnValue() != ""){
        QSqlQuery query;
        query.prepare("SELECT locationname FROM location WHERE locationid = :id");
        query.bindValue(":id",listTemp.returnValue());
        query.exec();
        query.next();
        editLocation->setText(query.value(0).toString());
    }
}

bool OrganizationForm::checkFill()
{
    bool fill = true;
    if(editName->text() == ""){
        editName->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editName->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    if(editLocation->text() == ""){
        editLocation->setStyleSheet("background-color: red;");
        QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Поле не может быть пустым!"));
        editLocation->setStyleSheet("background-color: #FFFFCC;");
        fill = false;
    }
    return fill;
}
