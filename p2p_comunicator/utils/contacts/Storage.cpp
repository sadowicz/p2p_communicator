#include "Storage.h"


Storage& Storage::storage(){
    static Storage s;
    return s;
}

bool Storage::load() {
    QString filename = QString(Config::get("history-log-file").c_str());
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
    QFile saveFile(QString(Config::get("history-log-file").c_str()));

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

    contacts.clear();
    QJsonArray contactsArray = json["contacts"].toArray();
    for (int i = 0; i < contactsArray.size(); ++i) {
        QJsonObject contactObject = contactsArray[i].toObject();
        Contact contact("example", "127.0.0.1", 8080);
        contact.read(contactObject);
        contacts[contact.getAddress()] = contact;
    }
}

std::unordered_map<std::string, Contact>& Storage::getContacts() {
    return contacts;
}

bool Storage::contactExists(std::string& ip) {
    return util::has(contacts, ip);
}

Contact* Storage::getContact(std::string& ip) {
    return &contacts[ip];
}

void Storage::write(QJsonObject &json) const {

    QJsonArray contactsArray;
    for (std::pair<std::string, Contact> entry : contacts) {
        QJsonObject contactObject;
        entry.second.write(contactObject);
        contactsArray.append(contactObject);
    }
    json["contacts"] = contactsArray;
}

void Storage::addContact(Contact newContact) {
    contacts[newContact.getAddress()] = newContact;
    save();
}

void Storage::clear(){
    contacts.clear();
    save();
}

void Storage::deleteContact(std::string& ip) {
    contacts.erase(ip);
    save();
}

void Storage::editContact(std::string ip, std::string newName, std::string newAddress, unsigned newPort) {
    auto oldContact = getContact(ip);
    auto editedContact = Contact(newName, newAddress, newPort);

    auto history = oldContact->getHistory();

    for(auto entry : history)
        editedContact.addToHistory(entry);

    deleteContact(oldContact->getAddress());
    addContact(editedContact);
}
