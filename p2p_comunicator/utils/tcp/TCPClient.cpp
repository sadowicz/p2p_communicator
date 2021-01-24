#include <tcp/TCPClient.h>

TCPClient::TCPClient(Storage& storage) : storage(storage) {
    this->socket = new QTcpSocket();
}

void TCPClient::send(string& address, string& packet) {
    if (!storage.contactExists(address)) {
        throw TCPException("Client error: contact doesn't exist");
    }

    QHostAddress hostAddress = QHostAddress(QString(address.c_str()));
    socket->connectToHost(hostAddress, (quint16) std::stoi(Config::get("port")));

    if (!socket->waitForConnected()) {
        throw TCPException("Client error: could not connect to host (timeout)");
    }

    if (socket->state() == QAbstractSocket::ConnectedState) {
        size_t writingResult = socket->write(packet.c_str());
        if (writingResult == -1 || writingResult != packet.size()) {
            string error = strbuilder()<<"Client error: writing failed, "
                    <<to_string(writingResult)<<" out of "<<to_string(packet.size())<<" bytes written"<<strbuilder::end();
            throw TCPException(error);
        }

        if (!socket->waitForBytesWritten()) {
            throw TCPException("Client error: could not send packet (timeout)");
        }
    } else {
        throw TCPException("Client error: could not connect to host");
    }
}

