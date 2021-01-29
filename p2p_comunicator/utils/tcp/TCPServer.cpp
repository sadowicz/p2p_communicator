#include <tcp/TCPServer.h>

TCPServer::TCPServer() {
    this->server = new QTcpServer(this);
    this->server->listen(QHostAddress::Any, std::stoi(Config::config().get("port")));
    Logger::log().info("Server listening on port " + Config::config().get("port"));

    connect(this->server, SIGNAL(newConnection()), this, SLOT(onAcceptConnection()));
}

void TCPServer::onAcceptConnection() {
    while (server->hasPendingConnections()) {
        Storage& storage = Storage::storage();
        QTcpSocket* connection = this->server->nextPendingConnection();
        std::string address = connection->peerAddress().toString().toStdString();
        short port = connection->peerPort();

        Logger::log().debug("Server accepting connection from: " + address);

        if (!storage.contactExists(address)) {
            Logger::log().debug("Server connected to a new contact: " + address + ":" + std::to_string(port));
            storage.addContact(new Contact(address, address, port));
        }

        Contact* contact = storage.getContact(address);
        contacts[connection] = contact;

        connect(connection, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(connection, SIGNAL(disconnected()), this,  SLOT(onDisconnected()));

        emit connected(contact);
    }
}

void TCPServer::onDisconnected() {
    QTcpSocket* socket = (QTcpSocket*) sender();
    Contact* contact = contacts[socket];
    Logger::log().debug("Server disconnected from: " + contact->getAddress());

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
    Logger::log().debug("Server recieved a message from: " + contact->getAddress());

    emit recieved(contact, TCPPacket::decode(content));
}


