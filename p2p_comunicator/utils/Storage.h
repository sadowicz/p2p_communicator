#pragma once
#include "Contact.h"

class Storage
{
public:
    bool load(); // load contacts from .json file
    bool save() const;  // save contacts to .json file

    void addContact(Contact newContact);

private:
    QList<Contact> contacts;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};
