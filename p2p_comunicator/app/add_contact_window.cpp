#include "add_contact_window.h"
#include "ui_add_contact_window.h"

AddContactWindow::AddContactWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactWindow)
{
    ui->setupUi(this);
    setFixedSize(this->minimumSize());  // sets size to fixed value, disables resizing

    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));
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
