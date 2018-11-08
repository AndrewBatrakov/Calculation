#include <QtWidgets>
#include "registrationform.h"
#include "lineedit.h"
#include "databasedirection.h"
//#include "uploadbase.h"
//#include "ftpform.h"

RegistrationForm::RegistrationForm(QWidget *parent) :
    QDialog(parent)
{

    labelUser = new QLabel(trUtf8("Пользователь:"));
    editUser = new LineEdit;
    labelUser->setBuddy(editUser);

    labelPassword = new QLabel(trUtf8("Пароль:"));
    editPassword = new LineEdit;
    labelPassword->setBuddy(editPassword);
    editPassword->setEchoMode(QLineEdit::Password);

    //**************************
    editUser->setText("admin");
    editPassword->setText("admin");

    okPushButton = new QPushButton(trUtf8("Вход"));
    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(okPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);

    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(okPushButton,SIGNAL(clicked()),this,SLOT(checkInput()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelUser,2,0);
    mainLayout->addWidget(editUser,2,1);
    mainLayout->addWidget(labelPassword,3,0);
    mainLayout->addWidget(editPassword,3,1);
    mainLayout->addWidget(buttonBox,4,1);
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Вход в программу"));

    QWidget::setTabOrder(editUser,editPassword);
    QWidget::setTabOrder(cancelPushButton,okPushButton);
    QWidget::setTabOrder(editPassword,cancelPushButton);
    editUser->setFocus();

    QFile fileWD(":/QWidgetStyle.txt");
    fileWD.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(fileWD.readAll());
    QWidget::setStyleSheet(styleSheetString);

}

bool RegistrationForm::checkInput()
{
//    QFile file("./CE_SQLite.dat");
//    if(!file.exists()){
//        UpLoadBase upLoad(this);
//        upLoad.exeVersion();
//    }
    bool rr;

    DataBaseDirection manipulation;

    if(!manipulation.connectDataBase()){
        close();
        rr = false;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE (userid = :id AND userpas = :name);");
        query.bindValue(":id",editUser->text());
        query.bindValue(":name",editPassword->text());
        query.exec();
        query.next();
        if(!query.isValid()){
//            QMessageBox::warning(this, QObject::trUtf8("Идентификация"),
//                                 QObject::trUtf8("Идентификация пользователя не выполнена!\n"
//                                                 "Попробуйте исправить настройки ftp\n"
//                                                 "и перезапустите программу."));
//            FtpForm openForm(this);
//            openForm.exec();
//            rr = false;
//            close();
        }else{
            QSettings settings("AO_Batrakov_Inc.", "Calculation");
            settings.setValue("CurrentUser",editUser->text());
            accept();
            rr = true;
        }
    }

    return rr;
}
