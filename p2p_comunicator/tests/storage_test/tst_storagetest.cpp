#include <QtTest>
#include "contacts/Storage.h"

class StorageTest : public QObject
{
    Q_OBJECT

public:
    StorageTest() {}
    ~StorageTest() {}

private slots:
    void test_case1() {

    }

};

QTEST_APPLESS_MAIN(StorageTest)

#include "tst_storagetest.moc"
