#ifndef ADD_CONTACT_WINDOW_H
#define ADD_CONTACT_WINDOW_H

#include <QDialog>
#include <QDebug>

#include <contacts/contact_validator.h>
#include <contacts/Storage.h>
#include <ui_add_contact_window.h>

namespace Ui {
class AddContactWindow;
}

class AddContactWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddContactWindow(QWidget *parent = nullptr);
    ~AddContactWindow();

private:
    Ui::AddContactWindow *ui;
    ContactValidator* validator;
    Storage storage;


signals:
    void contactAddSuccess();
    void contactAddCancel();
    void contactAddFailure(QString errorMessage);

private slots:
    void on_bbAddContact_accepted();
    void on_AddContactWindow_finished(int result);
};

#endif // ADD_CONTACT_WINDOW_H
