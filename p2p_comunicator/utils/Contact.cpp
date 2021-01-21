#include "Contact.h"

Contact::Contact(QString name, QString address, unsigned port) : name(name), address(address), port(port) {}

void Contact::read(const QJsonObject &json) {
    name = json["name"].toString();
    address = json["address"].toString();
    port = json["port"].toInt();

    history.clear();
    QJsonArray historyArray = json["history"].toArray();
    for (int i = 0; i < historyArray.size(); ++i) {
        QJsonObject contactMessage = historyArray[i].toObject();

        // using QJsonDocument to convert QJsonObject to QString
        QJsonDocument Doc(contactMessage);
        QByteArray ba = Doc.toJson();
        history.append(QString(ba));
    }
}

void Contact::write(QJsonObject &json) const {
    json["name"] = name;
    json["address"] = address;
    json["port"] = port;

    QJsonArray historyArray;
    foreach (const QString message, history) {
        historyArray.append(message);
    }
    json["history"] = historyArray;
}

void Contact::addToHistory(QString message) {
    history.append(message);
}



