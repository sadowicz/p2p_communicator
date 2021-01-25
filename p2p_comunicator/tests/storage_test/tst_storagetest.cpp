#include <QtTest>
#include "contacts/Storage.h"

class StorageTest : public QObject
{
    Q_OBJECT

public:
    StorageTest() {}
    ~StorageTest() {}

private slots:
    void addContact() {
        Storage storage{};

        QCOMPARE(0, storage.getContacts().size());

        try {
            storage.addContact(Contact{"name", "127.0.0.1", 1234});
        }
        catch(std::out_of_range&) {}

        auto contacts = storage.getContacts();

        QCOMPARE(1, contacts.size());

        QCOMPARE("name", contacts["127.0.0.1"].getName());
        QCOMPARE("127.0.0.1", contacts["127.0.0.1"].getAddress());
        QCOMPARE(1234, contacts["127.0.0.1"].getPort());

        try {
            storage.addContact(Contact{"other name", "22.77.34.12", 3412});
        }
        catch(std::out_of_range&) {}

        contacts = storage.getContacts();

        QCOMPARE(2, contacts.size());

        QCOMPARE("name", contacts["127.0.0.1"].getName());
        QCOMPARE("127.0.0.1", contacts["127.0.0.1"].getAddress());
        QCOMPARE(1234, contacts["127.0.0.1"].getPort());

        QCOMPARE("other name", contacts["22.77.34.12"].getName());
        QCOMPARE("22.77.34.12", contacts["22.77.34.12"].getAddress());
        QCOMPARE(3412, contacts["22.77.34.12"].getPort());
    }

    void contactExists() {
        Storage storage{};

        QCOMPARE(0, storage.getContacts().size());

        std::string address = "37.21.90.17";
        std::string address2 = "10.20.30.73";

        QCOMPARE(false, storage.contactExists(address));
        QCOMPARE(false, storage.contactExists(address2));

        try {
            storage.addContact(Contact{"name", address, 1234});
        }
        catch(std::out_of_range&) {}

        QCOMPARE(true, storage.contactExists(address));
        QCOMPARE(false, storage.contactExists(address2));

        try {
            storage.addContact(Contact{"name", address2, 1234});
        }
        catch(std::out_of_range&) {}

        QCOMPARE(true, storage.contactExists(address));
        QCOMPARE(true, storage.contactExists(address2));
    }

    void getContact() {
        Storage storage{};

        QCOMPARE(0, storage.getContacts().size());

        std::string address = "11.22.33.44";

        try {
            storage.addContact(Contact{"name", address, 8175});
        }
        catch(std::out_of_range&) {}

        QCOMPARE(1, storage.getContacts().size());

        auto contact = storage.getContact(address);

        QCOMPARE("name", contact.getName());
        QCOMPARE(address, contact.getAddress());
        QCOMPARE(8175, contact.getPort());
    }
};

QTEST_APPLESS_MAIN(StorageTest)

#include "tst_storagetest.moc"
