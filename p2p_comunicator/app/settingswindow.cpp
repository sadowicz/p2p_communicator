#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    ui->lePort->setText(QString::fromStdString(Config::config("port")));

    validator = new ContactValidator{};

    connect(this, SIGNAL(settingsSaveCancel()), parent, SLOT(on_contactAddCancel()));
    connect(this, SIGNAL(settingsSaveFailure(QString)), parent, SLOT(on_error(QString)));
    connect(this, SIGNAL(settingsSaveSuccess()), parent, SLOT(on_settingsSaveSuccess()));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
    delete validator;
}

void SettingsWindow::on_bbAddContact_accepted()
{
    if(validator->validateContactForm(ContactValidator::unchangedName, ui->lePort->text(),
                                      Storage::storage().getContacts()))
    {
        Config::config("port") = ui->lePort->text().toStdString();
        Config::config().save();
        emit settingsSaveSuccess();
        delete this;
    }
    else
        emit settingsSaveFailure("Unable to set port.\nInvalid port format.");
}


void SettingsWindow::on_SettingsWindow_finished(int result)
{
    if(result == QDialog::Rejected)
        emit settingsSaveCancel();
}

void SettingsWindow::on_bbAddContact_rejected()
{
    emit settingsSaveCancel();
    delete this;
}
