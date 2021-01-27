#include <tcp/TCPConnection.h>

TCPConnection* TCPConnection::connection = nullptr;

void TCPConnection::init() {
    Storage& storage = Storage::storage();
    connection = new TCPConnection;
    connection->server = new TCPServer();

    connect(connection->server, SIGNAL(connected(Contact*)), connection, SIGNAL(connected(Contact*)));
    connect(connection->server, SIGNAL(disconnected(Contact*)), connection, SIGNAL(disconnected(Contact*)));
    connect(connection->server, SIGNAL(recieved(Contact*, TCPPacket)), connection, SIGNAL(recieved(Contact*, TCPPacket)));

    for (pair<string, Contact> contact : storage.getContacts()) {
        TCPClient* client = new TCPClient(contact.second);

        connection->clients[contact.first] = client;
        connect(client, SIGNAL(failed(Contact*, TCPException)), connection, SIGNAL(sendingError(Contact*, TCPException)));
        connect(client, SIGNAL(connected(Contact*)), connection, SIGNAL(connected(Contact*)));
        connect(client, SIGNAL(disconnected(Contact*)), connection, SIGNAL(disconnected(Contact*)));
    }
}

void TCPConnection::send(string& ip, string& content) {
    clients[ip]->send(content);
}
