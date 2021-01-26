#include <QtTest>
#include "../../utils/contacts/Contact.h"

class ContactTest : public QObject
{
    Q_OBJECT

public:
    ContactTest();
    ~ContactTest();

private slots:
    void test_case1();

};

QTEST_APPLESS_MAIN(ContactTest)

#include "tst_contact_test.moc"
