#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStateMachine>
#include <QHistoryState>
#include <QDebug>

#include <MessageListDelegate.h>
#include <add_contact_window.h>
#include <editcontactwindow.h>
#include <settingswindow.h>
#include <error_window.h>
#include <config/Config.h>
#include <Logger.h>
#include "ui_mainwindow.h"

#include <contacts/Storage.h>
#include <contacts/ContactController.h>

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
    void edited(std::string ip, std::string name, int port);

private slots:
    void on_contactAddSuccess(Contact* contact);
    void on_contactEditSuccess(Contact* contact);
    void on_contactAddCancel();

    void on_contactStatusChanged();

    void on_error(QString errorMessage);
    void on_errorRead();
    void on_validateSendable();

    void on_pbNewContact_clicked();
    void on_lwContacts_itemClicked(QListWidgetItem *item);
    void on_pbDeleteContact_clicked();
    void on_pbEditContact_clicked();
    void on_pbSettings_clicked();

    void on_pbSend_clicked();

private:
    Ui::MainWindow *ui;
    AddContactWindow *addContactWin;
    EditContactWindow *editContactWin;
    SettingsWindow *settingsWin;
    ErrorWindow *errWin;

    QStateMachine* stateMachine;
    QState* Unlocked;
    QState* Locked;
    QHistoryState* History;
    QState* Disconnected;
    QState* Connected;
    QState* ValidateSendable;
    QState* Sendable;

    ContactController* contactController;
    Logger log;

    std::unordered_map<std::string, Contact*> contacts;
    Contact* activeContact;

    void assignStatesProperties();
    void setStatesTransistions();
    void setUpStateMachine();

    void loadContacts();
    void loadListItems();
    void refreshContactsList();

};
#endif // MAINWINDOW_H
