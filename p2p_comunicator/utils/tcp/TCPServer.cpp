#include <tcp/TCPServer.h>

TCPServer::TCPServer(Storage& storage) : storage(storage) {
    this->server = new QTcpServer(this);
    this->server->listen(QHostAddress::Any, std::stoi(Config::get("port")));
    connect(this->server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void TCPServer::acceptConnection() {
    while (server->hasPendingConnections()) {
        QTcpSocket* connection = this->server->nextPendingConnection();
        connect(connection, SIGNAL(readyRead()), SLOT(readyRead()));
    }
}

void TCPServer::readyRead() {

    QTcpSocket* socket = (QTcpSocket*) sender();
    std::string content{};

    // read message content
    while (socket->bytesAvailable() > 0) {
        content.append(socket->readAll().toStdString());
    }

    std::string address = socket->peerAddress().toString().toStdString();

    if (!storage.contactExists(address)) {
       // save new contact
        storage.addContact(Contact(address, address, std::stoi(Config::get("port"))));
    }
    Contact& contact = storage.getContact(address);

    // add message to contact's message history
    Message msg = Message(TCPPacket::decode(content)).withAddress(&contact.getAddress());
    contact.addToHistory(msg);

    // TODO: notify user that a message has been recieved?

    socket->close();
    delete socket;
}


