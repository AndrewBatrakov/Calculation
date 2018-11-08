#ifndef LOGISTIKACLASS_H
#define LOGISTIKACLASS_H

#include <QtWidgets>
#include "lineedit.h"

class LogistikaClass : public QDialog
{
    Q_OBJECT
public:
    LogistikaClass(QWidget *parent);

public slots:

private:
    QLabel *clientNameL;
    LineEdit *clientNameE;
    QLabel *clientLocationL;
    LineEdit *clientLocationE;
};

#endif // LOGISTIKACLASS_H
