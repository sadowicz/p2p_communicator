#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QDialog>

namespace Ui {
class ErrorWindow;
}

class ErrorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorWindow(QWidget *parent = nullptr, QString errorMessage = tr("Unidentified error."));
    ~ErrorWindow();

private:
    Ui::ErrorWindow *ui;
    QString errMsg;
};

#endif // ERRORWINDOW_H
