#include "editcontactwindow.h"
#include "ui_editcontactwindow.h"


EditContactWindow::EditContactWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditContactWindow)
{
    log = Logger(Config::config("log-file"), Config::config().debugMode());
    log.debug("Editing contact");

    validator = new ContactValidator{};

    ui->setupUi(this);

    QObject::connect(this, SIGNAL(contactAddSuccess(Contact*)), parent, SLOT(on_contactEditSuccess(Contact*)));
    QObject::connect(this, SIGNAL(contactAddCancel()), parent, SLOT(on_contactAddCancel()));
    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));
    QObject::connect(parent, SIGNAL(edited(std::string,std::string, int)), this, SLOT(setValues(std::string,std::string,int)));

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

    if(validationName == Storage::storage().getContact(ip)->getName().c_str())
        validationName = ContactValidator::unchangedName;

    if(validator->validateContactForm(validationName, ui->lePort->text(),
                                      Storage::storage().getContacts()))
    {
        Contact* editedContact = new Contact(ui->leName->text().toStdString(), ip,
                                             ui->lePort->text().toUInt());

        //if storage successful:
        emit contactAddSuccess(editedContact);

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
    delete this;
}
