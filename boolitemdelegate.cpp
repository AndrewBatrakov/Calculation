#include "boolitemdelegate.h"
#include <QtWidgets>

BoolItemDelegate::BoolItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

BoolItemDelegate::~BoolItemDelegate()
{

}

void BoolItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool date = index.model()->data(index, Qt::DisplayRole).toBool();
    if(date == 0 || date == 1){
        QString text;
        Qt::CheckState state;
        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
        if(date == true){
            //text = trUtf8("true");
            myOption.palette.setColor(QPalette::Text,QColor(Qt::darkBlue));
            //drawBackground(painter, myOption, index);
            //painter->fillRect(option.rect, QColor(Qt::red));
            //myOption.palette.background().setColor(Qt::red);
            state = Qt::Checked;
        }else{
            //text = trUtf8("false");
            myOption.palette.setColor(QPalette::Text,QColor(Qt::black));
            //drawBackground(painter, myOption, index);
            //painter->fillRect(option.rect, QColor(Qt::));
            state = Qt::Unchecked;
        }


        drawDisplay(painter,myOption,myOption.rect,text);
        drawCheck(painter,myOption,myOption.rect,state);
        drawBackground(painter,myOption,index);
        drawFocus(painter,myOption,myOption.rect);

    }else{
        QItemDelegate::paint(painter,option,index);
    }
}

void BoolItemDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state) const
{
    const int textMargin = 0;//QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    QRect checkRect = QStyle::alignedRect(option.direction,Qt::AlignCenter,rect.size(),
                                          QRect(option.rect.x() + textMargin, option.rect.y(),
                                                option.rect.width() - (textMargin*2),option.rect.height()));
    QItemDelegate::drawCheck(painter,option,checkRect,state);
}
