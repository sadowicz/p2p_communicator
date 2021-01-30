#include "Storage.h"

using namespace contacts;

Storage& Storage::storage(){
    static Storage s;
    return s;
}

bool Storage::load() {
    QString filename = QString::fromStdString(Config::config()["history-log-file"]);
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    read(loadDoc.object());

    return true;
}

bool Storage::save() const {
    QFile saveFile(QString::fromStdString(Config::config("history-log-file")));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject contactsObject;
    write(contactsObject);
    QJsonDocument saveDoc(contactsObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

void Storage::read(const QJsonObject &json) {

    for (pair<std::string, Contact*> pair : contacts) {
        delete pair.second;
    }
    contacts.clear();
    QJsonArray contactsArray = json["contacts"].toArray();
    for (int i = 0; i < contactsArray.size(); ++i) {
        QJsonObject contactObject = contactsArray[i].toObject();
        Contact* contact = new Contact();
        contact->read(contactObject);
        contacts[contact->getAddress()] = contact;
    }
}

std::unordered_map<std::string, Contact*>& Storage::getContacts() {
    return contacts;
}

bool Storage::contactExists(const std::string& ip) {
    return contacts.find(ip) != contacts.end();
}

Contact* Storage::getContact(const std::string& ip) {
    return contacts[ip];
}

void Storage::write(QJsonObject &json) const {

    QJsonArray contactsArray;
    for (std::pair<std::string, Contact*> entry : contacts) {
        QJsonObject contactObject;
        entry.second->write(contactObject);
        contactsArray.append(contactObject);
    }
    json["contacts"] = contactsArray;
}

void Storage::addContact(Contact* newContact) {
    contacts[newContact->getAddress()] = newContact;
    save();
}

void Storage::clear(){
    contacts.clear();
    save();
}

void Storage::deleteContact(const std::string& ip) {
    delete contacts[ip];
    contacts.erase(ip);
    save();
}
