#include "MessageListDelegate.h"

#include <QDebug>

/*
    2. Wywietlanie nick w message
    3. Signal z download
*/

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
    const QFontMetrics& fMetrics = option.fontMetrics;


    QRect senderRect = fMetrics.boundingRect(option.rect,Qt::TextWrapAnywhere,sender);
    QRect contentRect = fMetrics.boundingRect(option.rect,Qt::TextWrapAnywhere,content);

    QSize size = senderRect.size();

    if(content != ""){
        size += contentRect.size();
    }

    size.setHeight(size.height()+lineSpace);

    //add button size
    if(message->getType() == Message::FILE){

        int x = option.rect.left() + padding.width();
        int y = option.rect.bottom() - 25 - padding.height();

        QRect fileNameRect = fMetrics.boundingRect(
                    QRect(x+85,y+5,85 + x,75),
                    Qt::TextWrapAnywhere,
                    QString::fromStdString(message->getFilename())
                    );

        if(fileNameRect.size().height() < 25){
            size.setHeight(size.height()+25);
        }else{
            size.setHeight(size.height()+fileNameRect.height());
        }
     }



    return size + 2*padding;
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

    if(message->getType() == Message::FILE)
    {
        // paint download button if message have file
        paintDownload(message, painter, opt);
    }

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

    if(content != "")
        painter->drawText(textRect,Qt::TextWrapAnywhere, content);

}

void MessageListDelegate::paintDownload(const Message* message, QPainter* painter, const QStyleOptionViewItem &option) const{

    QStyleOptionButton btn;

    int w = 75;
    int h = 25;
    int x = option.rect.left() + padding.width();
    int y = option.rect.bottom() - h - padding.height();

    btn.rect = QRect(x,y,w,h);
    btn.text = "Download";

    QRect fileNameLabel = QRect(x+w+10,y+5,option.rect.width()-w,h);
    painter->drawText(fileNameLabel,Qt::TextWrapAnywhere,QString::fromStdString(message->getFilename()));

    QApplication::style()->drawControl( QStyle::CE_PushButton, &btn, painter);

}
