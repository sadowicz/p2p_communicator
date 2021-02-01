#ifndef MESSAGELISTDELEGATE_H
#define MESSAGELISTDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QFont>


#include <contacts/Message.h>
#include <contacts/Contact.h>

using namespace contacts;

class MessageListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    MessageListDelegate(QObject* parent = nullptr);


    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:

    void downloadClicked(Message* message);

private:
    QSize padding;
    QStyle::State btnState;

    void paintMessage(const Message* message, QPainter* painter, const QStyleOptionViewItem &option) const;
    void paintSender(const QString sender, QPainter* painter, const QStyleOptionViewItem &option) const;
    void paintTimeStamp(const Message* message, QPainter* painter, const QStyleOptionViewItem &option) const;
    void paintDownload(const Message* message, QPainter* painter, const QStyleOptionViewItem &option) const;

    int getSenderHeight(const QString& contactName, const QStyleOptionViewItem &option) const;
    int getMessageHeight(const Message* message, const QStyleOptionViewItem &option) const;
    int getDownloadHeight(const Message* message, const QStyleOptionViewItem &option) const;


};

#endif // MESSAGELISTDELEGATE_H
