#include <tcp/TCPServer.h>

TCPServer::TCPServer() {
    this->server = new QTcpServer(this);
    this->server->listen(QHostAddress::Any, std::stoi(Config::config().get("port")));
    connect(this->server, SIGNAL(newConnection()), this, SLOT(onAcceptConnection()));
}

void TCPServer::onAcceptConnection() {
    while (server->hasPendingConnections()) {
        Storage& storage = Storage::storage();
        QTcpSocket* connection = this->server->nextPendingConnection();
        std::string address = connection->peerAddress().toString().toStdString();

        if (!storage.contactExists(address)) {
            storage.addContact(new Contact(address, address, std::stoi(Config::config().get("port"))));
        }

        Contact* contact = storage.getContact(address);
        contacts[connection] = contact;

        connect(connection, SIGNAL(onReadyRead()), SLOT(onReadyRead()));
        connect(connection, SIGNAL(disconnected()), SLOT(onDisconnected()));

        emit connected(contact);
    }
}

void TCPServer::onDisconnected() {
    QTcpSocket* socket = (QTcpSocket*) sender();
    Contact* contact = contacts[socket];
    emit disconnected(contact);
}

void TCPServer::onReadyRead() {

    QTcpSocket* socket = (QTcpSocket*) sender();
    std::string content{};

    // read message content
    while (socket->bytesAvailable() > 0) {
        content.append(socket->readAll().toStdString());
    }

    Contact* contact = contacts[socket];
    emit recieved(contact, TCPPacket::decode(content));
}


