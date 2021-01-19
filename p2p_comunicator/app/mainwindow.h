#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <add_contact_window.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbNewContact_clicked();
    void on_contactAddSuccess();
    void on_contactAddFailure();

private:
    Ui::MainWindow *ui;
    AddContactWindow *addContactWin;
};
#endif // MAINWINDOW_H
