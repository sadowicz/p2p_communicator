#include <TCPClient.h>

TCPClient::TCPClient(Logger& log, string ip, unsigned int myPort, unsigned int port) : ip(ip), port(port), myPort(myPort), log(log) {
    socket = new QTcpSocket();
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void TCPClient::tryConnect() {
    log.debug("Trying to connect to contact: " + ip);
    if (socket->state() != QTcpSocket::ConnectingState) {
        QHostAddress hostAddress = QHostAddress(QString(ip.c_str()));
        socket->connectToHost(hostAddress, port);
    } else {
        log.debug("Client is already trying to connect");
    }
}

bool TCPClient::isConnected() {
    return socket->state() == QTcpSocket::ConnectedState;
}

void TCPClient::forceDisconnect() {
    log.debug("Force disconnected from contact: " + ip);
    socket->disconnectFromHost();
}

void TCPClient::onError(QAbstractSocket::SocketError e) {
    emit disconnected(ip);
    log.debug("Couldn't connect to contact: " + ip + ", error code: " + std::to_string(e));
}

void TCPClient::onDisconnect() {
    emit disconnected(ip);
    log.debug("Client disconnected from contact: " + ip);
}

void TCPClient::onConnect() {
    send(TCPPacket::encode(TCPPacket::PacketType::CONNECTION, "", std::to_string(myPort)));

    emit connected(ip, port);
    log.debug("Client connected to contact: " + ip + ":" + std::to_string(port));
}

void TCPClient::send(const string& packet) {
    log.debug("Sending message to contact: " + ip + " ...");

    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit failed(ip, TCPException("Client error: client was not connected to host"));
        return;
    }

    int writingResult = socket->write(packet.c_str());

    if (writingResult == -1 || writingResult != packet.size()) {
        string error = "Client error: writing failed, "
                 + to_string(writingResult) + " out of " + to_string(packet.size()) + " bytes written";
        emit failed(ip, TCPException(error));
        return;
     }

     // remove this (?)
     if (!socket->waitForBytesWritten()) {
        emit failed(ip, TCPException("Client error: could not send packet (timeout)"));
        return;
     }
     log.debug("... message sent successfully");
}

