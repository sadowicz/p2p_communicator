#include <tcp/TCPConnection.h>

TCPConnection& TCPConnection::get() {
    static TCPConnection connection{};
    return connection;
}

TCPConnection::TCPConnection() {
    Storage& storage = Storage::storage();
    server = new TCPServer();

    connect(server, SIGNAL(connected(Contact*)), SIGNAL(connected(Contact*)));
    connect(server, SIGNAL(disconnected(Contact*)), SIGNAL(disconnected(Contact*)));
    connect(server, SIGNAL(recieved(Contact*, TCPPacket)), SIGNAL(recieved(Contact*, TCPPacket)));

    for (auto& contact : storage.getContacts()) {
        TCPClient* newClient = addClient(contact.second);
        newClient->tryConnect();
    }
}

TCPClient* TCPConnection::addClient(Contact* contact) {
    TCPClient* client = new TCPClient(contact);

    clients[contact->getAddress()] = client;
    connect(client, SIGNAL(failed(Contact*, TCPException)), SIGNAL(sendingError(Contact*, TCPException)));
    connect(client, SIGNAL(connected(Contact*)), SIGNAL(connected(Contact*)));
    connect(client, SIGNAL(disconnected(Contact*)), SIGNAL(disconnected(Contact*)));

    return client;
}

void TCPConnection::closeConnection(string& ip) {
    /* TODO: client gets disconnected but server is still connected,
     *       the next message recieved from the contact will not be
     *       registered as a new contact request, the message won't be
     *       saved and the app will crash
     *
     * SOLUTION 1: send a "disconnect" request to the contact, which will make
     *             them close their client -> server connection
     * */

    TCPClient* client = clients[ip];
    client->forceDisconnect();
    delete client;
    clients.erase(ip);
}

void TCPConnection::reconnect(string& ip) {
    clients[ip]->tryConnect();
}

void TCPConnection::send(string& ip, string& content) {
    clients[ip]->send(content);
}
