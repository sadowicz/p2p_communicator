#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStateMachine>
#include <QHistoryState>

#include "add_contact_window.h"
#include "error_window.h"
#include <config/Config.h>

#include "Storage.h"

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
    void errorCatched();
    void msgSendable();
    void msgUnsendable();

private slots:
    void on_pbNewContact_clicked();
    void on_contactAddSuccess();
    void on_contactAddCancel();
    void on_error(QString errorMessage);
    void on_errorRead();
    void on_validateSendable();

private:
    Ui::MainWindow *ui;
    AddContactWindow *addContactWin;
    ErrorWindow *errWin;

    QStateMachine* stateMachine;
    QState* Unlocked;
    QState* Locked;
    QHistoryState* History;
    QState* Disconnected;
    QState* Connected;
    QState* ValidateSendable;
    QState* Sendable;

    void assignStatesProperties();
    void setStatesTransistions();
    void setUpStateMachine();

    Storage storage;
};
#endif // MAINWINDOW_H
