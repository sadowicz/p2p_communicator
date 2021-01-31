#include "MessageListDelegate.h"

#include <QDebug>

/*
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

    Message* message = index.data().value<Message*>();
    auto contact = dynamic_cast<const Contact*>(index.model());

    QString contactName = message->getSender() == Message::CONTACT
            ? QString::fromStdString(contact->getName())
            : "Me";

    contactName.append(':');

    int height = getSenderHeight(contactName, option) + 2*padding.height();

    if(message->getType() == Message::TEXT){
        height += getMessageHeight(message, option);
        height += option.fontMetrics.lineSpacing();
    }else if(message->getType() == Message::FILE){
        height += getDownloadHeight(message, option);
        height += option.fontMetrics.lineSpacing();
    }

    return QSize(option.rect.width(), height);
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const {

    if(!index.data().canConvert<Message*>())
        return;

    Message* message = qvariant_cast<Message*>(index.data());
    auto contact = dynamic_cast<const Contact*>(index.model());

    // set styles
    QStyleOptionViewItem opt = option;
    const QWidget* widget = opt.widget;
    QStyle* style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem,&opt,painter,widget);

    //draw Message
    paintMessage(message, contact ,painter, opt);

    if(message->getType() == Message::FILE)
    {
        // paint download button if message have file
        paintDownload(message, painter, opt);
    }

}

void MessageListDelegate::paintMessage(const Message* message, const Contact* contact, QPainter* painter, const QStyleOptionViewItem &option) const{

    QString content = QString::fromStdString(message->getContent());
    QString sender = message->getSender() == Message::CONTACT
            ? QString::fromStdString(contact->getName()) : "Me";
    sender.append(':');

    int lineSpace = option.fontMetrics.lineSpacing();
    QRect textRect = option.rect;

    textRect.setX(option.rect.left() + padding.width());
    textRect.setY(option.rect.top() + padding.height());
    textRect.setWidth(textRect.width() - padding.width());

    // print message
    painter->drawText(textRect,Qt::TextWrapAnywhere, sender);

    if(content != ""){
        textRect.setY(textRect.y() + 2*lineSpace);
        painter->drawText(textRect,Qt::TextWrapAnywhere, content);
    }

}

void MessageListDelegate::paintDownload(const Message* message, QPainter* painter, const QStyleOptionViewItem &option) const{

    QStyleOptionButton btn;

    int w = 75;
    int h = 25;
    int x = option.rect.left() + padding.width();
    int y = option.rect.bottom() - h - padding.height();

    if(message->getSender() == Message::CONTACT){
        btn.rect = QRect(x,y,w,h);
        btn.text = "Download";

        QRect fileNameLabel = QRect(x+w+10,y+5,option.rect.width()-w,h);
        painter->drawText(fileNameLabel,Qt::TextWrapAnywhere,QString::fromStdString(message->getFilename()));
        QApplication::style()->drawControl( QStyle::CE_PushButton, &btn, painter);
    }else{
        QString fileName = QString::fromStdString(message->getFilename());
        QRect fileNameLabel = QRect(x,y,option.rect.width(),h);
        painter->drawText(
                    fileNameLabel,
                    Qt::TextWrapAnywhere,
                    fileName.prepend("File sent: ")
                    );
    }
}

int MessageListDelegate::getMessageHeight(const Message* message, const QStyleOptionViewItem &option)const {

    QRect textRect = option.rect;
    textRect.setWidth(textRect.width() - 2*padding.width());

    QString content = QString::fromStdString(message->getContent());
    QRect contentRect = option.fontMetrics.boundingRect(textRect,Qt::TextWrapAnywhere,content);

    return contentRect.height();
}

int MessageListDelegate::getDownloadHeight(const Message* message, const QStyleOptionViewItem &option)const{

    int height = 0;
    QString fileName = QString::fromStdString(message->getFilename());

    if(message->getSender() == Message::ME){
        QRect fileNameRect = option.fontMetrics.boundingRect(
                    QRect(0,0,option.rect.width() - 2*padding.width(), 75),
                    Qt::TextWrapAnywhere,
                    fileName.prepend("File sent: ")
                    );

        return height + fileNameRect.height();
    }

    // Message from Contact
    QRect fileNameRect = option.fontMetrics.boundingRect(
                QRect(0,0,option.rect.width() - 85 - 2*padding.width(), 75),
                Qt::TextWrapAnywhere,
                fileName
                );

    if(fileNameRect.size().height() < 25){
        height = 25;
    }else{
        height = fileNameRect.height();
    }

    return height;
}

int MessageListDelegate::getSenderHeight(const QString& contactName, const QStyleOptionViewItem &option) const{
    QRect textRect = option.rect;
    textRect.setWidth(textRect.width() - 2*padding.width());

    QRect senderRect = option.fontMetrics.boundingRect(textRect,Qt::TextWrapAnywhere,contactName);

    return senderRect.height();
}












