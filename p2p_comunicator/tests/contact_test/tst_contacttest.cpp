#include <QtTest>
#include "contacts/Contact.h"


class ContactTest : public QObject
{
    Q_OBJECT

public:
    ContactTest() {}
    ~ContactTest() {}

private slots:
    void constructor() {
        Contact contact{"name", "22.33.11.44", 1234};

        QCOMPARE("name", contact.getName());
        QCOMPARE("22.33.11.44", contact.getAddress());
        QCOMPARE(1234, contact.getPort());
        QCOMPARE(true, contact.getHistory().empty());
    }

};



QTEST_APPLESS_MAIN(ContactTest)

#include "tst_contacttest.moc"
