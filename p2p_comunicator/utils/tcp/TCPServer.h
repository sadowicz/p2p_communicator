#pragma once

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

#include <contacts/Storage.h>
#include <contacts/Contact.h>
#include <config/Config.h>
#include <tcp/TCPPacket.h>

#include <unordered_map>

class TCPServer : public QObject {
    Q_OBJECT

public:
    TCPServer() = delete;
    TCPServer(Storage& storage);

private:
    QTcpServer* server;
    Storage& storage;
    unordered_map<QTcpSocket*, Contact*> contacts;

private slots:
    void onAcceptConnection();
    void onReadyRead();
    void onDisconnected();

signals:
    void recieved(Contact*, TCPPacket);
    void connected(Contact*);
    void disconnected(Contact*);

};

