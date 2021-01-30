#include <TCPConnection.h>

void TCPConnection::startServer(short port) {
    server = new TCPServer(log, port);

    connect(server, SIGNAL(connected(string, short)), SIGNAL(connected(string, short)));
    connect(server, SIGNAL(disconnected(string)), SIGNAL(disconnected(string)));
    connect(server, SIGNAL(recieved(string, TCPPacket)), SIGNAL(recieved(string, TCPPacket)));
}

TCPClient* TCPConnection::registerClient(string ip, short port) {
    TCPClient* client = new TCPClient(log, ip, port);

    clients[ip] = client;
    connect(client, SIGNAL(failed(string, TCPException)), SIGNAL(sendingError(string, TCPException)));
    connect(client, SIGNAL(connected(string, short)), SIGNAL(connected(string, short)));
    connect(client, SIGNAL(disconnected(string)), SIGNAL(disconnected(string)));

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
