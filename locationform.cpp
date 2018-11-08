#include <QtWidgets>
#include <QtSql>

#include "locationform.h"
#include "lineedit.h"
#include "numprefix.h"


LocationForm::LocationForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    indexTemp = id;

    labelName = new QLabel(trUtf8("Наименование:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExpFamiliya("[\\x0410-\\x044f 0-9 \" -]{150}");
    editName->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelName->setBuddy(editName);

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

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT locationname FROM location WHERE locationid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());
        }
    }else{
        editName->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Местоположение"));
    readSettings();
}

void LocationForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE location SET locationname = :name WHERE locationid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
        line += "UPDATE location SET locationname = '";
        line += editName->text().toUtf8();
        line += "' WHERE locationid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM location WHERE locationname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("location");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO location (locationid, locationname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            line += "INSERT INTO location (locationid, locationname) VALUES('";
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

void LocationForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    QSqlQuery query;
    query.prepare("DELETE FROM location WHERE locationid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();

    line += "DELETE FROM location WHERE locationid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void LocationForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void LocationForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    restoreGeometry(settings.value("LocationForm").toByteArray());
}

void LocationForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("LocationForm", saveGeometry());
}
