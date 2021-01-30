#ifndef CONTACTVALIDATOR_H
#define CONTACTVALIDATOR_H

#include <QRegularExpression>
#include <map>

#include "Storage.h"
#include "Contact.h"

namespace contacts {

class ContactValidator
{
public:
    ContactValidator();
    ~ContactValidator() = default;

    bool validateContactForm(QString name, QString ip, QString port,
                             std::unordered_map<std::string, Contact*>& contacts);

    bool validateContactForm(QString name, QString port, std::unordered_map<std::string, Contact*>& contacts);
    QString validationErrMsg() { return _validationErrMsg; }

private:
    bool isValidName{};
    bool isValidIP{};
    bool isValidPort{};
    bool isUnique{};

    QString _validationErrMsg;

    static QString headerErrMsg;
    static QString nameErrMsg;
    static QString IPErrMsg;
    static QString portErrMsg;
    static QString uniqNameErrMsg;
    static QString uniqIPErrMsg;

    void validateName(QString name);
    void validateIP(QString ip);
    void validatePort(QString port);
    void validateUnique(QString name, QString ip, std::unordered_map<std::string, Contact*>& contacts);
    void validateUnique(QString name, std::unordered_map<std::string, Contact*>& contacts);
};


}
#endif // CONTACTVALIDATOR_H


