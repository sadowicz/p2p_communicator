#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <contacts/Message.h>
#include <vector>

namespace contacts {

class Contact {
public:

    Contact() {}
    Contact(std::string name, std::string address, unsigned port);

    void read(const QJsonObject &json);
    void write(QJsonObject &json);

    void addToHistory(Message message);
    std::vector<Message>& getHistory() { return history; }

    std::string& getName();
    std::string& getAddress();
    int getPort();
    bool isActive();
    void setActiveState(bool state);

private:
    std::string name;
    std::string address;
    int port;
    std::vector<Message> history;
    bool active;

};

}
