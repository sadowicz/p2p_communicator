#include <contacts/ContactController.h>

using namespace contacts;

ContactController::ContactController(Logger& log) : log(log) {
    connection = new TCPConnection(log);
    connect(connection, SIGNAL(connected(string, short)), SLOT(onConnect(string,short)));
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
    Storage::storage().getContact(ip)->addToHistory(Message::createTextMessage(message));
    Storage::storage().save();
    connection->send(ip, TCPPacket::encode(TCPPacket::PacketType::TEXT, "", message));
}

void ContactController::onConnect(const string ip, short port) {
    if (!Storage::storage().contactExists(ip)) {
        log.info("Adding new contact: " + ip + ":" + std::to_string(port));

        Contact* newContact = new Contact(ip, ip, port);
        Storage::storage().addContact(newContact);
    }

    if (Storage::storage().getContact(ip)->isActive() == false) {
        Storage::storage().getContact(ip)->setActiveState(true);
        emit contactStatusChanged();
    }
}

void ContactController::tryConnect(const string& ip) {
    // dont reconnect if contact is already active
    if (!isActive(ip)) {
        connection->reconnect(ip);
    }
}

void ContactController::forceDisconnect(const string& ip) {
    connection->closeConnection(ip);
}

bool ContactController::isActive(const string& ip) {
    return Storage::storage().getContact(ip)->isActive();
}

void ContactController::onDisconnect(const string ip) {
    if (Storage::storage().contactExists(ip)) {
        Storage::storage().getContact(ip)->setActiveState(false);
        emit contactStatusChanged();
    }
}

void ContactController::onSendError(const string ip, TCPException e) {
    log.error("Error sending message to: " + ip + ", reason: " + e.what());
    // TODO: handle error here
}

void ContactController::onRecieve(const string ip, TCPPacket packet) {
    log.debug("Recieved message from: " + ip + ", content: " + packet.getContent());

    Storage::storage().getContact(ip)->addToHistory(new Message(packet));
    Storage::storage().save();

    Storage::storage().getContact(ip)->setUnreadMsgState(true);
    emit contactStatusChanged();
}
