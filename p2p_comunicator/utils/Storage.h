#pragma once
#include "Contact.h"

class Storage
{
public:
    bool load();
    bool save() const;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    QList<Contact> contacts;
};
