#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    config::init();
    ui->setupUi(this);

    auto stateMachine = new QStateMachine{this};

    auto Unlocked = new QState{stateMachine};

    auto History = new QHistoryState{Unlocked};
    auto Disconnected = new QState{Unlocked};
    auto Connected = new QState{Unlocked};
    auto ValidateSendable = new QState{Unlocked};
    auto Sendable = new QState{Unlocked};

    auto Locked = new QState{stateMachine};

    setUpStateMachine(stateMachine, Unlocked, Locked, History, Disconnected, Connected, ValidateSendable, Sendable);

    stateMachine->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpStateMachine(QStateMachine* stateMachine, QState* Unlocked, QState* Locked, QHistoryState* History,
                                   QState* Disconnected, QState* Connected, QState* ValidateSendable, QState* Sendable)
{
    assignStatesProperties(Unlocked, Locked, Disconnected, Connected, Sendable);
    setStatesTransistions(Unlocked, Locked, Disconnected, Connected, ValidateSendable, Sendable);

    History->setDefaultState(Disconnected);
    Unlocked->setInitialState(History);
    stateMachine->setInitialState(Unlocked);
}

void MainWindow::assignStatesProperties(QState* Unlocked, QState* Locked, QState* Disconnected, QState* Connected, QState* Sendable)
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

void MainWindow::setStatesTransistions(QState* Unlocked, QState* Locked, QState* Disconnected, QState* Connected, QState* ValidateSendable, QState* Sendable)
{
    Unlocked->addTransition(ui->pbNewContact, SIGNAL(clicked()), Locked);

    Connected->addTransition(ui->teSend, SIGNAL(textChanged()), ValidateSendable);

    connect(ValidateSendable, SIGNAL(entered()), this, SLOT(on_validateSendable()));
    ValidateSendable->addTransition(this, SIGNAL(msgSendable()), Sendable);
    ValidateSendable->addTransition(this, SIGNAL(msgUnsendable()), Connected);

    Sendable->addTransition(ui->teSend, SIGNAL(textChanged()), ValidateSendable);

    Locked->addTransition(this, SIGNAL(contactAdded()), Connected);
    Locked->addTransition(this, SIGNAL(contactAdditionCanceled()), Unlocked);
    Locked->addTransition(this, SIGNAL(errorCatched()), Unlocked);
}

void MainWindow::on_pbNewContact_clicked()
{
    addContactWin = new AddContactWindow{this};
    addContactWin->show();
}

void MainWindow::on_contactAddSuccess()
{
    // TODO : update contact list form file
    emit contactAdded();
}

void MainWindow::on_contactAddCancel()
{
    emit contactAdditionCanceled();
}

void MainWindow::on_error(QString errorMessage)
{
    errWin = new Err$cwd = '/tmp';orWindow{this, errorMessage};
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
