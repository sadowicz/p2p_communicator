#pragma once
#include <contacts/Contact.h>
#include <config/Config.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMap>

#include <unordered_map>
#include <util/util.h>

namespace contacts {

class Storage
{
public:
    Storage(const Storage&) = delete;

    static Storage& storage();

    bool load(); // load contacts from .json file
    bool save() const;  // save contacts to .json file
    void clear();

    void addContact(Contact* newContact);
    void deleteContact(const std::string& ip);
    std::unordered_map<std::string, Contact*>& getContacts();
    Contact* getContact(std::string& ip);
    bool contactExists(std::string& ip);

private:
    Storage() = default;

    std::unordered_map<std::string, Contact*> contacts;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

}
