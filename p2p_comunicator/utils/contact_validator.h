#ifndef CONTACTVALIDATOR_H
#define CONTACTVALIDATOR_H

#include <QRegularExpression>
#include <QObject>

class ContactValidator
{
    Q_OBJECT
public:
    ContactValidator();

    bool validateContactForm(QString name, QString ip, QString port);
    QString validationErrMsg() { return _validationErrMsg; }

private:
    bool isValidName{};
    bool isValidIP{};
    bool isValidPort{};

    QString _validationErrMsg;

    bool validateName(QString name);
    bool validateIP(QString ip);
    bool validatePort(QString port);
};

#endif // CONTACTVALIDATOR_H
