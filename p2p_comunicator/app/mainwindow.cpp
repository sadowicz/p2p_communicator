#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addContactWin = new AddContactWindow(this);

    QObject::connect(addContactWin, SIGNAL(contactAddFailure(QString)), this, SLOT(on_error(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pbNewContact_clicked()
{
    addContactWin->show();
    addContactWin->setFixedSize(addContactWin->minimumSize());  // sets size to fixed value, disables resizing
}

void MainWindow::on_contactAddSuccess()
{

}

void MainWindow::on_error(QString errorMessage)
{
    errWin = new ErrorWindow(this, errorMessage);
    errWin->show();
}
