#include <QtTest>

#include <contacts/contact_validator.h>

using namespace contacts;

class ContactValidatorTest : public QObject
{
    Q_OBJECT

public:
    ContactValidatorTest();
    ~ContactValidatorTest();

private slots:
    void contactValidatorIsCreateable();
    void constructorSetsErrMsgHeader();

    void emptyFiledReturnFalse_data();
    void emptyFiledReturnFalse();

    void allFieldsValidReturnTrue();

    void nameShorterThan25ReturnTrue();
    void nameLongerThan25ReturnFalse();

    void ipInvalidFormatReturnFalse_data();
    void ipInvalidFormatReturnFalse();

    void portInRangeReturnTrue();
    void portOutOfRangeReturnFalse();
    void nonNumericPortReturnFalse();

    void uniqueNameAndIPReturnTrue();
    void notUniqueNameReturnFalse();
    void notUniqueIPReturnFalse();

    void validationErrMsgReturnsErrorInfo_data();
    void validationErrMsgReturnsErrorInfo();
    void validationErrMsgReturnsIPUniquenessErrorInfo();
    void validationErrMsgReturnsNameUniquenessErrorInfo();
};

ContactValidatorTest::ContactValidatorTest() {}

ContactValidatorTest::~ContactValidatorTest() {}

void ContactValidatorTest::contactValidatorIsCreateable()
{
    ContactValidator validator{};
}

void ContactValidatorTest::constructorSetsErrMsgHeader()
{
    ContactValidator validator{};
    QCOMPARE(validator.validationErrMsg(), QString("Unable to submit contact.\n"));
}

void ContactValidatorTest::emptyFiledReturnFalse_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("ip");
    QTest::addColumn<QString>("port");
    QTest::addColumn<bool>("result");

    QTest::newRow("all empty") << QString("") << QString("") << QString("") << false;

    QTest::newRow("ip, port empty") << QString("abc") << QString("") << QString("") << false;
    QTest::newRow("name, port empty") << QString("") << QString("1.1.1.1") << QString("") << false;
    QTest::newRow("name, ip empty") << QString("") << QString("") << QString("1234") << false;

    QTest::newRow("port empty") << QString("abc") << QString("1.1.1.1") << QString("") << false;
    QTest::newRow("name empty") << QString("") << QString("1.1.1.1") << QString("1234") << false;
    QTest::newRow("ip empty") << QString("abc") << QString("") << QString("1234") << false;
}

void ContactValidatorTest::emptyFiledReturnFalse()
{
    ContactValidator validator{};
    std::unordered_map<std::string, Contact*> contacts{};

    QFETCH(QString, name);
    QFETCH(QString, ip);
    QFETCH(QString, port);
    QFETCH(bool, result);

    QCOMPARE(validator.validateContactForm(name, ip, port, contacts), result);
}

void ContactValidatorTest::allFieldsValidReturnTrue()
{
    ContactValidator validator1{};
    ContactValidator validator2{};

    std::unordered_map<std::string, Contact*> contacts{};

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("1111"), contacts), true);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("1111"), contacts), true);
}

void ContactValidatorTest::nameShorterThan25ReturnTrue()
{
    ContactValidator validator1{};
    ContactValidator validator2{};

    std::unordered_map<std::string, Contact*> contacts{};
    QString name = "a";

    for(int i = 1; i <= 25; i++, name += "a")
    {
        QCOMPARE(validator1.validateContactForm(name, QString("127.0.0.1"), QString("1111"), contacts), true);
        QCOMPARE(validator2.validateContactForm(name, QString("1111"), contacts), true);
    }
}

void ContactValidatorTest::nameLongerThan25ReturnFalse()
{
    ContactValidator validator1{};
    ContactValidator validator2{};
    std::unordered_map<std::string, Contact*> contacts{};

    QCOMPARE(validator1.validateContactForm(QString("aaaaaaaaaaaaaaaaaaaaaaaaaaa"), QString("127.0.0.1"), QString("1111"), contacts), false);
    QCOMPARE(validator2.validateContactForm(QString("aaaaaaaaaaaaaaaaaaaaaaaaaaa"), QString("1111"), contacts), false);

}

void ContactValidatorTest::ipInvalidFormatReturnFalse_data()
{
    QTest::addColumn<QString>("ip");
    QTest::addColumn<bool>("result");

    QTest::newRow("non numeric string") << QString("asdasd") << false;
    QTest::newRow("no octets separation") << QString("111111111111") << false;
    QTest::newRow("not enough octets (2)") << QString("111.111") << false;
    QTest::newRow("not enough octets (3)") << QString("111.111.111") << false;
    QTest::newRow("octet value exceeded") << QString("257.111.111") << false;
}

void ContactValidatorTest::ipInvalidFormatReturnFalse()
{
    ContactValidator validator{};
    std::unordered_map<std::string, Contact*> contacts{};

    QFETCH(QString, ip);
    QFETCH(bool, result);

    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), ip, QString("1234"), contacts), result);
}

void ContactValidatorTest::portInRangeReturnTrue()
{
    ContactValidator validator1{};
    ContactValidator validator2{};
    std::unordered_map<std::string, Contact*> contacts{};

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("0"), contacts), true);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("0"), contacts), true);

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("1234"), contacts), true);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("1234"), contacts), true);

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("27895"), contacts), true);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("27895"), contacts), true);

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("65535"), contacts), true);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("65535"), contacts), true);
}

void ContactValidatorTest::portOutOfRangeReturnFalse()
{
    ContactValidator validator1{};
    ContactValidator validator2{};

    std::unordered_map<std::string, Contact*> contacts{};

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("-1"), contacts), false);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("-1"), contacts), false);

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("65536"), contacts), false);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("65536"), contacts), false);
}

void ContactValidatorTest::nonNumericPortReturnFalse()
{
    ContactValidator validator1{};
    ContactValidator validator2{};
    std::unordered_map<std::string, Contact*> contacts{};

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("abcd"), contacts), false);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("abcd"), contacts), false);

}

void ContactValidatorTest::uniqueNameAndIPReturnTrue()
{
    ContactValidator validator1{};
    ContactValidator validator2{};
    std::unordered_map<std::string, Contact*> contacts{};

    QCOMPARE(validator1.validateContactForm(QString("John Smith"), QString("127.126.1.1"), QString("1234"), contacts), true);
    QCOMPARE(validator2.validateContactForm(QString("John Smith"), QString("1234"), contacts), true);

    Contact added{"John Smith", "127.126.1.1", 1234};
    contacts.insert({"127.126.1.1", &added});

    QCOMPARE(validator1.validateContactForm(QString("Joe Doe"), QString("128.121.1.1"), QString("1234"), contacts), true);
    QCOMPARE(validator2.validateContactForm(QString("Joe Doe"), QString("1234"), contacts), true);
}

void ContactValidatorTest::notUniqueNameReturnFalse()
{
    ContactValidator validator1{};
    ContactValidator validator2{};
    std::unordered_map<std::string, Contact*> contacts{};
    Contact added{"John Smith", "127.126.1.1", 1234};
    contacts.insert({"127.126.1.1", &added});

    QCOMPARE(validator1.validateContactForm(QString("John Smith"), QString("128.121.1.1"), QString("1234"), contacts), false);
    QCOMPARE(validator2.validateContactForm(QString("John Smith"), QString("1234"), contacts), false);
}

void ContactValidatorTest::notUniqueIPReturnFalse()
{
    ContactValidator validator{};
    std::unordered_map<std::string, Contact*> contacts{};
    Contact added{"John Smith", "127.126.1.1", 1234};
    contacts.insert({"127.126.1.1", &added});

    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.126.1.1"), QString("1234"), contacts), false);
}

void ContactValidatorTest::validationErrMsgReturnsErrorInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("ip");
    QTest::addColumn<QString>("port");
    QTest::addColumn<QString>("msg");

    QTest::newRow("all invalid") << QString("") << QString("") << QString("")
                                 << "Unable to submit contact.\n\nInvalid contact name format.\nInvalid IP address format.\nInvalid port format.";

    QTest::newRow("ip, port invalid") << QString("abc") << QString("") << QString("")
                                      << "Unable to submit contact.\n\nInvalid IP address format.\nInvalid port format.";
    QTest::newRow("name, port invalid") << QString("") << QString("1.1.1.1") << QString("")
                                        << "Unable to submit contact.\n\nInvalid contact name format.\nInvalid port format.";
    QTest::newRow("name, ip invalid") << QString("") << QString("") << QString("1234")
                                      << "Unable to submit contact.\n\nInvalid contact name format.\nInvalid IP address format.";

    QTest::newRow("port invalid") << QString("abc") << QString("1.1.1.1") << QString("")
                                  << "Unable to submit contact.\n\nInvalid port format.";
    QTest::newRow("name invalid") << QString("") << QString("1.1.1.1") << QString("1234")
                                  << "Unable to submit contact.\n\nInvalid contact name format.";
    QTest::newRow("ip invalid") << QString("abc") << QString("") << QString("1234")
                                << "Unable to submit contact.\n\nInvalid IP address format.";
}

void ContactValidatorTest::validationErrMsgReturnsErrorInfo()
{
    ContactValidator validator{};
    std::unordered_map<std::string, Contact*> contacts{};

    QFETCH(QString, name);
    QFETCH(QString, ip);
    QFETCH(QString, port);
    QFETCH(QString, msg);

    validator.validateContactForm(name, ip, port, contacts);
    QCOMPARE(validator.validationErrMsg(), msg);

}

void ContactValidatorTest::validationErrMsgReturnsIPUniquenessErrorInfo()
{
    ContactValidator validator{};
    std::unordered_map<std::string, Contact*> contacts{};
    Contact added{"John Smith", "127.126.1.1", 1234};
    contacts.insert({"127.126.1.1", &added});

    validator.validateContactForm(QString("Joe Doe"), QString("127.126.1.1"), QString("1111"), contacts);
    QCOMPARE(validator.validationErrMsg(), "Unable to submit contact.\n\nIP occupied by existing contact.");
}

void ContactValidatorTest::validationErrMsgReturnsNameUniquenessErrorInfo()
{
    ContactValidator validator1{};
    ContactValidator validator2{};
    std::unordered_map<std::string, Contact*> contacts{};
    Contact added{"John Smith", "127.126.1.1", 1234};
    contacts.insert({"127.126.1.1", &added});

    validator1.validateContactForm(QString("John Smith"), QString("128.126.1.1"), QString("1111"), contacts);
    QCOMPARE(validator1.validationErrMsg(), "Unable to submit contact.\n\nContact name already exists.");

    validator2.validateContactForm(QString("John Smith"), QString("1111"), contacts);
    QCOMPARE(validator2.validationErrMsg(), "Unable to submit contact.\n\nContact name already exists.");
}

QTEST_APPLESS_MAIN(ContactValidatorTest)

#include "tst_contact_validator_test.moc"
