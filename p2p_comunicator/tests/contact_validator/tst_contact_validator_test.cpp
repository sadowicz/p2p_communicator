#include <QtTest>

#include "../../utils/contact_validator.h"

class ContactValidatorTest : public QObject
{
    Q_OBJECT

public:
    ContactValidatorTest();
    ~ContactValidatorTest();

private slots:
    void contactValidatorIsCreateable();
    void constructorSetsErrMsgHeader();

    void emptyFiledMakeFormInvalid_data();
    void emptyFiledMakeFormInvalid();

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

void ContactValidatorTest::emptyFiledMakeFormInvalid_data()
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

void ContactValidatorTest::emptyFiledMakeFormInvalid()
{
    ContactValidator validator{};

    QFETCH(QString, name);
    QFETCH(QString, ip);
    QFETCH(QString, port);
    QFETCH(bool, result);

    QCOMPARE(validator.validateContactForm(name, ip, port), result);
}

QTEST_APPLESS_MAIN(ContactValidatorTest)

#include "tst_contact_validator_test.moc"
