#include "add_contact_window.h"
#include "ui_add_contact_window.h"
#include "Contact.h"
#include "Storage.h"

AddContactWindow::AddContactWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactWindow)
{
    ui->setupUi(this);
    setFixedSize(this->minimumSize());  // sets size to fixed value, disables resizing

    QObject::connect(this, SIGNAL(contactAddSuccess()), parent, SLOT(on_contactAddSuccess()));
    QObject::connect(this, SIGNAL(contactAddCancel()), parent, SLOT(on_contactAddCancel()));
    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));

    Storage storage{};
    Contact contact{"example", "127.0.0.1", 8080};
    storage.addContact(contact);
    Contact contact2{"example2", "127.5.0.1", 7080};
    storage.addContact(contact2);
    storage.save();
}

AddContactWindow::~AddContactWindow()
{
    delete ui;
}

bool AddContactWindow::validateForm()
{
    bool isValidName = (!ui->leName->text().isEmpty() &&
                        ui->leName->text().length() <= 25);

    bool isValidIP = validateIP();

    bool isValidPort = (!ui->lePort->text().isEmpty() &&
                        ui->lePort->text().toInt() >= 0 &&
                        ui->lePort->text().toInt() <= 65535);

    if(!isValidName || !isValidIP || !isValidPort)
       return false;

    return true;
}

bool AddContactWindow::validateIP()
{
    QRegularExpression regex{"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"};
    return regex.match(ui->leIP->text()).hasMatch();
}

void AddContactWindow::on_bbAddContact_accepted()
{
    if(validateForm())
    {
        // TODO: File Storage



        //if storage successfull:
        emit contactAddSuccess();
    }
    else
        emit contactAddFailure(tr("Unable to add contact to contact list.\nInvalid input parameters"));
}

void AddContactWindow::on_AddContactWindow_finished(int result)
{
    if(result == QDialog::Rejected)
        emit contactAddCancel();
}
