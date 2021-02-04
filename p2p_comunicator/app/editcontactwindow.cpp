#include "editcontactwindow.h"
#include "ui_editcontactwindow.h"

EditContactWindow::EditContactWindow(std::string ip, std::string name, int port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditContactWindow)
{
    ui->setupUi(this);

    setValues(ip, name, port);

    log = util::getLogger();
    log.debug("Editing contact");

    validator = new ContactValidator{};

    QObject::connect(this, SIGNAL(contactAddSuccess(std::string, std::string, unsigned int)),
                     parent, SLOT(on_contactEditSuccess(std::string, std::string, unsigned int)));
    QObject::connect(this, SIGNAL(contactAddCancel()), parent, SLOT(on_contactAddCancel()));
    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));

    Storage::storage().load();
}

EditContactWindow::~EditContactWindow()
{
    delete ui;
    delete validator;
}

void EditContactWindow::on_bbAddContact_accepted()
{
    log.debug("Edit contact successful");
    QString validationName = ui->leName->text();
    if (!Storage::storage().contactExists(ip)) {
        emit contactAddFailure(QString("Contact was not added!!!"));
        return;
    }
    if(validationName == Storage::storage().getContact(ip)->getName().c_str())
        validationName = ContactValidator::unchangedName;

    if(validator->validateContactForm(validationName, ui->lePort->text(),
                                      Storage::storage().getContacts()))
    {

        //if storage successful:
        emit contactAddSuccess(ip, ui->leName->text().toStdString(), ui->lePort->text().toUInt());

        delete this;
    }
    else
        emit contactAddFailure(validator->validationErrMsg());
}

void EditContactWindow::setValues(std::string ip, std::string name, int port) {
    this->ip = ip;
    this->name = name;
    this->port = port;
    log.debug("Editing contact: " + ip + ":" + std::to_string(port));

    ui->ipLabel->setText(QString::fromStdString(ip));
    ui->leName->setText(QString::fromStdString(name));
    ui->lePort->setText(QString::number(port));
}

void EditContactWindow::on_EditContactWindow_finished(int result)
{
    log.debug("Editing cancelled");
    if(result == QDialog::Rejected)
        emit contactAddCancel();
}

void EditContactWindow::on_bbAddContact_rejected()
{
    emit contactAddCancel();
    delete this;
}
