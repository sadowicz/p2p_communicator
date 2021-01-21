#include "Contact.h"

Contact::Contact(QString name, QString address, unsigned port) : name(name), address(address), port(port) {}

void Contact::read(const QJsonObject &json) {
    name = json["name"].toString();
    address = json["address"].toString();
    port = json["port"].toInt();
}

void Contact::write(QJsonObject &json) const {
    json["name"] = name;
    json["address"] = address;
    json["port"] = port;
}



