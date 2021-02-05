#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <contacts/contact_validator.h>
#include <config/Config.h>
#include "ui_settingswindow.h"

using namespace contacts;

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
    void settingsSaveSuccess();
    void settingsSaveCancel();
    void settingsSaveFailure(QString errorMessage);

private slots:
    void on_bbAddContact_accepted();

    void on_SettingsWindow_finished(int result);

    void on_bbAddContact_rejected();

private:
    Ui::SettingsWindow *ui;
    ContactValidator* validator;
};

#endif // SETTINGSWINDOW_H
