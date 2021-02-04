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
    void editContact(const std::string ip, std::string name, unsigned int port);
    void sendMessage(const string& ip, const string& message);
    bool isActive(const string& ip) const;
    void tryConnect(const string& ip);
    void forceDisconnect(const string& ip);
    bool contactExists(const string& ip);

public slots:
    void onMsgRead(const string);
    void onFileReady(string filename, const string* content);
    void onFileCancelled();

signals:
    void refreshContactList();
    void msgTooLong();  //TODO: connect this to an error handler

private:
    TCPConnection* connection;
    Logger& log;

    bool fileIsReady;
    string filename;
    const string* fileContent;

    void onTextAndFileMessage(const string ip, TCPPacket packet);
    void onConnectMessage(const string ip, TCPPacket packet);

private slots:
    void onConnect(const string, unsigned int);
    void onDisconnect(const string);
    void onRecieve(const string, TCPPacket);
    void onSendError(const string, TCPException);

};

}
