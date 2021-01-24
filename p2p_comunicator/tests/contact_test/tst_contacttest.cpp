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

    void addToHistory() {
        Contact contact{"name", "22.33.11.44", 1234};

        QCOMPARE(true, contact.getHistory().empty());

        Message message(TCPPacket("example0"));
        contact.addToHistory(message);

        QCOMPARE(1, contact.getHistory().size());

        for(int i=1; i<10; i++) {
            Message message(TCPPacket("example" + std::to_string(i)));
            contact.addToHistory(message);
        }

        QCOMPARE(10, contact.getHistory().size());
    }

    void writeAndRead() {
        Contact contact{"name", "22.33.11.44", 1234};
        Contact contact2{"otherName", "127.0.0.1", 8080};
        QJsonObject object;

        contact.write(object);

        QCOMPARE("otherName", contact2.getName());
        QCOMPARE("127.0.0.1", contact2.getAddress());
        QCOMPARE(8080, contact2.getPort());

        contact2.read(object);

        QCOMPARE("name", contact2.getName());
        QCOMPARE("22.33.11.44", contact2.getAddress());
        QCOMPARE(1234, contact2.getPort());
    }

    void writeAndReadWithHistory() {
        Contact contact{"name", "22.33.11.44", 1234};
        Contact contact2{"otherName", "127.0.0.1", 8080};
        QJsonObject object;

        for(int i=0; i<10; i++) {
            Message message(TCPPacket("example" + std::to_string(i)));
            contact.addToHistory(message);
        }

        contact.write(object);

        QCOMPARE("otherName", contact2.getName());
        QCOMPARE("127.0.0.1", contact2.getAddress());
        QCOMPARE(8080, contact2.getPort());
        QCOMPARE(true, contact2.getHistory().empty());

        contact2.read(object);

        QCOMPARE("name", contact2.getName());
        QCOMPARE("22.33.11.44", contact2.getAddress());
        QCOMPARE(1234, contact2.getPort());
        QCOMPARE(10, contact2.getHistory().size());
    }

};

QTEST_APPLESS_MAIN(ContactTest)

#include "tst_contacttest.moc"
