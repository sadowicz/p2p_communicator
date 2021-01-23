#include <add_contact_window.h>

AddContactWindow::AddContactWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactWindow)
{
    validator = new ContactValidator{};

    ui->setupUi(this);
    setFixedSize(this->minimumSize());  // sets size to fixed value, disables resizing

    QObject::connect(this, SIGNAL(contactAddSuccess()), parent, SLOT(on_contactAddSuccess()));
    QObject::connect(this, SIGNAL(contactAddCancel()), parent, SLOT(on_contactAddCancel()));
    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));

    storage.load();
}

AddContactWindow::~AddContactWindow()
{
    delete ui;
    delete validator;
}

void AddContactWindow::on_bbAddContact_accepted()
{
    if(validator->validateContactForm(ui->leName->text(), ui->leIP->text(), ui->lePort->text()))
    {
        // TODO: File Storage

        Contact newContact = Contact(ui->leName->text().toStdString(), ui->leIP->text().toStdString(), ui->lePort->text().toUInt());
        storage.addContact(newContact);

        //if storage successfull:
        emit contactAddSuccess();
    }
    else
        emit contactAddFailure(validator->validationErrMsg());
}

void AddContactWindow::on_AddContactWindow_finished(int result)
{
    if(result == QDialog::Rejected)
        emit contactAddCancel();
}
