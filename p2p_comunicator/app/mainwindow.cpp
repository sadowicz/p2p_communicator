#include "mainwindow.h"

/*TODO:
    - two more packet types FILE-REQUEST and NEW-CONTACT
    - state to locked after emieting error form constructor (loadContacts() method)
    - improve TCPPacket encoding: [packetType:1][filenameLength:1][filename:filenameLength][contentLength:4][content:contentLength]
    - test if sending messages works
    - implement sending files
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpStateMachine();
    stateMachine->start();

    connect(this, SIGNAL(error(QString)), this, SLOT(on_error(QString)));

    loadContacts();
    loadListItems();

    log = Logger(Config::config("log-file"), Config::config().debugMode());
    log.info("------------ App started ------------");
    contactController = new ContactController(log);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpStateMachine()
{
    stateMachine = new QStateMachine{this};

    Unlocked = new QState{stateMachine};

    History = new QHistoryState{Unlocked};
    Disconnected = new QState{Unlocked};
    Connected = new QState{Unlocked};
    ValidateSendable = new QState{Unlocked};
    Sendable = new QState{Unlocked};

    Locked = new QState{stateMachine};

    assignStatesProperties();
    setStatesTransistions();

    History->setDefaultState(Disconnected);
    Unlocked->setInitialState(History);
    stateMachine->setInitialState(Unlocked);
}

void MainWindow::assignStatesProperties()
{
    Unlocked->assignProperty(ui->pbNewContact, "enabled", true);
    Unlocked->assignProperty(ui->pbSend, "enabled", true);
    Unlocked->assignProperty(ui->pbAttachFile, "enabled", true);
    Unlocked->assignProperty(ui->lwContacts, "enabled", true);
    Unlocked->assignProperty(ui->teChat, "enabled", true);
    Unlocked->assignProperty(ui->teSend, "enabled", true);
    Unlocked->assignProperty(ui->pbDeleteContact, "enabled", false);
    Unlocked->assignProperty(ui->pbEditContact, "enabled", false);

    Disconnected->assignProperty(ui->pbNewContact, "enabled", true);
    Disconnected->assignProperty(ui->pbSend, "enabled", false);
    Disconnected->assignProperty(ui->pbAttachFile, "enabled", false);
    Disconnected->assignProperty(ui->lwContacts, "enabled", true);
    Disconnected->assignProperty(ui->teChat, "enabled", false);
    Disconnected->assignProperty(ui->teSend, "enabled", false);
    Disconnected->assignProperty(ui->pbDeleteContact, "enabled", false);
    Disconnected->assignProperty(ui->pbEditContact, "enabled", false);

    Connected->assignProperty(ui->pbNewContact, "enabled", true);
    Connected->assignProperty(ui->pbSend, "enabled", false);
    Connected->assignProperty(ui->pbAttachFile, "enabled", true);
    Connected->assignProperty(ui->lwContacts, "enabled", true);
    Connected->assignProperty(ui->teChat, "enabled", true);
    Connected->assignProperty(ui->teSend, "enabled", true);
    Connected->assignProperty(ui->pbDeleteContact, "enabled", false);
    Connected->assignProperty(ui->pbEditContact, "enabled", false);

    Sendable->assignProperty(ui->pbNewContact, "enabled", true);
    Sendable->assignProperty(ui->pbSend, "enabled", true);
    Sendable->assignProperty(ui->pbAttachFile, "enabled", true);
    Sendable->assignProperty(ui->lwContacts, "enabled", true);
    Sendable->assignProperty(ui->teChat, "enabled", true);
    Sendable->assignProperty(ui->teSend, "enabled", true);
    Sendable->assignProperty(ui->pbDeleteContact, "enabled", false);
    Sendable->assignProperty(ui->pbEditContact, "enabled", false);

    Locked->assignProperty(ui->pbNewContact, "enabled", false);
    Locked->assignProperty(ui->pbSend, "enabled", false);
    Locked->assignProperty(ui->pbAttachFile, "enabled", false);
    Locked->assignProperty(ui->lwContacts, "enabled", false);
    Locked->assignProperty(ui->teChat, "enabled", false);
    Locked->assignProperty(ui->teSend, "enabled", false);
    Locked->assignProperty(ui->pbDeleteContact, "enabled", false);
    Locked->assignProperty(ui->pbEditContact, "enabled", false);
}

void MainWindow::setStatesTransistions()
{
    Unlocked->addTransition(ui->pbNewContact, SIGNAL(clicked()), Locked);
    Unlocked->addTransition(this, SIGNAL(error(QString)), Locked);

    Disconnected->addTransition(ui->lwContacts, SIGNAL(itemClicked(QListWidgetItem*)), Connected);

    Connected->addTransition(ui->teSend, SIGNAL(textChanged()), ValidateSendable);

    connect(ValidateSendable, SIGNAL(entered()), this, SLOT(on_validateSendable()));
    ValidateSendable->addTransition(this, SIGNAL(msgSendable()), Sendable);
    ValidateSendable->addTransition(this, SIGNAL(msgUnsendable()), Connected);

    Sendable->addTransition(ui->teSend, SIGNAL(textChanged()), ValidateSendable);

    Locked->addTransition(this, SIGNAL(contactAdded()), Connected);
    Locked->addTransition(this, SIGNAL(contactAdditionCanceled()), Unlocked);
    Locked->addTransition(this, SIGNAL(errorCatched()), Unlocked);
}

void MainWindow::loadContacts()
{
    Storage& storage = Storage::storage();
    if(storage.load())
    {
        for(auto& contact : storage.getContacts())
        {
            contacts.insert({contact.second->getName(), contact.second});
        }
    }
    else
        emit error("Unable to load contacts data.");
}

void MainWindow::loadListItems()
{
    for(auto& contact : contacts)
    {
        new QListWidgetItem(contact.first.c_str(), ui->lwContacts);
    }
}

void MainWindow::on_pbNewContact_clicked()
{
    addContactWin = new AddContactWindow{this};
    addContactWin->show();
}

void MainWindow::on_contactEditSuccess(Contact* contact) {
    // edit contact
    contactController->editContact(contact);

    // refresh GUI
    refreshContactsList();

    emit contactAdded();
}

void MainWindow::on_contactAddSuccess(Contact* newContact) {
    // add contact to storage and try to connect to it
    contactController->addContact(newContact);
    contactController->tryConnect(newContact->getAddress());

    // refresh gui list
    refreshContactsList();

    emit contactAdded();
}

void MainWindow::refreshContactsList() {
    contacts.clear();
    ui->lwContacts->clear();
    loadContacts();
    loadListItems();
}

void MainWindow::on_contactAddCancel()
{
    emit contactAdditionCanceled();
}

void MainWindow::on_error(QString errorMessage)
{
    errWin = new ErrorWindow{this, errorMessage};
    errWin->show();
}

void MainWindow::on_errorRead()
{
    emit errorCatched();
}

void MainWindow::on_validateSendable()
{
    if(ui->teSend->toPlainText().isEmpty())
        emit msgUnsendable();
    else
        emit msgSendable();
}

void MainWindow::on_lwContacts_itemClicked(QListWidgetItem *item)
{
    activeContact = contacts[item->text().toStdString()];
    ui->pbDeleteContact->setEnabled(true);
    ui->pbEditContact->setEnabled(true);

    // try connecting to contact if it's inactive
    if (!activeContact->isActive()) {
        contactController->tryConnect(activeContact->getAddress());
    }
}

void MainWindow::on_pbDeleteContact_clicked()
{
    // remove contact from storage and close TCP/IP connection
    contactController->removeContact(activeContact->getAddress());

    // refresh GUI
    refreshContactsList();

    ui->pbDeleteContact->setEnabled(false);
    ui->pbEditContact->setEnabled(false);
}

void MainWindow::on_pbEditContact_clicked()
{
    editContactWin = new EditContactWindow{this};
    editContactWin->show();

    emit edited(activeContact->getAddress(), activeContact->getName(), activeContact->getPort());
}

void MainWindow::on_pbSettings_clicked()
{
    settingsWin = new SettingsWindow{this};
    settingsWin->show();
}

void MainWindow::on_pbSend_clicked() {
    std::string packet = TCPPacket::encode(TCPPacket::PacketType::TEXT, "", ui->teSend->toPlainText().toStdString());
    contactController->send(activeContact->getAddress(), packet);
}
