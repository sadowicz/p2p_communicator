#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

#include <config/Config.h>

class TCPServer : public QObject {
    Q_OBJECT

public:
    TCPServer();

private:
    QTcpServer* server;

private slots:
    void acceptConnection();

};

#endif // TCP_SERVER_H
