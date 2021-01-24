#pragma once
#include <string>

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>

#include <contacts/Storage.h>
#include <config/Config.h>

using namespace std;

class TCPClient {

public:
    TCPClient(Storage& storage);

    ~TCPClient() { delete socket; }

    // connect and send packet
    void send(string& address, string& packet);

private:
    Storage& storage;
    QTcpSocket* socket;
};
