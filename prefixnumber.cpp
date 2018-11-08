#include <QtSql>
#include "prefixnumber.h"

PrefixNumber::PrefixNumber(QWidget *parent) :
    QDialog(parent)
{
    labelPrefix = new QLabel(trUtf8("Префикс:"));
    editPrefix = new LineEdit;

    QRegExp regExp("[A-Za-z]{3}");
    editPrefix->setValidator(new QRegExpValidator(regExp,this));
    labelPrefix->setBuddy(editPrefix);

    savePushButton = new QPushButton(trUtf8("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editPN()));

    cancelPushButton = new QPushButton(trUtf8("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QSettings settings("AO_Batrakov_Inc.", "Calculation");
    if(settings.value("numprefix").toString() == ""){
        editPrefix->setText(tr("Три буквы латинского алфавита"));
        editPrefix->selectAll();
    }else{
        editPrefix->setText(settings.value("numprefix").toString());
    }
    editPrefix->setReadOnly(false);
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelPrefix,0,0);
    mainLayout->addWidget(editPrefix,0,1);
    mainLayout->addWidget(buttonBox,1,1);
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Prefix Number "));
}

void PrefixNumber::editPN()
{
    if(editPrefix->text().count() == 3){
        QSettings settings("AO_Batrakov_Inc.", "Calculation");
        if(editPrefix->text() != "SRV"){
            settings.setValue("numprefix", editPrefix->text());
        }else{
            editPrefix->clear();
            return;
        }
        prefix = editPrefix->text();
        emit accept();
        close();
    }

}

void PrefixNumber::keyReleaseEvent(QKeyEvent *)
{
    editPrefix->setText(editPrefix->text().toUpper());
}

void PrefixNumber::done(int result)
{
    QDialog::done(result);
}
