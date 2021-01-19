#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pbNewContact_clicked()
{
    addContactWin = new AddContactWindow(this);
    addContactWin->show();
}

void MainWindow::on_contactAddSuccess()
{

}

void MainWindow::on_error(QString errorMessage)
{
    errWin = new ErrorWindow(this, errorMessage);
    errWin->show();
}
