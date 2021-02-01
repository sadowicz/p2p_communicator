#include "Contact.h"

using namespace contacts;

Contact::Contact(std::string name, std::string address, unsigned int port, QObject* parent)
    : QAbstractListModel(parent), name(name), address(address), port(port), active(false), unreadMsg(false) {}

void Contact::read(const QJsonObject &json) {
    name = json["name"].toString().toStdString();
    address = json["address"].toString().toStdString();
    port = json["port"].toInt();

    history.clear();
    QJsonArray historyArray = json["history"].toArray();
    for (int i = 0; i < historyArray.size(); ++i) {
        QJsonObject contactMessage = historyArray[i].toObject();

        // using QJsonDocument to convert QJsonObject to QString
        Message* msg = new Message(contactMessage,this);
        history.push_back(msg);
    }
}

void Contact::write(QJsonObject &json) {
    json["name"] = QString(name.c_str());
    json["address"] = QString(address.c_str());
    json["port"] = (int) port;

    QJsonArray historyArray;
    for (Message* message : history) {
        historyArray.append(message->serialize());
    }
    json["history"] = historyArray;
}

void Contact::addToHistory(Message* message) {
    beginResetModel();
    message->setParent(this);
    history.push_back(message);
    endResetModel();

    emit onHistoryChange();
}

std::string Contact::getName() const {
    return this->name;
}

std::string Contact::getAddress() const {
    return this->address;
}

unsigned int Contact::getPort() const {
    return this->port;
}

void Contact::setPort(unsigned int port) {
    this->port = port;
}

void Contact::setName(std::string name) {
    this->name = name;
}

bool Contact::isActive() const {
    return this->active;
}

bool Contact::hasUnreadMsg() const {
    return this->unreadMsg;
}

void Contact::setUnreadMsgState(bool state) {
    this->unreadMsg = state;
}

void Contact::setActiveState(bool state) {
    this->active = state;
}

int Contact::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()){
        return 0;
    }

    return history.size();
}

QVariant Contact::data(const QModelIndex &index, int role) const {

   // if(index.isValid())
     //   return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    Message* msg = history[index.row()];

    return QVariant::fromValue(msg);
}


