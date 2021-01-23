#include "Contact.h"

// TODO: validate IP before saving
Contact::Contact(QString name, QString address, unsigned port) : name(name), address(address), port(port) {}

void Contact::read(const QJsonObject &json) {
    name = json["name"].toString();
    address = json["address"].toString();
    port = json["port"].toInt();

    foreach (const Message* message, history) {
        delete message;
    }
    history.clear();

    QJsonArray historyArray = json["history"].toArray();
    for (int i = 0; i < historyArray.size(); ++i) {
        QJsonObject contactMessage = historyArray[i].toObject();

        // using QJsonDocument to convert QJsonObject to QString

        Message* msg = Message::deserialize(contactMessage);
        history.append(msg);
    }
}

void Contact::write(QJsonObject &json) const {
    json["name"] = name;
    json["address"] = address;
    json["port"] = port;

    QJsonArray historyArray;
    foreach (Message* message, history) {
        QJsonObject msgObject{};
        message->serialize(msgObject);
        historyArray.append(msgObject);
    }
    json["history"] = historyArray;
}

void Contact::addToHistory(Message* message) {
    history.append(message);
}

QString Contact::getName() {
    return this->name;
}

QString Contact::getAddress() {
    return this->address;
}

int Contact::getPort() {
    return this->port;
}





