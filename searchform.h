#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QtGui>
#include "lineedit.h"

class QLabel;
class LineEdit;
class QDialogButtonBox;

class SearchForm : public QDialog
{
    Q_OBJECT
public:
    SearchForm(QString, QWidget *parent);

    void done(int result);
    QString returnValue() const {return stTemp;};

private:
    QLabel *labelSearch;
    LineEdit *editSearch;

    QComboBox *field;

    QPushButton *searchPushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString stTemp;
};

#endif // SEARCHFORM_H
