#pragma once

#include <QObject>

#include <unordered_map>
#include <string>

#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>
#include <Logger.h>

using namespace std;


class TCPConnection : public QObject {
    Q_OBJECT

public:
    TCPConnection(Logger& log) : log(log) {}
    TCPClient* registerClient(const string ip, unsigned int port);
    void startServer(unsigned int port);

private:
    TCPServer* server;
    unordered_map<string, TCPClient*> clients;
    Logger& log;
    unsigned int myPort;

signals:
    void recieved(const string, TCPPacket);
    void connected(const string, unsigned int);
    void disconnected(const string);
    void sendingError(const string, TCPException);

public slots:
    void closeConnection(const string& ip);
    void reconnect(const string& ip);
    void send(const string& ip, const string& content);

};
