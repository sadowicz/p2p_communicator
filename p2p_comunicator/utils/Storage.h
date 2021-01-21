#pragma once
#include "Contact.h"

#include <QMap>

class Storage
{
public:
    bool load(); // load contacts from .json file
    bool save() const;  // save contacts to .json file

    void addContact(Contact newContact);
    QMap<QString, Contact> getContacts();

private:
    QMap<QString, Contact> contacts;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};
