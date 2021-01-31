#pragma once

#include <QObject>

#include <TCPConnection.h>
#include <TCPPacket.h>

#include <contacts/Storage.h>

namespace contacts {

class ContactController : public QObject {
    Q_OBJECT

public:
    ContactController(Logger& log);
    void addContact(Contact* contact);
    void removeContact(const string& ip);
    void editContact(Contact* editedContact);
    void sendTextMessage(const string& ip, const string& message);
    bool isActive(const string& ip) const;
    void tryConnect(const string& ip);
    void forceDisconnect(const string& ip);

public slots:
    void onMsgRead(const string);

signals:
    void refreshContactList();
    void msgReceived();

private:
    TCPConnection* connection;
    Logger& log;

    void onTextMessage(const string ip, TCPPacket packet);
    void onFileMessage(const string ip, TCPPacket packet);
    void onConnectMessage(const string ip, TCPPacket packet);

private slots:
    void onConnect(const string, unsigned int);
    void onDisconnect(const string);
    void onRecieve(const string, TCPPacket);
    void onSendError(const string, TCPException);

};

}
