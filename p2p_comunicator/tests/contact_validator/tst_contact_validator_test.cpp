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

QTEST_APPLESS_MAIN(ContactValidatorTest)

#include "tst_contact_validator_test.moc"
