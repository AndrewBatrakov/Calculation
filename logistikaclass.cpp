#include "logistikaclass.h"

LogistikaClass::LogistikaClass(QWidget *parent) : QDialog(parent)
{
    clientNameL = new QLabel(tr("Наименование клиента:"));
    clientNameE = new LineEdit;
    clientNameL->setBuddy(clientNameE);

    clientLocationL = new QLabel(tr("Местоположение клиента:"));
    clientLocationE = new LineEdit;
    clientLocationL->setBuddy(clientLocationE);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(clientNameL,0,0);
    mainLayout->addWidget(clientNameE,0,1);
    mainLayout->addWidget(clientLocationL,0,3);
    mainLayout->addWidget(clientLocationE,0,4);

    setLayout(mainLayout);

    setWindowTitle(tr("Калькуляция Логистики"));
}
