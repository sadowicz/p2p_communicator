#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    validator = new ContactValidator{};
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
    delete validator;
}

void SettingsWindow::on_bbAddContact_rejected()
{
    delete this;
}

void SettingsWindow::on_bbAddContact_accepted()
{
    if(validator->validateContactForm("name", "127.0.0.1", ui->lePort->text()))
    {
        std::string p = "port";
        std::string val = ui->lePort->text().toStdString();
        Config::config().set(p, val);
        Config::config().save();

        //if storage successfull:
        emit contactAddSuccess("newContact.getAddress()");
    }
    else
        emit contactAddFailure(validator->validationErrMsg());
}

void SettingsWindow::on_SettingsWindow_finished(int result)
{
    if(result == QDialog::Rejected)
        emit contactAddCancel();
}
