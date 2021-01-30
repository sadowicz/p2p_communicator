#ifndef MESSAGELISTDELEGATE_H
#define MESSAGELISTDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPointer>

#include <contacts/Message.h>

class MessageListDelegate : public QStyledItemDelegate
{

public:
    MessageListDelegate(QObject* parent = nullptr);


    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:

    QSize padding;

};

#endif // MESSAGELISTDELEGATE_H
