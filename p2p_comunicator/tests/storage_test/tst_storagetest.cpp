#include <QtTest>
#include "contacts/Storage.h"

class StorageTest : public QObject
{
    Q_OBJECT

public:
    StorageTest() {}
    ~StorageTest() {}

private slots:
    void addContactAndGetContacts() {
        Storage& storage = Storage::storage();

        std::string name = "name";
        std::string address = "127.0.0.1";
        unsigned int port = 1234;

        std::string name2 = "other name";
        std::string address2 = "22.77.34.12";
        unsigned int port2 = 3412;

        QCOMPARE(0, storage.getContacts().size());

        try {
            storage.addContact(Contact{name, address, port});
        } catch(std::out_of_range&) {}

        auto contacts = storage.getContacts();

        QCOMPARE(1, contacts.size());

        QCOMPARE(name, contacts[address].getName());
        QCOMPARE(address, contacts[address].getAddress());
        QCOMPARE(port, contacts[address].getPort());

        try {
            storage.addContact(Contact{name2, address2, port2});
        } catch(std::out_of_range&) {}

        contacts = storage.getContacts();

        QCOMPARE(2, contacts.size());

        QCOMPARE(name, contacts[address].getName());
        QCOMPARE(address, contacts[address].getAddress());
        QCOMPARE(port, contacts[address].getPort());

        QCOMPARE(name2, contacts[address2].getName());
        QCOMPARE(address2, contacts[address2].getAddress());
        QCOMPARE(port2, contacts[address2].getPort());

        storage.clear();
    }

    void contactExists() {
           Storage& storage = Storage::storage();

           std::string name = "name";
           std::string address = "37.21.90.17";
           unsigned int port = 1234;

           std::string name2 = "name2";
           std::string address2 = "10.20.30.73";
           unsigned int port2 = 4321;

           QCOMPARE(0, storage.getContacts().size());

           QCOMPARE(false, storage.contactExists(address));
           QCOMPARE(false, storage.contactExists(address2));

           try {
               storage.addContact(Contact{name, address, port});
           } catch(std::out_of_range&) {}

           QCOMPARE(true, storage.contactExists(address));
           QCOMPARE(false, storage.contactExists(address2));

           try {
               storage.addContact(Contact{name2, address2, port2});
           } catch(std::out_of_range&) {}

           QCOMPARE(true, storage.contactExists(address));
           QCOMPARE(true, storage.contactExists(address2));

           storage.clear();
       }

    void getContact() {
        Storage& storage = Storage::storage();

        std::string name = "name";
        std::string address = "11.22.33.44";
        unsigned int port = 8175;

        std::string name2 = "other name";
        std::string address2 = "74.92.76.33";
        unsigned int port2 = 5342;

        QCOMPARE(0, storage.getContacts().size());

        try {
            storage.addContact(Contact{name, address, port});
        } catch(std::out_of_range&) {}
        try {
            storage.addContact(Contact{name2, address2, port2});
        } catch(std::out_of_range&) {}

        QCOMPARE(2, storage.getContacts().size());

        auto contact = storage.getContact(address);

        QCOMPARE(name, contact->getName());
        QCOMPARE(address, contact->getAddress());
        QCOMPARE(port, contact->getPort());

        contact = storage.getContact(address2);

        QCOMPARE(name2, contact->getName());
        QCOMPARE(address2, contact->getAddress());
        QCOMPARE(port2, contact->getPort());

        storage.clear();
    }

    void deleteContact() {
        Storage& storage = Storage::storage();

        std::string name = "name";
        std::string address = "11.22.33.44";
        unsigned int port = 8175;

        std::string name2 = "other name";
        std::string address2 = "74.92.76.33";
        unsigned int port2 = 5342;

        QCOMPARE(0, storage.getContacts().size());

        try {
            storage.addContact(Contact{name, address, port});
        } catch(std::out_of_range&) {}
        try {
            storage.addContact(Contact{name2, address2, port2});
        } catch(std::out_of_range&) {}

        QCOMPARE(2, storage.getContacts().size());
        QCOMPARE(true, storage.contactExists(address));
        QCOMPARE(true, storage.contactExists(address2));

        try {
            storage.deleteContact(address);
        } catch (std::out_of_range) {}

        QCOMPARE(1, storage.getContacts().size());
        QCOMPARE(false, storage.contactExists(address));
        QCOMPARE(true, storage.contactExists(address2));

        try {
            storage.deleteContact(address2);
        } catch (std::out_of_range) {}

        QCOMPARE(0, storage.getContacts().size());
        QCOMPARE(false, storage.contactExists(address));
        QCOMPARE(false, storage.contactExists(address2));
    }

    void saveAndLoad() {
        Storage& storage = Storage::storage();

        std::string name = "name";
        std::string address = "11.22.33.44";
        unsigned int port = 8175;

        std::string name2 = "other name";
        std::string address2 = "74.92.76.33";
        unsigned int port2 = 5342;

        QCOMPARE(0, storage.getContacts().size());

        try {
            storage.save();
        } catch(std::out_of_range&) {}
        try {
            storage.load();
        } catch(std::out_of_range&) {}

        QCOMPARE(0, storage.getContacts().size());

        try {
            storage.addContact(Contact{name, address, port});
        }
        catch(std::out_of_range&) {}

        try {
            storage.save();
        } catch(std::out_of_range&) {}

        try {
            storage.load();
        } catch(std::out_of_range&) {}

        QCOMPARE(1, storage.getContacts().size());
        QCOMPARE(true, storage.contactExists(address));
        QCOMPARE(false, storage.contactExists(address2));

        try {
            storage.addContact(Contact{name2, address2, port2});
        }
        catch(std::out_of_range&) {}

        try {
            storage.save();
        } catch(std::out_of_range&) {}

        try {
            storage.load();
        } catch(std::out_of_range&) {}

        QCOMPARE(2, storage.getContacts().size());
        QCOMPARE(true, storage.contactExists(address));
        QCOMPARE(true, storage.contactExists(address2));

        storage.clear();
    }
};

QTEST_APPLESS_MAIN(StorageTest)

#include "tst_storagetest.moc"
