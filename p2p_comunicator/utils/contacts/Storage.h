#pragma once
#include <contacts/Contact.h>
#include <config/Config.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMap>

#include <unordered_map>
#include <util/util.h>

class Storage
{
public:
    bool load(); // load contacts from .json file
    bool save() const;  // save contacts to .json file

    void addContact(Contact newContact);
    void deleteContact(std::string& ip);
    void editContact(std::string& ip, std::string newName, std::string newAddress, unsigned newPort);
    std::unordered_map<std::string, Contact>& getContacts();
    Contact* getContact(std::string& ip);
    bool contactExists(std::string& ip);

private:
    std::unordered_map<std::string, Contact> contacts;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};
