#pragma once
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class Contact
{
public:
    Contact(QString name, QString address, unsigned port);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void addToHistory(QString message);

private:
    QString name;
    QString address;
    int port;
    QList<QString> history;
};
