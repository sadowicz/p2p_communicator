#pragma once

#include <QObject>

#include <TCPConnection.h>
#include <TCPPacket.h>

#include <contacts/Storage.h>

namespace contacts {

class ContactController : QObject {
    Q_OBJECT

public:
    ContactController(Logger& log);
    void addContact(Contact* contact);
    void removeContact(string& ip);
    void editContact(Contact* editedContact);
    void send(Contact* contact, string& message);
    bool isActive(string& ip);

    void tryConnect(string& ip);
    void forceDisconnect(string& ip);

private:
    TCPConnection* connection;
    Logger& log;

private slots:
    void onConnect(string, short);
    void onDisconnect(string);
    void onRecieve(string, TCPPacket);
    void onSendError(string, TCPException);

};

}
