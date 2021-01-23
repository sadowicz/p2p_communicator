#include <tcp/TCPServer.h>

TCPServer::TCPServer(Storage& storage) : storage(storage) {
    this->server = new QTcpServer(this);
    int port = std::stoi(Config::get("port"));
    this->server->listen(QHostAddress::Any, (quint16) port);
    connect(this->server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void TCPServer::acceptConnection() {
    QTcpSocket* connection = this->server->nextPendingConnection();
    std::string address = connection->peerAddress().toString().toStdString();
    std::string content = connection->readAll().toStdString();
    unsigned short port = connection->peerPort();

    if (!storage.contactExists(address)) {
        // save new contact
        storage.addContact(Contact(address, address, port));
    }
    Contact& contact = storage.getContact(address);

    // read message content
    TCPPacket* packet = TCPPacket::decode(content);
    Message msg(packet);
    contact.addToHistory(msg);

    connection->close();
    delete packet;
    delete connection;
}
