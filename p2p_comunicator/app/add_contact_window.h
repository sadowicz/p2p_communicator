#ifndef ADD_CONTACT_WINDOW_H
#define ADD_CONTACT_WINDOW_H

#include <QDialog>
#include <QRegularExpression>
#include <QDebug>

namespace Ui {
class AddContactWindow;
}

class AddContactWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddContactWindow(QWidget *parent = nullptr);
    ~AddContactWindow();

private:
    Ui::AddContactWindow *ui;

    bool validateForm();
    bool validateIP();

signals:
    void contactAddSuccess();
    void contactAddCancel();
    void contactAddFailure(QString errorMessage);

private slots:
    void on_bbAddContact_accepted();
    void on_bbAddContact_rejected();
};

#endif // ADD_CONTACT_WINDOW_H
