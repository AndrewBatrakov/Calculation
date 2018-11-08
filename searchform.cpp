#include <QtGui>
#include <QtSql>
#include "searchform.h"

SearchForm::SearchForm(QString valueTempModel,QWidget *parent) :
    QDialog(parent)
{
    stTemp = "";
    labelSearch = new QLabel(trUtf8("Search:"));
    editSearch = new LineEdit;

    /*QString textQuery = "SHOW COLUMNS FROM ";
    textQuery += valueTempModel;

    if(valueTempModel == "employee"){

    }
    QSqlQuery query;
    query.exec(textQuery);
    QStringList stringList;
    while(query.next()){
        QString temp;
        temp = query.value(0).toString();
        temp += query.value(1).toString();
        stringList << temp;
    }

    field = new QComboBox;
    field->addItems(stringList);*/

    searchPushButton = new QPushButton(trUtf8("Search"));
    connect(searchPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    searchPushButton->setDefault(true);

    cancelPushButton = new QPushButton(trUtf8("Cancel"));
    //cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(close()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(searchPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelSearch,0,0);
    mainLayout->addWidget(editSearch,0,1);
    //mainLayout->addWidget(field,1,1);
    mainLayout->addWidget(buttonBox,2,1);
    setLayout(mainLayout);
    setWindowTitle(trUtf8("Search"));
}


void SearchForm::done(int result)
{
    stTemp = "";
    if (result == QDialog::Accepted) {
        stTemp = editSearch->text().simplified();
    }
    QDialog::done(result);
}
