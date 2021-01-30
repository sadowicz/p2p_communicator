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
    static const QString unchangedName;

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

    static const QString headerErrMsg;
    static const QString nameErrMsg;
    static const QString IPErrMsg;
    static const QString portErrMsg;
    static const QString uniqNameErrMsg;
    static const QString uniqIPErrMsg;

    void validateName(QString name);
    void validateIP(QString ip);
    void validatePort(QString port);
    void validateUnique(QString name, QString ip, std::unordered_map<std::string, Contact*>& contacts);
    void validateUnique(QString name, std::unordered_map<std::string, Contact*>& contacts);
};


}
#endif // CONTACTVALIDATOR_H


