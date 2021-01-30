#ifndef CONTACTVALIDATOR_H
#define CONTACTVALIDATOR_H

#include <QRegularExpression>

namespace contacts {

class ContactValidator
{
public:
    ContactValidator();
    ~ContactValidator() = default;

    bool validateContactForm(QString name, QString ip, QString port);
    QString validationErrMsg() { return _validationErrMsg; }

private:
    bool isValidName{};
    bool isValidIP{};
    bool isValidPort{};

    QString _validationErrMsg;

    static QString headerErrMsg;
    static QString nameErrMsg;
    static QString IPErrMsg;
    static QString portErrMsg;

    void validateName(QString name);
    void validateIP(QString ip);
    void validatePort(QString port);
};


}
#endif // CONTACTVALIDATOR_H


