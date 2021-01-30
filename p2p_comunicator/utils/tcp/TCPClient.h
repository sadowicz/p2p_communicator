#pragma once
#include <string>

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QObject>
#include <QDebug>

#include <contacts/Storage.h>
#include <config/Config.h>
#include <logger/Logger.h>

using namespace std;
using namespace contacts;

class TCPClient : public QObject {

    Q_OBJECT
public:
    TCPClient(string ip, short port);

    ~TCPClient() { delete socket; }

    void send(string& packet);
    void tryConnect();
    void forceDisconnect();

private:
    string ip;
    short port;
    QTcpSocket* socket;

private slots:
    void onDisconnect();
    void onConnect();
    void onError(QAbstractSocket::SocketError e);

signals:
    void failed(string ip, TCPException e);
    void connected(string ip, short port);
    void disconnected(string ip);

};
