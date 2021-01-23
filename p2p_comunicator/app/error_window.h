#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QDialog>

#include <contacts/Storage.h>
#include <ui_error_window.h>

namespace Ui {
class ErrorWindow;
}

class ErrorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorWindow(QWidget *parent = nullptr, QString errorMessage = tr("Unidentified error."));
    ~ErrorWindow();

signals:
    void errorRead();

private slots:
    void on_ErrorWindow_finished(int result);

private:
    Ui::ErrorWindow *ui;
    QString errMsg;
    Storage storage;
};

#endif // ERRORWINDOW_H
