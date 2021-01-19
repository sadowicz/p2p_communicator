#ifndef ADD_CONTACT_WINDOW_H
#define ADD_CONTACT_WINDOW_H

#include <QDialog>

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

signals:
    void contactAddSuccess();
    void conactAddFailure();
};

#endif // ADD_CONTACT_WINDOW_H
