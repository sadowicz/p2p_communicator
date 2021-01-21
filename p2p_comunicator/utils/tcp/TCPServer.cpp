#include "TCPServer.h"

TCPServer::TCPServer() {
    this->server = new QTcpServer(this);
    int port = std::stoi(Config::get("port"));
    this->server->listen(QHostAddress::Any, (quint16) port);
    connect(this->server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void TCPServer::acceptConnection() {
    QTcpSocket* connection = this->server->nextPendingConnection();

    // TODO: recieve message and add new contact if necessary

    connection->close();
    delete connection;
}
