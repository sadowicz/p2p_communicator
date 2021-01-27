#include "mainwindow.h"

/*TODO:
    - user should be notified when someone sends them a message (icon next to contact name?)
    - contact editing, when someone sends you a message first their name is unknown
    - two more packet types FILE-REQUEST and NEW-CONTACT
<<<<<<< HEAD
    - state to locked after emieting error form constructor (loadContacts() method)
=======
    - add "active" flag to Contact class
>>>>>>> bd2d0c703607830b5dfbdfac520226918aff816f
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    setUpStateMachine();
    stateMachine->start();

    connect(this, SIGNAL(error(QString)), this, SLOT(on_error(QString)));

    Config::init();

    loadContacts();
    loadListItems();

    TCPConnection::init(storage);
    // connect(this, SIGNAL(sendMsg(string&, string&)), TCPConnection::get(), SLOT(send(string&, string&)));
    //  sending example:
    // emit sendMsg("ip", "content");

    // connect(TCPConnection::get(), SIGNAL(sendingError()), this, SLOT(...));
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

    Disconnected->assignProperty(ui->pbNewContact, "enabled", true);
    Disconnected->assignProperty(ui->pbSend, "enabled", false);
    Disconnected->assignProperty(ui->pbAttachFile, "enabled", false);
    Disconnected->assignProperty(ui->lwContacts, "enabled", true);
    Disconnected->assignProperty(ui->teChat, "enabled", false);
    Disconnected->assignProperty(ui->teSend, "enabled", false);

    Connected->assignProperty(ui->pbNewContact, "enabled", true);
    Connected->assignProperty(ui->pbSend, "enabled", false);
    Connected->assignProperty(ui->pbAttachFile, "enabled", true);
    Connected->assignProperty(ui->lwContacts, "enabled", true);
    Connected->assignProperty(ui->teChat, "enabled", true);
    Connected->assignProperty(ui->teSend, "enabled", true);

    Sendable->assignProperty(ui->pbNewContact, "enabled", true);
    Sendable->assignProperty(ui->pbSend, "enabled", true);
    Sendable->assignProperty(ui->pbAttachFile, "enabled", true);
    Sendable->assignProperty(ui->lwContacts, "enabled", true);
    Sendable->assignProperty(ui->teChat, "enabled", true);
    Sendable->assignProperty(ui->teSend, "enabled", true);

    Locked->assignProperty(ui->pbNewContact, "enabled", false);
    Locked->assignProperty(ui->pbSend, "enabled", false);
    Locked->assignProperty(ui->pbAttachFile, "enabled", false);
    Locked->assignProperty(ui->lwContacts, "enabled", false);
    Locked->assignProperty(ui->teChat, "enabled", false);
    Locked->assignProperty(ui->teSend, "enabled", false);
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
    if(storage.load())
    {
        for(auto& contact : storage.getContacts())
        {
            contacts.insert({contact.second.getName(), contact.second});
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

void MainWindow::on_contactAddSuccess(std::string ip)
{
    // Update contact list form file
    storage.load();

    auto added = storage.getContact(ip);
    contacts.insert({added->getName(), *added});

    contacts.clear();
    ui->lwContacts->clear();
    loadContacts();
    loadListItems();

    emit contactAdded();
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
    activeContact = &contacts[item->text().toStdString()];
}

void MainWindow::on_pbDeleteContact_clicked()
{
    storage.deleteContact(activeContact->getAddress());
    contacts.clear();
    ui->lwContacts->clear();
    loadContacts();
    loadListItems();
}

void MainWindow::on_pbEditContact_clicked()
{
    editContactWin = new EditContactWindow{this};
    editContactWin->show();

    emit edited(activeContact->getAddress(), activeContact->getName(), activeContact->getPort());
}
