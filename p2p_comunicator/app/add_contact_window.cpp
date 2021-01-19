#include "add_contact_window.h"
#include "ui_add_contact_window.h"

AddContactWindow::AddContactWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactWindow)
{
    ui->setupUi(this);
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

    bool isValidPort = (!ui->leName->text().isEmpty() &&
                        ui->lePort->text().toInt() >= 0 &&
                        ui->lePort->text().toInt() <= 65535);

    if(!isValidName || !isValidIP || !isValidPort)
       return false;

    return true;
}

bool AddContactWindow::validateIP()
{
    QRegularExpression regex{"\\d{1,4}.\\d{1,4}.\\d{1,4}.\\d{1,4}"};
    return regex.match(ui->leName->text()).hasMatch();
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
        emit contactAddFailure(tr("Unable to add contact to contact list"));
}
