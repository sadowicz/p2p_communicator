#pragma once
#include <string>

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QObject>
#include <QDebug>

#include <Logger.h>
#include <TCPException.h>

using namespace std;

class TCPClient : public QObject {

    Q_OBJECT
public:
    TCPClient(Logger& log, string ip, short port);

    ~TCPClient() { delete socket; }

    void send(const string& packet);
    void tryConnect();
    void forceDisconnect();

private:
    string ip;
    short port;
    QTcpSocket* socket;
    Logger& log;

private slots:
    void onDisconnect();
    void onConnect();
    void onError(QAbstractSocket::SocketError e);

signals:
    void failed(const string ip, TCPException e);
    void connected(const string ip, short port);
    void disconnected(const string ip);

};
