#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

class tcp_server : public QObject {
    Q_OBJECT

public:
    tcp_server();

private:
    QTcpServer* server;

};

#endif // TCP_SERVER_H
