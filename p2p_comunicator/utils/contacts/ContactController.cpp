#include <contacts/ContactController.h>

#define MAX_TEXT_MSG_LENGTH 2048

using namespace contacts;

ContactController::ContactController(Logger& log) : log(log), fileIsReady(false) {
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

void ContactController::editContact(const std::string ip, std::string name, unsigned int port) {
    if (!Storage::storage().contactExists(ip)) {
        return;
    }
    Contact* contact = Storage::storage().getContact(ip);
    contact->setName(name);
    if (contact->getPort() != port) {
        // change port -> close client -> register client again -> reconnect
        contact->setPort(port);
        connection->closeConnection(ip);
        connection->registerClient(ip, port);
        connection->reconnect(ip);
    }
    Storage::storage().save();
    emit refreshContactList();
}

void ContactController::sendMessage(const string& ip, const string& message) {
    if (connection->isClientConnected(ip)) {
        if (!Storage::storage().contactExists(ip)) {
            return;
        }
        Contact* contact = Storage::storage().getContact(ip);

        // text message
        if (message.size() > 0 && message.size() <= MAX_TEXT_MSG_LENGTH) {
            contact->addToHistory(Message::createTextMessage(new string(message)));
            connection->send(ip, TCPPacket::encode(TCPPacket::PacketType::TEXT, "", message));
        }
        if (message.size() > MAX_TEXT_MSG_LENGTH) {
            emit msgTooLong();
        }

        // file message
        if (fileIsReady) {
            contact->addToHistory(Message::createFileMessage(filename));
            connection->send(ip, TCPPacket::encode(TCPPacket::PacketType::FILE, filename, *fileContent));
            // TODO: dont pass fileContent by value
        }

        Storage::storage().save();
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

    if (Storage::storage().contactExists(ip) && Storage::storage().getContact(ip)->isActive() == false) {
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

bool ContactController::contactExists(const string& ip) {
    return Storage::storage().contactExists(ip);
}

void ContactController::forceDisconnect(const string& ip) {
    connection->closeConnection(ip);
}

bool ContactController::isActive(const string& ip) const{
    return Storage::storage().contactExists(ip) && Storage::storage().getContact(ip)->isActive();
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
    case TCPPacket::PacketType::TEXT:           onTextAndFileMessage(ip, packet); break;
    case TCPPacket::PacketType::FILE:           onTextAndFileMessage(ip, packet); break;
    case TCPPacket::PacketType::CONNECTION:     onConnectMessage(ip, packet); break;
    }
}

void ContactController::onTextAndFileMessage(const string ip, TCPPacket packet) {
    if (!Storage::storage().contactExists(ip)) {
        return;
    }
    Message* msg = new Message(packet);
    Contact* contact = Storage::storage().getContact(ip);
    contact->addToHistory(msg);
    contact->setUnreadMsgState(true);
    Storage::storage().save();
    emit refreshContactList();
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

void ContactController::onFileReady(string filename, const string* content) {
    this->fileIsReady = true;
    this->filename = filename;
    this->fileContent = content;
}

void ContactController::onFileCancelled() {
    this->fileIsReady = false;
}

void ContactController::onMsgRead(const string ip) {
    log.debug("Read message from: " + ip);
    if (!Storage::storage().contactExists(ip)) {
        return;
    }
    Storage::storage().getContact(ip)->setUnreadMsgState(false);
    emit refreshContactList();
}
