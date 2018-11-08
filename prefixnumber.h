#ifndef PREFIXNUMBER_H
#define PREFIXNUMBER_H

#include <QtWidgets>
#include "lineedit.h"

class QLabel;
class LineEdit;
class QDialogButtonBox;

class PrefixNumber : public QDialog
{
    Q_OBJECT
public:
    PrefixNumber(QWidget *);
    void done(int);
    QString returnValue() const {return prefix;};

private slots:
    void editPN();

private:
    QLabel *labelPrefix;
    LineEdit *editPrefix;
    QPushButton *savePushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;
    QString prefix;

protected:
    void keyReleaseEvent(QKeyEvent *);
};

#endif // PREFIXNUMBER_H
