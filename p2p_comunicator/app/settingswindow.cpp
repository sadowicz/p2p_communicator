#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    validator = new ContactValidator{};

    QObject::connect(this, SIGNAL(contactAddCancel()), parent, SLOT(on_contactAddCancel()));
    QObject::connect(this, SIGNAL(contactAddFailure(QString)), parent, SLOT(on_error(QString)));
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

        delete this;
    }
    else
        emit contactAddFailure("Unable to set port.\nInvalid port format.");
}

void SettingsWindow::on_SettingsWindow_finished(int result)
{
    if(result == QDialog::Rejected)
        emit contactAddCancel();
}
