#ifndef EDITCONTACTWINDOW_H
#define EDITCONTACTWINDOW_H

#include <QDialog>
#include <contacts/contact_validator.h>
#include <contacts/Storage.h>

#include <config/Config.h>
#include <util/LoggerHelper.h>

using namespace contacts;

namespace Ui {
class EditContactWindow;
}

class EditContactWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditContactWindow(std::string ip, std::string name, int port, QWidget *parent = nullptr);
    ~EditContactWindow();

private:
    Ui::EditContactWindow *ui;
    ContactValidator* validator;

    std::string ip;
    std::string name;
    Logger log;
    int port;

    void setValues(std::string ip, std::string name, int port);

signals:
    void contactAddSuccess(std::string, std::string, unsigned int);
    void contactAddCancel();
    void contactAddFailure(QString errorMessage);
private slots:
    void on_bbAddContact_accepted();
    void on_EditContactWindow_finished(int result);
    void on_bbAddContact_rejected();

};

#endif // EDITCONTACTWINDOW_H
