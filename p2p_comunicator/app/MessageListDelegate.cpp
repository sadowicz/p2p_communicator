#include "MessageListDelegate.h"

#include <QDebug>

MessageListDelegate::MessageListDelegate(QObject* parent)
    : QStyledItemDelegate(parent),
      padding(10,10)
{
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{

    if(!index.data().canConvert<Message*>())
        return QSize();

    Message* message =index.data().value<Message*>();

    QString content = QString::fromStdString(message->getContent());
    QString sender =  QString::fromStdString(message->getAddress());

    QRect rect = option
            .fontMetrics
            .boundingRect(option.rect,Qt::TextWrapAnywhere,sender+"\n\n\n"+content);


     return rect.size();
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const {

    if(!index.data().canConvert<Message*>())
        return;

    Message* message = qvariant_cast<Message*>(index.data());

    QString content = QString::fromStdString(message->getContent());
    QString sender = QString::fromStdString(message->getAddress());

    int lineSpace = option.fontMetrics.lineSpacing();

    QRect textRect = option.rect;

    textRect.setX(textRect.x() + padding.width());
    textRect.setY(textRect.y() + padding.height());

    // print message
    painter->drawText(textRect,Qt::TextWrapAnywhere, sender);

    textRect.setY(textRect.y() + lineSpace);

    painter->drawText(textRect,Qt::TextWrapAnywhere, content);
}
