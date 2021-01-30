#include "MessageListDelegate.h"

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

    int lineSpace = option.fontMetrics.lineSpacing();

    QRect senderRect = option
            .fontMetrics
            .boundingRect(option.rect,Qt::TextWrapAnywhere,sender);

    QRect contentRect = option
            .fontMetrics
            .boundingRect(option.rect,Qt::TextWrapAnywhere,content);

    QSize size = senderRect.size() + contentRect.size() + 2* padding;
    size.setHeight(size.height()+lineSpace);

    return size;
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const {

    if(!index.data().canConvert<Message*>())
        return;

    Message* message = qvariant_cast<Message*>(index.data());



    // set styles
    QStyleOptionViewItem opt = option;
    const QWidget* widget = opt.widget;
    QStyle* style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem,&opt,painter,widget);


    //draw Message
    paintMessage(message, painter, opt);

}

void MessageListDelegate::paintMessage(const Message* message, QPainter* painter, const QStyleOptionViewItem &option) const{


    QString content = QString::fromStdString(message->getContent());
    QString sender = QString::fromStdString(message->getAddress());

    int lineSpace = option.fontMetrics.lineSpacing();
    QRect textRect = option.rect;

    textRect.setX(textRect.x() + padding.width());
    textRect.setY(textRect.y() + padding.height());

    // print message
    painter->drawText(textRect,Qt::TextWrapAnywhere, sender);
    textRect.setY(textRect.y() + 2*lineSpace);
    painter->drawText(textRect,Qt::TextWrapAnywhere, content);

}
