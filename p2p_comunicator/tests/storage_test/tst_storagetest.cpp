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
        QCOMPARE(1234, contacts["127.0.0.1"].getPort());

    }

};

QTEST_APPLESS_MAIN(StorageTest)

#include "tst_storagetest.moc"
