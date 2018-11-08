#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QtWidgets>
#include "lineedit.h"

class RegistrationForm : public QDialog
{
    Q_OBJECT

public:
    RegistrationForm(QWidget *parent = 0);

private slots:
    bool checkInput();

private:
    QLabel *labelUser;
    QLabel *labelPassword;
    QLabel *labelHost;
    QLabel *labelBase;

    LineEdit *editUser;
    LineEdit *editPassword;
    LineEdit *editHost;
    LineEdit *editBase;

    QPushButton *okPushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;
};


#endif // REGISTRATIONFORM_H
