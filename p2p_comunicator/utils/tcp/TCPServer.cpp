#include <tcp/TCPServer.h>

TCPServer::TCPServer(Storage& storage) : storage(storage) {
    this->server = new QTcpServer(this);
    int port = std::stoi(Config::get("port"));
    this->server->listen(QHostAddress::Any, (quint16) port);
    connect(this->server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void TCPServer::acceptConnection() {
    QTcpSocket* connection = this->server->nextPendingConnection();
    QString address = connection->peerAddress().toString();
    unsigned short port = connection->peerPort();
    QByteArray content = connection->readAll();

    Contact* contact = storage.getContacts()[address];
    if (nullptr == contact) {
        // save new contact
        contact = new Contact(address, address, port);
        storage.addContact(contact);
    }

    // read message content
    TCPPacket* packet = TCPPacket::decode(content.toStdString());


    connection->close();
    delete connection;
}
