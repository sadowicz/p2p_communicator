#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

#include <contacts/Storage.h>
#include <contacts/Contact.h>
#include <config/Config.h>
#include <tcp/TCPPacket.h>

class TCPServer : public QObject {
    Q_OBJECT

public:
    TCPServer() = delete;
    TCPServer(Storage& storage);

private:
    QTcpServer* server;
    Storage& storage;

private slots:
    void acceptConnection();

};

#endif // TCP_SERVER_H
