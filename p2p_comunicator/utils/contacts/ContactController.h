#pragma once

#include <QObject>

#include <tcp/TCPConnection.h>
#include <tcp/TCPPacket.h>

#include <contacts/Storage.h>

class ContactController : QObject {
    Q_OBJECT

public:
    ContactController();
    void addContact(Contact* contact);
    void removeContact(string& ip);
    void editContact(Contact* editedContact);
    void send(Contact* contact, string& message);
    bool isActive(string& ip);

    void tryConnect(string& ip);
    void forceDisconnect(string& ip);

private:
    TCPConnection* connection;

private slots:
    void onConnect(string, short);
    void onDisconnect(string);
    void onRecieve(string, TCPPacket);
    void onSendError(string, TCPException);

};
