#include "Contact.h"

// TODO: validate IP before saving
Contact::Contact(std::string name, std::string address, unsigned port) : name(name), address(address), port(port) {}

void Contact::read(const QJsonObject &json) {
    name = json["name"].toString().toStdString();
    address = json["address"].toString().toStdString();
    port = json["port"].toInt();

    history.clear();
    QJsonArray historyArray = json["history"].toArray();
    for (int i = 0; i < historyArray.size(); ++i) {
        QJsonObject contactMessage = historyArray[i].toObject();

        // using QJsonDocument to convert QJsonObject to QString
        Message msg(contactMessage);
        history.push_back(msg);
    }
}

void Contact::write(QJsonObject &json) {
    json["name"] = QString(name.c_str());
    json["address"] = QString(address.c_str());
    json["port"] = port;

    QJsonArray historyArray;
    for (Message& message : history) {
        historyArray.append(message.serialize());
    }

    json["history"] = historyArray;
}

void Contact::addToHistory(Message message) {
    history.push_back(message);
}

std::string& Contact::getName() {
    return this->name;
}

std::string& Contact::getAddress() {
    return this->address;
}

int Contact::getPort() {
    return this->port;
}





