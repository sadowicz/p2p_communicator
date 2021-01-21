#pragma once
#include <QString>
#include <QList>
#include <QJsonObject>

class Contact
{
public:
    Contact();
    Contact(QString name, QString address, unsigned port);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    QString name;
    QString address;
    int port;
    QList<QString> history;
};
