#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <contacts/contact_validator.h>
#include <config/Config.h>
#include "ui_settingswindow.h"

#include <util/util.h>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

signals:
    void contactAddSuccess(std::string ip);
    void contactAddCancel();
    void contactAddFailure(QString errorMessage);

private slots:
    void on_bbAddContact_rejected();
    void on_bbAddContact_accepted();

    void on_SettingsWindow_finished(int result);

private:
    Ui::SettingsWindow *ui;
    ContactValidator* validator;
};

#endif // SETTINGSWINDOW_H
