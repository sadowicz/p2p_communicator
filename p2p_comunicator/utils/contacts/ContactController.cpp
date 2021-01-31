#include <contacts/ContactController.h>

#define MAX_TEXT_MSG_LENGTH 2048

using namespace contacts;

ContactController::ContactController(Logger& log) : log(log) {
    connection = new TCPConnection(log);
    connect(connection, SIGNAL(connected(string, unsigned int)), SLOT(onConnect(string, unsigned int)));
    connect(connection, SIGNAL(disconnected(string)), SLOT(onDisconnect(string)));
    connect(connection, SIGNAL(recieved(string, TCPPacket)), SLOT(onRecieve(string, TCPPacket)));
    connect(connection, SIGNAL(sendingError(string, TCPException)), SLOT(onSendError(string, TCPException)));

    connection->startServer(std::stoi(Config::config("port")));

    // load all saved contacts and try to connect to them
    for (auto& entry : Storage::storage().getContacts()) {
        Contact* contact = entry.second;
        log.debug("Loaded contact: " + contact->getAddress() + ":" + std::to_string(contact->getPort()) + ", trying to connect...");
        connection->registerClient(contact->getAddress(), contact->getPort())->tryConnect();
    }
}

void ContactController::addContact(Contact* contact) {
    Storage::storage().addContact(contact);
    connection->registerClient(contact->getAddress(), contact->getPort());
}

void ContactController::removeContact(const string& ip) {
    Storage::storage().deleteContact(ip);
    connection->closeConnection(ip);
}

void ContactController::editContact(Contact* editedContact) {
    Contact* oldContact = Storage::storage().getContact(editedContact->getAddress());

    std::vector<Message*> history = oldContact->getHistory();

    for(Message* msg : history) {
        editedContact->addToHistory(msg);
    }

    if (editedContact->getPort() == oldContact->getPort()) {
        Storage::storage().deleteContact(editedContact->getAddress());
        Storage::storage().addContact(editedContact);
    } else {
        // if port was changed client should be recreated and try connecting again
        removeContact(editedContact->getAddress());
        addContact(editedContact);
        tryConnect(editedContact->getAddress());
    }
}

void ContactController::sendTextMessage(const string& ip, const string& message) {
    if (connection->isClientConnected(ip)) {
        if (message.size() > MAX_TEXT_MSG_LENGTH) {
            log.error("Message was too long, cannot send");
            return;
        }
        Storage::storage().getContact(ip)->addToHistory(Message::createTextMessage(message));
        Storage::storage().save();
        connection->send(ip, TCPPacket::encode(TCPPacket::PacketType::TEXT, "", message));
    } else {
        log.error("Client was not connected, can't send message");
    }
}

void ContactController::onConnect(const string ip, unsigned int port) {
    if (!Storage::storage().contactExists(ip)) {
        log.info("Adding new contact: " + ip + ":" + std::to_string(port));

        Contact* newContact = new Contact(ip, ip, port);
        Storage::storage().addContact(newContact);
        emit refreshContactList();
    }

    if (Storage::storage().getContact(ip)->isActive() == false) {
        Storage::storage().getContact(ip)->setActiveState(true);
        emit refreshContactList();
    }
}

void ContactController::tryConnect(const string& ip) {
    // dont reconnect if contact is already active
    Contact* contact = Storage::storage().getContact(ip);
    if (!contact->isActive()) {
        connection->reconnect(ip);
    }
}

void ContactController::forceDisconnect(const string& ip) {
    connection->closeConnection(ip);
}

bool ContactController::isActive(const string& ip) const{
    return Storage::storage().getContact(ip)->isActive();
}

void ContactController::onDisconnect(const string ip) {
    if (Storage::storage().contactExists(ip)) {
        Storage::storage().getContact(ip)->setActiveState(false);
    }
    emit refreshContactList();
}

void ContactController::onSendError(const string ip, TCPException e) {
    log.error("Error sending message to: " + ip + ", reason: " + e.what());
    // TODO: handle error here
}

void ContactController::onRecieve(const string ip, TCPPacket packet) {
    log.debug("Recieved message from: " + ip + ", content: " + packet.getContent());

    switch(packet.getType()) {
    case TCPPacket::PacketType::TEXT:           onTextMessage(ip, packet); break;
    case TCPPacket::PacketType::FILE:           onFileMessage(ip, packet); break;
    case TCPPacket::PacketType::CONNECTION:    onConnectMessage(ip, packet); break;
    }
}

void ContactController::onTextMessage(const string ip, TCPPacket packet) {
    Storage::storage().getContact(ip)->addToHistory(new Message(packet));
    Storage::storage().getContact(ip)->setUnreadMsgState(true);
    Storage::storage().save();
    emit refreshContactList();
}

void ContactController::onFileMessage(const string ip, TCPPacket packet) {
    // TODO
}

void ContactController::onConnectMessage(const string ip, TCPPacket packet) {
    // connection attempt from a contact

    if (Storage::storage().contactExists(ip)) {
        Contact* old = Storage::storage().getContact(ip);
        unsigned int port = std::stoul(packet.getContent());

        /* If saved port is different from what was sent -> replace the contacts port.
           This is done because on connecting, QTcpSocket chooses a random port for itself
           and connects to the server through it, then it's aviable through socket->peerPort(),
           but since we need the other server's port to connect to, the peer's server's port
           is sent in a TCP/IP packet right after connection.
        */
        if (old->getPort() != port) {
            log.debug("Changing port of existing contact: " + ip + " to: " + std::to_string(port));
            old->setPort(port);
            Storage::storage().save();
            connection->registerClient(ip, port);
        }
    }

    // if client is not connected (can't send) -> try connecting
    if (!connection->isClientConnected(ip)) {
        connection->reconnect(ip);
    }
}

void ContactController::onMsgRead(const string ip) {
    log.debug("Read message from: " + ip);
    Storage::storage().getContact(ip)->setUnreadMsgState(false);
    emit refreshContactList();
}
