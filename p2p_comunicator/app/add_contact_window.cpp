#include "add_contact_window.h"
#include "ui_add_contact_window.h"

AddContactWindow::AddContactWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactWindow)
{
    ui->setupUi(this);
    setFixedSize(this->minimumSize());  // sets size to fixed value, disables resizing

    QObject::connect(this, SIGNAL(contactAddSuccess()), parent, SLOT(on_contactAddSuccess()));
    QObject::connect(this, SIGNAL(contactAddCancel()), parent, SLOT(on_contactAddCancel()));
    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));
}

AddContactWindow::~AddContactWindow()
{
    delete ui;
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
