#include "contact_validator.h"

using namespace contacts;

QString ContactValidator::headerErrMsg = "Unable to add new contact.\n";
QString ContactValidator::nameErrMsg = "\nInvalid contact name format.";
QString ContactValidator::IPErrMsg = "\nInvalid IP address format.";
QString ContactValidator::portErrMsg = "\nInvalid port format.";
QString ContactValidator::uniqNameErrMsg = "\nContact name already exists.";
QString ContactValidator::uniqIPErrMsg = "\nIP occupied by existing contact.";

ContactValidator::ContactValidator()
{
    _validationErrMsg = ContactValidator::headerErrMsg;
}

bool ContactValidator::validateContactForm(QString name, QString ip, QString port,
                                           std::unordered_map<std::string, Contact*>& contacts)
{
    validateName(name);
    validateIP(ip);
    validatePort(port);
    validateUnique(name, ip, contacts);

    return isValidName && isValidIP && isValidPort && isUnique;
}

void ContactValidator::validateName(QString name)
{
    isValidName = (!name.isEmpty() &&
                    name.length() <= 25);

    if(!isValidName) _validationErrMsg += ContactValidator::nameErrMsg;
}

void ContactValidator::validateIP(QString ip)
{
    QRegularExpression regex{"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"};
    isValidIP = regex.match(ip).hasMatch();

    if(!isValidIP) _validationErrMsg += ContactValidator::IPErrMsg;
}

void ContactValidator::validatePort(QString port)
{
    QRegularExpression regex{"\\d+"};

    isValidPort = (regex.match(port).hasMatch() &&
                   port.toInt() >= 0 &&
                   port.toInt() <= 65535);

    if(!isValidPort) _validationErrMsg += ContactValidator::portErrMsg;
}

void ContactValidator::validateUnique(QString name, QString ip, std::unordered_map<std::string, Contact*>& contacts)
{   
    if(contacts.find(ip.toStdString()) == contacts.end())
    {
        for(auto& contact : contacts)
        {
            if(contact.second->getName().c_str() == name)
            {
                _validationErrMsg += ContactValidator::uniqNameErrMsg;
                return;
            }
        }

        isUnique = true;
    }
    else
        _validationErrMsg += ContactValidator::uniqIPErrMsg;
}
