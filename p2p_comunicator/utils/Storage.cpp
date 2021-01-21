#include "Storage.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

bool Storage::load() {
    QFile loadFile(QStringLiteral("../../data.json"));

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
    QFile saveFile(QStringLiteral("../../data.json"));

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
        Contact contact{};
        contact.read(contactObject);
        contacts.append(contact);
    }
}

void Storage::write(QJsonObject &json) const {

    QJsonArray contactsArray;
    foreach (const Contact contact, contacts) {
        QJsonObject contactObject;
        contact.write(contactObject);
        contactsArray.append(contactObject);
    }
    json["contacts"] = contactsArray;
}

void Storage::addContact(Contact newContact) {
    contacts.append(newContact);
}

