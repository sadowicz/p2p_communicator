#include "error_window.h"
#include "ui_error_window.h"

ErrorWindow::ErrorWindow(QWidget *parent, QString errorMessage) :
    QDialog(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
    setFixedSize(this->minimumSize()); // sets fixed window size and disables resizing
    errMsg = errorMessage;
    ui->lbErrorPrompt->setText(errMsg);

    QObject::connect(this, SIGNAL(errorRead()), parent, SLOT(on_errorRead()));
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}

void ErrorWindow::on_bbErrorPrompt_rejected()
{
    emit errorRead();
}
