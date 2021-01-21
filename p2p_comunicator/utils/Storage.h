#pragma once
#include "Contact.h"

class Storage
{
public:
    bool load();
    bool save() const;

    void addContact(Contact newContact);

private:
    QList<Contact> contacts;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};
