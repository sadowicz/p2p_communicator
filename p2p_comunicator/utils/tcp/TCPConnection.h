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
    static TCPConnection& get();
    TCPClient* registerClient(Contact* contact);

private:
    TCPServer* server;
    unordered_map<string, TCPClient*> clients;

signals:
    void recieved(Contact*, TCPPacket);
    void connected(Contact*);
    void disconnected(Contact*);
    void sendingError(Contact*, TCPException);

public slots:
    void closeConnection(string& ip);
    void reconnect(string& ip);
    void send(string& ip, string& content);

private slots:
    void onConnect(Contact* contact) {
        contact->setActiveState(true);
        emit connected(contact);
    }

    void onDisconnect(Contact* contact) {
        contact->setActiveState(false);
        emit disconnected(contact);
    }

};
