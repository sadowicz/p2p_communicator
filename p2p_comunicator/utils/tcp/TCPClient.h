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

class TCPClient : public QObject {

    Q_OBJECT
public:
    TCPClient(Contact* contact);

    ~TCPClient() { delete socket; }

    void send(string& packet);
    void tryConnect();

private:
    Contact* contact;
    QTcpSocket* socket;

private slots:
    void onDisconnect();
    void onConnect();
    void onError(QAbstractSocket::SocketError e);

signals:
    void failed(Contact* contact, TCPException e);
    void connected(Contact* contact);
    void disconnected(Contact* contact);

};
