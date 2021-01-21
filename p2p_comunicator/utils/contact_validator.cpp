#include "contact_validator.h"

ContactValidator::ContactValidator()
{
    _validationErrMsg = "Unable to add new contact.\n";
}

bool ContactValidator::validateContactForm(QString name, QString ip, QString port)
{
    validateName(name);
    validateIP(ip);
    validatePort(port);

    return isValidName && isValidIP && isValidPort;
}

void ContactValidator::validateName(QString name)
{
    isValidName = (!name.isEmpty() &&
                    name.length() <= 25);

    if(!isValidName) _validationErrMsg += "\nInvalid contact name format.";
}

void ContactValidator::validateIP(QString ip)
{
    QRegularExpression regex{"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"};
    isValidIP = regex.match(ip).hasMatch();

    if(!isValidIP) _validationErrMsg += "\nInvalid IP adress format.";
}

void ContactValidator::validatePort(QString port)
{
    isValidPort = (!port.isEmpty() &&
                    port.toInt() >= 0 &&
                    port.toInt() <= 65535);

    if(!isValidPort) _validationErrMsg += "\nInvalid port adress format.";
}
