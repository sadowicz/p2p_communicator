#pragma once
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <contacts/Message.h>

class Contact
{
public:
    Contact(QString name, QString address, unsigned port);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void addToHistory(Message* message);

    QString getName();
    QString getAddress();
    int getPort();

private:
    QString name;
    QString address;
    int port;
    QList<Message*> history;

};
