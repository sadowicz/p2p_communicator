#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStateMachine>
#include <QHistoryState>
#include <QDebug>

#include <add_contact_window.h>
#include <editcontactwindow.h>
#include <error_window.h>
#include <config/Config.h>
#include "ui_mainwindow.h"

#include <contacts/Storage.h>
#include <tcp/TCPConnection.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void contactAdded();
    void contactAdditionCanceled();
    void error(QString errorMessage);
    void errorCatched();
    void msgSendable();
    void msgUnsendable();
    void sendMsg(string& ip, string& content);

private slots:
    void on_pbNewContact_clicked();
    void on_contactAddSuccess(std::string ip);
    void on_contactAddCancel();
    void on_error(QString errorMessage);
    void on_errorRead();
    void on_validateSendable();

    void on_lwContacts_itemClicked(QListWidgetItem *item);

    void on_pbDeleteContact_clicked();

    void on_pbEditContact_clicked();

private:
    Ui::MainWindow *ui;
    AddContactWindow *addContactWin;
    EditContactWindow *editContactWin;
    ErrorWindow *errWin;

    QStateMachine* stateMachine;
    QState* Unlocked;
    QState* Locked;
    QHistoryState* History;
    QState* Disconnected;
    QState* Connected;
    QState* ValidateSendable;
    QState* Sendable;

    Storage storage;
    std::unordered_map<std::string, Contact> contacts;
    Contact* activeContact;

    void assignStatesProperties();
    void setStatesTransistions();
    void setUpStateMachine();

    void loadContacts();
    void loadListItems();

};
#endif // MAINWINDOW_H
