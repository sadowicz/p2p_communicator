#ifndef MESSAGELISTDELEGATE_H
#define MESSAGELISTDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QFont>
#include <QPushButton>

#include <contacts/Message.h>
#include <contacts/Contact.h>

using namespace contacts;

class MessageListDelegate : public QStyledItemDelegate
{

public:
    MessageListDelegate(QObject* parent = nullptr);


    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QSize padding;


    void paintMessage(const Message* message, const Contact* contact, QPainter* painter, const QStyleOptionViewItem &option) const;
    void paintDownload(const Message* message, QPainter* painter, const QStyleOptionViewItem &option) const;

    int getSenderHeight(const QString& contactName, const QStyleOptionViewItem &option) const;
    int getMessageHeight(const Message* message, const QStyleOptionViewItem &option) const;
    int getDownloadHeight(const Message* message, const QStyleOptionViewItem &option) const;


};

#endif // MESSAGELISTDELEGATE_H
