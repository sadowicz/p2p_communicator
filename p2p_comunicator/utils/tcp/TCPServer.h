#pragma once

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

#include <contacts/Storage.h>
#include <contacts/Contact.h>
#include <config/Config.h>
#include <tcp/TCPPacket.h>
#include <logger/Logger.h>

#include <unordered_map>

using namespace contacts;

class TCPServer : public QObject {
    Q_OBJECT

public:
    TCPServer(short port);

private:
    QTcpServer* server;
    unordered_map<QTcpSocket*, string> clientIPs;

private slots:
    void onAcceptConnection();
    void onReadyRead();
    void onDisconnected();

signals:
    void recieved(string, TCPPacket);
    void connected(string, short);
    void disconnected(string);

};

