#pragma once

#include <QObject>

#include <unordered_map>
#include <string>

#include <tcp/TCPClient.h>
#include <tcp/TCPServer.h>
#include <tcp/TCPPacket.h>

using namespace std;


class TCPConnection : public QObject {
    Q_OBJECT

public:
    TCPConnection(Storage& storage) : storage(storage) {}
    static void init(Storage& storage);
    static TCPConnection* get() {return TCPConnection::connection;}

private:
    static TCPConnection* connection;
    Storage& storage;

    TCPServer* server;
    unordered_map<string, TCPClient*> clients;

signals:
    void recieved(Contact*, TCPPacket);
    void connected(Contact*);
    void disconnected(Contact*);
    void sendingError(Contact*, TCPException);

public slots:
    void send(string& ip, string& content);

};
