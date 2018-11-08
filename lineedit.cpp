#include <QtGui>
#include "lineedit.h"
#include <QToolButton>
#include <QStyle>

LineEdit::LineEdit(QWidget *parent)
   : QLineEdit(parent){
   clearButton = new QToolButton(this);
   QPixmap pixmap(":/fileclose.png");
   clearButton->setIcon(QIcon(pixmap));
   clearButton->setIconSize(pixmap.size());
   clearButton->setCursor(Qt::ArrowCursor);
   clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
   clearButton->hide();
   //if(!isReadOnly()){
       connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
  // }
   connect(this, SIGNAL(textChanged(QString)), this, SLOT(updateCloseButton(QString)));
   int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
   setStyleSheet(QString("QLineEdit { padding-right: %1px; } ")
                 .arg(clearButton->sizeHint().width() + frameWidth + 1));
   QSize msz = minimumSizeHint();
   setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                  qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
}

void LineEdit::resizeEvent(QResizeEvent *)
{
   QSize sz = clearButton->sizeHint();
   int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
   clearButton->move(rect().right() - frameWidth - sz.width(),
                     (rect().bottom() + 1 - sz.height())/2);
}

void LineEdit::updateCloseButton(QString text)
{
    if(!isReadOnly()){
    clearButton->setVisible(!text.isEmpty());
    }
    int j = fontMetrics().width(text);
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    setMinimumWidth(j + clearButton->sizeHint().height() + frameWidth * 2 + 10);
}
