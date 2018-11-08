#ifndef BOOLITEMDELEGATE_H
#define BOOLITEMDELEGATE_H

#include <QAbstractItemModel>
#include <QtWidgets/QItemDelegate>

class BoolItemDelegate : public QItemDelegate
{
    //Q_OBJECT

public:
    BoolItemDelegate(QObject *parent=0);
    ~BoolItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state) const;
};
#endif // BOOLITEMDELEGATE_H
