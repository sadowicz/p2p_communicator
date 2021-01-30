#include <tcp/TCPClient.h>

TCPClient::TCPClient(string ip, short port) : ip(ip), port(port) {
    this->socket = new QTcpSocket(this);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void TCPClient::tryConnect() {
    Logger::log().debug("Trying to connect to contact: " + ip);

    QHostAddress hostAddress = QHostAddress(QString(ip.c_str()));
    socket->connectToHost(hostAddress, port);
}

void TCPClient::forceDisconnect() {
    Logger::log().debug("Force disconnected from contact: " + ip);
    socket->disconnectFromHost();
}

void TCPClient::onError(QAbstractSocket::SocketError e) {
    emit disconnected(ip);
    Logger::log().debug("Couldn't connect to contact: " + ip + ", reason: " + std::to_string(e));
}

void TCPClient::onDisconnect() {
    emit disconnected(ip);
    Logger::log().debug("Client disconnected from contact: " + ip);
}

void TCPClient::onConnect() {
    emit connected(ip, port);
    Logger::log().debug("Client connected to contact: " + ip);
}

void TCPClient::send(string& packet) {
    Logger::log().debug("Sending message to contact: " + ip + " ...");

    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit failed(ip, TCPException("Client error: client was not connected to host"));
        return;
    }

    size_t writingResult = socket->write(packet.c_str());

    if (writingResult == -1 || writingResult != packet.size()) {
        string bytesWritten = to_string(writingResult);
        string totalBytes = to_string(packet.size());
        string error = strbuilder() + "Client error: writing failed, "
                 + bytesWritten + " out of " + totalBytes + " bytes written" + strbuilder::end();
        emit failed(ip, TCPException(error));
        return;
     }

     // remove this (?)
     if (!socket->waitForBytesWritten()) {
        emit failed(ip, TCPException("Client error: could not send packet (timeout)"));
        return;
     }
     Logger::log().debug("... message sent successfully");
}

