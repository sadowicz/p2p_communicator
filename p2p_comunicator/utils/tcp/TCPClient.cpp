#include <tcp/TCPClient.h>

TCPClient::TCPClient(Contact& contact) : contact(contact) {
    this->socket = new QTcpSocket(this);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void TCPClient::tryConnect() {
    Logger::log().debug("Trying to connect to contact: " + contact.getAddress());

    QHostAddress hostAddress = QHostAddress(QString(contact.getAddress().c_str()));
    socket->connectToHost(hostAddress, (short) contact.getPort());
}

void TCPClient::onError(QAbstractSocket::SocketError e) {
    emit disconnected(&contact);
}

void TCPClient::onDisconnect() {
    emit disconnected(&contact);
    Logger::log().debug("Disconnected from contact: " + contact.getAddress());
}

void TCPClient::onConnect() {
    emit connected(&contact);
    Logger::log().debug("Connected to contact: " + contact.getAddress());
}

void TCPClient::send(string& packet) {
    Logger::log().debug("Sending message to contact: " + contact.getAddress() + " ...");

    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit failed(&contact, TCPException("Client error: client was not connected to host"));
        return;
    }

    size_t writingResult = socket->write(packet.c_str());

    if (writingResult == -1 || writingResult != packet.size()) {
        string bytesWritten = to_string(writingResult);
        string totalBytes = to_string(packet.size());
        string error = strbuilder() + "Client error: writing failed, "
                 + bytesWritten + " out of " + totalBytes + " bytes written" + strbuilder::end();
        emit failed(&contact, TCPException(error));
        return;
     }

     if (!socket->waitForBytesWritten()) {
        emit failed(&contact, TCPException("Client error: could not send packet (timeout)"));
        return;
     }
     Logger::log().debug("... message sent successfully");
}

