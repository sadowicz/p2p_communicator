#ifndef EDITCONTACTWINDOW_H
#define EDITCONTACTWINDOW_H

#include <QDialog>
#include <contacts/contact_validator.h>
#include <contacts/Storage.h>

namespace Ui {
class EditContactWindow;
}

class EditContactWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditContactWindow(QWidget *parent = nullptr);
    ~EditContactWindow();

private:
    Ui::EditContactWindow *ui;
    ContactValidator* validator;
    Storage storage;

signals:
    void contactAddSuccess(std::string ip);
    void contactAddCancel();
    void contactAddFailure(QString errorMessage);
private slots:
    void on_bbAddContact_accepted();
    void on_EditContactWindow_finished(int result);
};

#endif // EDITCONTACTWINDOW_H
