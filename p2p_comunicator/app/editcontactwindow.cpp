#include "editcontactwindow.h"
#include "ui_editcontactwindow.h"

EditContactWindow::EditContactWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditContactWindow)
{
    validator = new ContactValidator{};

    ui->setupUi(this);
    //setFixedSize(this->minimumSize());  // sets size to fixed value, disables resizing

    QObject::connect(this, SIGNAL(contactAddSuccess(std::string)), parent, SLOT(on_contactAddSuccess(std::string)));
    QObject::connect(this, SIGNAL(contactAddCancel()), parent, SLOT(on_contactAddCancel()));
    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));
    QObject::connect(parent, SIGNAL(edited(std::string,std::string, int)), this, SLOT(setValues(std::string,std::string,int)));

    storage.load();
}

EditContactWindow::~EditContactWindow()
{
    delete ui;
    delete validator;
}

void EditContactWindow::on_bbAddContact_accepted()
{
    if(validator->validateContactForm(ui->leName->text(), ui->leIP->text(), ui->lePort->text()))
    {
        storage.editContact(ui->leIP->text().toStdString(), ui->leName->text().toStdString(), ui->leIP->text().toStdString(), ui->lePort->text().toUInt());

        //if storage successfull:
        emit contactAddSuccess(ui->leIP->text().toStdString());
    }
    else
        emit contactAddFailure(validator->validationErrMsg());
}

void EditContactWindow::setValues(std::string ip, std::string name, int port) {
    ui->leIP->setText(QString::fromStdString(ip));
    ui->leName->setText(QString::fromStdString(name));
    ui->lePort->setText(QString::number(port));
}

void EditContactWindow::on_EditContactWindow_finished(int result)
{
    if(result == QDialog::Rejected)
        emit contactAddCancel();
}

void EditContactWindow::on_bbAddContact_rejected()
{
    delete this;
}
