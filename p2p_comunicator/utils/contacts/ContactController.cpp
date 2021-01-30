#include <contacts/ContactController.h>

using namespace contacts;

ContactController::ContactController() {
    connection = new TCPConnection();
    connect(connection, SIGNAL(connected(string, short)), SLOT(onConnect(string,short)));
    connect(connection, SIGNAL(disconnected(string)), SLOT(onDisconnect(string)));
    connect(connection, SIGNAL(recieved(string,TCPPacket)), SLOT(onRecieve(string,TCPPacket)));

    connection->startServer(std::stoi(Config::config("port")));

    // load all saved contacts and try to connect to them
    for (auto& entry : Storage::storage().getContacts()) {
        Contact* contact = entry.second;
        connection->registerClient(contact->getAddress(), contact->getPort())->tryConnect();
    }
}

void ContactController::addContact(Contact* contact) {
    Storage::storage().addContact(contact);
    connection->registerClient(contact->getAddress(), contact->getPort());
}

void ContactController::removeContact(string& ip) {
    Storage::storage().deleteContact(ip);
    connection->closeConnection(ip);
}

void ContactController::editContact(Contact* editedContact) {
    Contact* oldContact = Storage::storage().getContact(editedContact->getAddress());

    std::vector<Message> history = oldContact->getHistory();

    for(Message msg : history) {
        editedContact->addToHistory(msg);
    }

    if (editedContact->getPort() == oldContact->getPort()) {
        Storage::storage().deleteContact(editedContact->getAddress());
        Storage::storage().addContact(editedContact);
    } else {
        // if port was changed client be recreated and try connecting again
        removeContact(editedContact->getAddress());
        addContact(editedContact);
        tryConnect(editedContact->getAddress());
    }
}

void ContactController::send(Contact* contact, string& message) {
    connection->send(contact->getAddress(), message);
}

void ContactController::onConnect(string ip, short port) {
    Logger::log().info("Adding new contact: " + ip + ":" + std::to_string(port));

    if (!Storage::storage().contactExists(ip)) {
        Contact* newContact = new Contact(ip, ip, port);
        Storage::storage().addContact(newContact);
    }

    Storage::storage().getContact(ip)->setActiveState(true);
}

void ContactController::tryConnect(string& ip) {
    connection->reconnect(ip);
}

void ContactController::forceDisconnect(string& ip) {
    connection->closeConnection(ip);
}

bool ContactController::isActive(string& ip) {
    return Storage::storage().getContact(ip)->isActive();
}

void ContactController::onDisconnect(string ip) {
    Contact* contact = Storage::storage().getContact(ip);
    contact->setActiveState(false);
}

void ContactController::onSendError(string ip, TCPException e) {
    Logger::log().error("Error sending message to: " + ip + ", reason: " + e.what());
    // TODO: handle error here
}

void ContactController::onRecieve(string ip, TCPPacket packet) {
    Logger::log().debug("Recieved message from: " + ip);
    // TODO: add message to history and such
}
