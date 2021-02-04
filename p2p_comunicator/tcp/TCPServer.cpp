#include <TCPServer.h>

#define BLOCK_SIZE 4096

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

        if (bufferInfos.find(connection) == bufferInfos.end()) {
            bufferInfos[connection] = ClientBufferInfo(address);
        }

        connect(connection, SIGNAL(readyRead()), SLOT(onReadyRead()));
        connect(connection, SIGNAL(disconnected()), SLOT(onDisconnected()));

        emit connected(address, peerPort);
    }
}

void TCPServer::onDisconnected() {
    QTcpSocket* socket = (QTcpSocket*) sender();
    ClientBufferInfo& bufferInfo = bufferInfos[socket];
    string& ip = bufferInfo.getAddress();
    log.debug("Server disconnected from: " + ip);
    bufferInfos.erase(socket);

    emit disconnected(ip);
}

void TCPServer::onReadyRead() {
    QTcpSocket* socket = (QTcpSocket*) sender();
    ClientBufferInfo& bufferInfo = bufferInfos[socket];

    // if first part of packet was sent
    if (bufferInfo.getExpectedSize() == 0) {
        int packetSize = getPacketSize(socket);
        bufferInfo.setExpectedSize(packetSize);
    }

    // read data
    int bytesAvailable = socket->bytesAvailable();
    int totalRead = 0;
    char* buffer = (char*) calloc(BLOCK_SIZE, sizeof(char));

    while (totalRead < bytesAvailable) {
        int bytesRead = socket->read(buffer, BLOCK_SIZE);
        if (-1 == bytesRead) onReadingError(bufferInfo);
        bufferInfo.add(buffer);
        memset(buffer, 0, BLOCK_SIZE);
        totalRead += bytesRead;
    }

    free(buffer);

    // if it's the last part of the packet
    if (bufferInfo.isDone()) {
        bufferInfo.setExpectedSize(0);
        emit recieved(bufferInfo.getAddress(), TCPPacket::decode(*bufferInfo.getBuffer()));
        bufferInfo.getBuffer()->clear();
    }
}

void TCPServer::onReadingError(ClientBufferInfo& buffer) {
    log.debug("Error while reading from socket");
    buffer.setExpectedSize(0);
    buffer.getBuffer()->clear();
}

int TCPServer::getPacketSize(QTcpSocket* socket) {
    while (socket->bytesAvailable() < sizeof(int));
    QByteArray header = socket->read(sizeof(int));
    int packetSize = 0;
    memcpy(&packetSize, header.toStdString().c_str(), sizeof(int));
    return packetSize;
}


