#pragma once

#include <QObject>

#include <unordered_map>
#include <string>

#include <tcp/TCPClient.h>
#include <tcp/TCPServer.h>
#include <tcp/TCPPacket.h>
#include <logger/Logger.h>

using namespace std;


class TCPConnection : public QObject {
    Q_OBJECT

public:
    TCPConnection();
    TCPClient* registerClient(string ip, short port);
    void startServer(short port);

private:
    TCPServer* server;
    unordered_map<string, TCPClient*> clients;

signals:
    void recieved(string, TCPPacket);
    void connected(string, short);
    void disconnected(string);
    void sendingError(string, TCPException);

public slots:
    void closeConnection(string& ip);
    void reconnect(string& ip);
    void send(string& ip, string& content);

};
