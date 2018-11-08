#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QtWidgets>

class QToolButton;

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void updateCloseButton(QString text);

private:
    QToolButton *clearButton;
};

#endif // LINEEDIT_H
