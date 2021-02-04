#include "mainwindow.h"

/*TODO:
    - FILE_REQUEST package type
    - state to locked after emieting error form constructor (loadContacts() method)
    - improve TCPPacket encoding: [packetType:1][filenameLength:1][filename:filenameLength][contentLength:4][content:contentLength]
    - implement sending files

    - @Filip: sortowanie listy piotrek zrobił, wyłączanie czatu też naprawił, da się zrobić żeby się
              kontakt nie pogrubiał jak mam otwarty z nim czat?
    - [OK] @Tomasz: crash jak się usunie kontakt do którego jestem podłączony
    - @Tomasz: jak się zmieni nazwę kontaktu to się rozłącza ale dalej można wysyłać wiadomości
    - @Tomasz: kontakt niekatywny się odznacza jak kliknę na niego
    - [OK] @Tomasz: kontakt niekatywny się odznacza jak kliknę na niego
    - [OK] @Tomasz: nie da się przesyłać wiadomości długości więcej niż około 50k znaków (nie powinno się dać, dodać walidację)
    - [OK] @Tomasz: nie powinno się dać pisać do kontaktu który jest nieaktywny
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

    this->messageListDelegate = new MessageListDelegate;
    ui->msgListView->setItemDelegate(this->messageListDelegate);

    log = util::getLogger();
    log.info("------------ App started ------------");
    contactController = new ContactController(log);

    connect(this, &MainWindow::fileReady, contactController, &ContactController::onFileReady);
    connect(this, &MainWindow::fileRemoved, contactController, &ContactController::onFileCancelled);
    connect(contactController, &ContactController::refreshContactList, this, &MainWindow::on_refreshContactsList);
    connect(this, SIGNAL(msgRead(const string)), contactController, SLOT(onMsgRead(const string)));

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
    Unlocked->assignProperty(ui->teSend, "enabled", true);
    Unlocked->assignProperty(ui->lbAttachFile, "enabled", true);
    Unlocked->assignProperty(ui->pbDeleteContact, "enabled", false);
    Unlocked->assignProperty(ui->pbEditContact, "enabled", false);

    Disconnected->assignProperty(ui->pbNewContact, "enabled", true);
    Disconnected->assignProperty(ui->pbSend, "enabled", false);
    Disconnected->assignProperty(ui->pbAttachFile, "enabled", false);
    Disconnected->assignProperty(ui->lwContacts, "enabled", true);
    Disconnected->assignProperty(ui->teSend, "enabled", false);
    Disconnected->assignProperty(ui->lbAttachFile, "enabled", false);
    Disconnected->assignProperty(ui->pbDeleteContact, "enabled", false);
    Disconnected->assignProperty(ui->pbEditContact, "enabled", false);

    Connected->assignProperty(ui->pbNewContact, "enabled", true);
    Connected->assignProperty(ui->pbSend, "enabled", false);
    Connected->assignProperty(ui->pbAttachFile, "enabled", true);
    Connected->assignProperty(ui->lwContacts, "enabled", true);
    Connected->assignProperty(ui->teSend, "enabled", true);
    Connected->assignProperty(ui->lbAttachFile, "enabled", true);
    Connected->assignProperty(ui->pbDeleteContact, "enabled", false);
    Connected->assignProperty(ui->pbEditContact, "enabled", false);

    Sendable->assignProperty(ui->pbNewContact, "enabled", true);
    Sendable->assignProperty(ui->pbSend, "enabled", true);
    Sendable->assignProperty(ui->pbAttachFile, "enabled", true);
    Sendable->assignProperty(ui->lwContacts, "enabled", true);
    Sendable->assignProperty(ui->teSend, "enabled", true);
    Sendable->assignProperty(ui->lbAttachFile, "enabled", true);
    Sendable->assignProperty(ui->pbDeleteContact, "enabled", false);
    Sendable->assignProperty(ui->pbEditContact, "enabled", false);

    Locked->assignProperty(ui->pbNewContact, "enabled", false);
    Locked->assignProperty(ui->pbSend, "enabled", false);
    Locked->assignProperty(ui->pbAttachFile, "enabled", false);
    Locked->assignProperty(ui->lwContacts, "enabled", false);
    Locked->assignProperty(ui->teSend, "enabled", false);
    Locked->assignProperty(ui->lbAttachFile, "enabled", false);
    Locked->assignProperty(ui->pbDeleteContact, "enabled", false);
    Locked->assignProperty(ui->pbEditContact, "enabled", false);
}

void MainWindow::setStatesTransistions()
{
    Unlocked->addTransition(ui->pbNewContact, SIGNAL(clicked()), Locked);
    Unlocked->addTransition(this, SIGNAL(error(QString)), Locked);

    Disconnected->addTransition(this, SIGNAL(contactConnected()), Connected);

    Connected->addTransition(this, SIGNAL(contactDisconnected()), Disconnected);
    Connected->addTransition(ui->teSend, SIGNAL(textChanged()), ValidateSendable);
    Connected->addTransition(this, SIGNAL(fileChanged()), ValidateSendable);

    connect(ValidateSendable, SIGNAL(entered()), this, SLOT(on_validateSendable()));
    ValidateSendable->addTransition(this, SIGNAL(msgSendable()), Sendable);
    ValidateSendable->addTransition(this, SIGNAL(msgUnsendable()), Connected);

    Sendable->addTransition(this, SIGNAL(fileChanged()), ValidateSendable);
    Sendable->addTransition(ui->teSend, SIGNAL(textChanged()), ValidateSendable);

    Locked->addTransition(this, SIGNAL(contactAdded()), Unlocked);
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
    log.debug("Loading contact list items:");

    for(auto& contact : contacts)
    {
        auto loaded = new QListWidgetItem(contact.first.c_str(), ui->lwContacts);

        if(contact.second->isActive() == true) loaded->setTextColor(Qt::darkGreen);
        else if(contact.second->isActive() == false) loaded->setTextColor(Qt::darkRed);

        if(contact.second->hasUnreadMsg() == true)
        {
            QFont font{};
            font.setBold(true);
            loaded->setFont(font);
        }

        log.debug("\t> " + contact.first +
                  "\tactive : " + std::to_string(contact.second->isActive()) +
                  "\tunread : " + std::to_string(contact.second->hasUnreadMsg()));
    }

    ui->lwContacts->sortItems();

    if(!activeContact.empty() && Storage::storage().contactExists(activeContact)){
        auto contact = Storage::storage().getContact(activeContact);
        ui->msgListView->setModel(contact);
        connect(contact, &Contact::onHistoryChange, this, &MainWindow::onMessageListChange);
    }
}

void MainWindow::on_pbNewContact_clicked()
{
    addContactWin = new AddContactWindow{this};
    addContactWin->show();
}

void MainWindow::on_contactEditSuccess(std::string ip, std::string name, unsigned int port) {
    // edit contact
    contactController->editContact(ip, name, port);

    // refresh GUI
    refreshContactsList();

    emit contactAdded();
}

void MainWindow::on_contactAddSuccess(Contact* newContact) {
    // add contact to storage and try to connect to it
    contactController->addContact(newContact);
    contactController->tryConnect(newContact->getAddress());

    // refresh GUI
    refreshContactsList();

    emit contactAdded();
}

void MainWindow::on_refreshContactsList() {
    refreshContactsList();
}

void MainWindow::refreshContactsList() {
    contacts.clear();
    ui->lwContacts->clear();
    loadContacts();
    loadListItems();

    reselectContact();
}

void MainWindow::reselectContact() {
    Storage& storage = Storage::storage();
    if (!storage.contactExists(activeContact)) {
        return;
    }
    std::string name = storage.getContact(activeContact)->getName();

    QList<QListWidgetItem*> selectedItems = ui->lwContacts->findItems(QString::fromStdString(name), Qt::MatchWrap);
    if (selectedItems.size() != 1) {
        return;
    }
    QListWidgetItem* selectedItem = selectedItems.at(0);
    QList<QListWidgetItem*> allItems = ui->lwContacts->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    size_t index = std::distance(allItems.begin(), std::find(allItems.begin(), allItems.end(), selectedItem));
    ui->lwContacts->selectionModel()->select(ui->lwContacts->model()->index(index, 0), QItemSelectionModel::Select);
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
    if(ui->teSend->toPlainText().isEmpty() && ui->lbAttachFile->text().isEmpty())
        emit msgUnsendable();
    else
        emit msgSendable();

}

void MainWindow::on_lwContacts_itemClicked(QListWidgetItem *item)
{
    if(Storage::storage().contactExists(activeContact)){
        disconnect(Storage::storage().getContact(activeContact), &Contact::onHistoryChange,
                   this, &MainWindow::onMessageListChange);
    }

    Contact* contact =  contacts[item->text().toStdString()];
    activeContact = contact->getAddress();

    connect(contact, &Contact::onHistoryChange, this, &MainWindow::onMessageListChange);

    ui->pbDeleteContact->setEnabled(true);
    ui->pbEditContact->setEnabled(true);

    ui->msgListView->setModel(contact);
    ui->msgListView->scrollToBottom();

    // try connecting to contact if it's inactive
    if (!contact->isActive()) {
        emit contactDisconnected();
        contactController->tryConnect(contact->getAddress());
    }
    else
        emit contactConnected();

    if(contact->hasUnreadMsg() == true) {
        emit msgRead(contact->getAddress());
    }
}

void MainWindow::on_pbDeleteContact_clicked()
{
    // remove contact from storage and close TCP/IP connection
    contactController->removeContact(activeContact);

    // refresh GUI
    refreshContactsList();

    ui->pbDeleteContact->setEnabled(false);
    ui->pbEditContact->setEnabled(false);

    emit contactDisconnected();
}

void MainWindow::on_pbEditContact_clicked()
{
    Contact* contact = Storage::storage().getContact(activeContact);
    editContactWin = new EditContactWindow{contact->getAddress(), contact->getName(), static_cast<int>(contact->getPort()), this};
    editContactWin->show();
}

void MainWindow::on_pbSettings_clicked()
{
    settingsWin = new SettingsWindow{this};
    settingsWin->show();
}

void MainWindow::on_pbSend_clicked() {
    Contact* contact = Storage::storage().getContact(activeContact);
    contactController->sendMessage(contact->getAddress(), ui->teSend->toPlainText().toStdString());
}

void MainWindow::on_pbAttachFile_clicked()
{
    if(ui->pbAttachFile->text() == "Attach File"){
        attachFile();
    }else if(ui->pbAttachFile->text() == "Remove"){
        removeFile();
    }
}

void MainWindow::attachFile(){
    QString filePath = QFileDialog::getOpenFileName(this, "Open file", QDir::homePath());
    QFile file(filePath);

    if(filePath == "")
        return;

    if(!file.open(QIODevice::ReadOnly)){
        log.error(filePath.prepend("Failed to open File: ").toStdString());
        emit error(file.fileName().prepend("Failed to open: "));
        return;
    }

    // load bytes and get file name
    qint64 size = file.size();
    std::string* fileContent = new std::string;
    *fileContent = file.readAll().toStdString();

    if(size < static_cast<int>(fileContent->size()))
    {
        log.error(filePath.prepend("Failed to read File: ").toStdString());
        emit error(file.fileName().prepend("Failed to open: "));
        return;
    }

    QString fileName = QFileInfo(file).fileName();

    log.info(filePath.prepend("File ready to send: ").toStdString());
    ui->lbAttachFile->setText(fileName);
    ui->pbAttachFile->setText("Remove");

    emit fileChanged();
    emit fileReady(fileName.toStdString(), fileContent);
}

void MainWindow::removeFile(){

    ui->lbAttachFile->setText("");
    ui->pbAttachFile->setText("Attach File");

    emit fileChanged();
    emit fileRemoved();
}

void MainWindow::onMessageListChange(){
    ui->msgListView->scrollToBottom();
}
