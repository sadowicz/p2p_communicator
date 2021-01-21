#include "contact_validator.h"

ContactValidator::ContactValidator()
{
    _validationErrMsg = "Unable to add new contact.";
}

bool ContactValidator::validateContactForm(QString name, QString ip, QString port)
{
    return validateName(name) &&
           validateIP(ip) &&
           validatePort(port);
}

bool ContactValidator::validateName(QString name)
{
    isValidName = (!name.isEmpty() &&
                    name.length() <= 25);

    if(!isValidName) _validationErrMsg += "\nInvalid contact name format.";

    return isValidName;
}

bool ContactValidator::validateIP(QString ip)
{
    QRegularExpression regex{"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"};
    isValidIP = regex.match(ip).hasMatch();

    if(!isValidIP) _validationErrMsg += "\nInvalid IP adress format.";

    return isValidIP;
}

bool ContactValidator::validatePort(QString port)
{
    isValidPort = (!port.isEmpty() &&
                    port.toInt() >= 0 &&
                    port.toInt() <= 65535);

    if(!isValidPort) _validationErrMsg += "\nInvalid port adress format.";

    return isValidPort;
}
