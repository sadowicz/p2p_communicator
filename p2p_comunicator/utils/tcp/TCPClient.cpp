#include <tcp/TCPClient.h>

TCPClient::TCPClient(Contact* contact) : contact(contact) {
    this->socket = new QTcpSocket();
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));

    QHostAddress hostAddress = QHostAddress(QString(contact->getAddress().c_str()));
    socket->connectToHost(hostAddress, (short) contact->getPort());
}

void TCPClient::onDisconnect() {
    emit disconnected(contact);
}

void TCPClient::onConnect() {
    emit connected(contact);
}

void TCPClient::send(string& packet) {

    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit failed(contact, TCPException("Client error: client was not connected to host"));
        return;
    }

    size_t writingResult = socket->write(packet.c_str());

    if (writingResult == -1 || writingResult != packet.size()) {
        string bytesWritten = to_string(writingResult);
        string totalBytes = to_string(packet.size());
        string error = strbuilder() + "Client error: writing failed, "
                 + bytesWritten + " out of " + totalBytes + " bytes written" + strbuilder::end();
        emit failed(contact, TCPException(error));
        return;
     }

     if (!socket->waitForBytesWritten()) {
        emit failed(contact, TCPException("Client error: could not send packet (timeout)"));
        return;
     }
}

