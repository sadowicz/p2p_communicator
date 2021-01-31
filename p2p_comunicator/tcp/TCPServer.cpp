#include <TCPServer.h>

TCPServer::TCPServer(Logger& log, unsigned int port) : log(log) {
    this->server = new QTcpServer(this);
    this->server->listen(QHostAddress::Any, port);
    log.info("Server listening on port " + std::to_string(port));

    connect(this->server, SIGNAL(newConnection()), SLOT(onAcceptConnection()));
}

void TCPServer::onAcceptConnection() {
    while (server->hasPendingConnections()) {
        QTcpSocket* connection = this->server->nextPendingConnection();
        std::string address = QHostAddress(connection->peerAddress().toIPv4Address()).toString().toStdString();
        unsigned int peerPort = connection->peerPort();

        log.debug("Server accepting connection from: " + address + ":" + std::to_string(peerPort));

        clientIPs[connection] = address;

        connect(connection, SIGNAL(readyRead()), SLOT(onReadyRead()));
        connect(connection, SIGNAL(disconnected()), SLOT(onDisconnected()));

        emit connected(address, peerPort);
    }
}

void TCPServer::onDisconnected() {
    QTcpSocket* socket = (QTcpSocket*) sender();
    string ip = clientIPs[socket];
    log.debug("Server disconnected from: " + ip);

    emit disconnected(ip);
}

void TCPServer::onReadyRead() {

    QTcpSocket* socket = (QTcpSocket*) sender();
    std::string content{};

    // read message content
    while (socket->bytesAvailable() > 0) {
        content.append(socket->readAll().toStdString());
    }

    string ip = clientIPs[socket];
    log.debug("Server recieved a message from: " + ip);

    emit recieved(ip, TCPPacket::decode(content));
}


