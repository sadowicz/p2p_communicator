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

    void validationErrMsgReturnsErrorInfo_data();
    void validationErrMsgReturnsErrorInfo();
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
    QCOMPARE(validator.validationErrMsg(), QString("Unable to add new contact.\n"));
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

    QFETCH(QString, name);
    QFETCH(QString, ip);
    QFETCH(QString, port);
    QFETCH(bool, result);

    QCOMPARE(validator.validateContactForm(name, ip, port), result);
}

void ContactValidatorTest::allFieldsValidReturnTrue()
{
    ContactValidator validator{};
    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("1111")), true);
}

void ContactValidatorTest::nameShorterThan25ReturnTrue()
{
    ContactValidator validator{};
    QString name = "a";

    for(int i = 1; i <= 25; i++, name += "a")
        QCOMPARE(validator.validateContactForm(name, QString("127.0.0.1"), QString("1111")), true);
}

void ContactValidatorTest::nameLongerThan25ReturnFalse()
{
    ContactValidator validator{};
    QCOMPARE(validator.validateContactForm(QString("aaaaaaaaaaaaaaaaaaaaaaaaaaa"), QString("127.0.0.1"), QString("1111")), false);
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

    QFETCH(QString, ip);
    QFETCH(bool, result);

    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), ip, QString("1234")), result);
}

void ContactValidatorTest::portInRangeReturnTrue()
{
    ContactValidator validator{};

    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("0")), true);
    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("1234")), true);
    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("27895")), true);
    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("65535")), true);
}

void ContactValidatorTest::portOutOfRangeReturnFalse()
{
    ContactValidator validator{};

    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("-1")), false);
    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("65536")), false);
}

void ContactValidatorTest::nonNumericPortReturnFalse()
{
    ContactValidator validator{};

    QCOMPARE(validator.validateContactForm(QString("Joe Doe"), QString("127.0.0.1"), QString("abcd")), false);
}

void ContactValidatorTest::validationErrMsgReturnsErrorInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("ip");
    QTest::addColumn<QString>("port");
    QTest::addColumn<QString>("msg");

    QTest::newRow("all invalid") << QString("") << QString("") << QString("")
                                 << "Unable to add new contact.\n\nInvalid contact name format.\nInvalid IP address format.\nInvalid port format.";

    QTest::newRow("ip, port invalid") << QString("abc") << QString("") << QString("")
                                      << "Unable to add new contact.\n\nInvalid IP address format.\nInvalid port format.";
    QTest::newRow("name, port invalid") << QString("") << QString("1.1.1.1") << QString("")
                                        << "Unable to add new contact.\n\nInvalid contact name format.\nInvalid port format.";
    QTest::newRow("name, ip invalid") << QString("") << QString("") << QString("1234")
                                      << "Unable to add new contact.\n\nInvalid contact name format.\nInvalid IP address format.";

    QTest::newRow("port invalid") << QString("abc") << QString("1.1.1.1") << QString("")
                                  << "Unable to add new contact.\n\nInvalid port format.";
    QTest::newRow("name invalid") << QString("") << QString("1.1.1.1") << QString("1234")
                                  << "Unable to add new contact.\n\nInvalid contact name format.";
    QTest::newRow("ip invalid") << QString("abc") << QString("") << QString("1234")
                                << "Unable to add new contact.\n\nInvalid IP address format.";
}

void ContactValidatorTest::validationErrMsgReturnsErrorInfo()
{
    ContactValidator validator{};

    QFETCH(QString, name);
    QFETCH(QString, ip);
    QFETCH(QString, port);
    QFETCH(QString, msg);

    validator.validateContactForm(name, ip, port);
    QCOMPARE(validator.validationErrMsg(), msg);
}

QTEST_APPLESS_MAIN(ContactValidatorTest)

#include "tst_contact_validator_test.moc"
