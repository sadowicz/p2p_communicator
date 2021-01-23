#include <QString>
#include <QtTest>

#include <tcp/TCPPacket.h>

class tcp_packet_test : public QObject
{
    Q_OBJECT

public:
    tcp_packet_test();

private Q_SLOTS:
    void encodeFilePacket();
    void encodeTextPacket();
    void decodeFilePacket();
    void decodeTextPacket();

    void encodeIncorrectPacket_incorrectPacketType();
    void decodeIncorrectFilePacket_noSemicolonAfterHeader();
    void decodeIncorrectTextPacket_noSpaceAfterHeader();
    void decodeIncorrectFilePacket_emptyFilename();
    void decodeIncorrectFilePacket_noContent();
};

tcp_packet_test::tcp_packet_test(){
}

void tcp_packet_test::decodeFilePacket() {
    // - DATA -
    std::string raw("<FILE:test.txt> some content");
    std::string content("some content");
    std::string filename("test.txt");
    TCPPacket::PacketType type = TCPPacket::PacketType::FILE;

    // - TEST -
    TCPPacket* actual = TCPPacket::decode(raw);

    // - ASSERT -
    QCOMPARE(raw, actual->getRaw());
    QCOMPARE(content, actual->getContent());
    QCOMPARE(type, actual->getType());
    QCOMPARE(filename, actual->getFilename());

    // - CLEANUP -
    delete actual;
}

void tcp_packet_test::decodeTextPacket() {
    // - DATA -
    std::string raw("<TEXT> some content");
    std::string content("some content");
    TCPPacket::PacketType type = TCPPacket::PacketType::TEXT;

    // - TEST -
    TCPPacket* actual = TCPPacket::decode(raw);

    // - ASSERT -
    QCOMPARE(raw, actual->getRaw());
    QCOMPARE(content, actual->getContent());
    QCOMPARE(type, actual->getType());

    // - CLEANUP -
    delete actual;
}

void tcp_packet_test::encodeFilePacket() {
    // - DATA -
    std::string raw("<FILE:test.txt> some content");
    std::string content("some content");
    std::string filename("test.txt");
    TCPPacket::PacketType type = TCPPacket::PacketType::FILE;

    // - TEST -
    std::string encoded = TCPPacket::encode(type, filename, content);

    // - ASSERT -
    QCOMPARE(raw, encoded);
}

void tcp_packet_test::encodeTextPacket() {
    // - DATA -
    std::string raw("<TEXT> some content");
    std::string content("some content");
    TCPPacket::PacketType type = TCPPacket::PacketType::TEXT;

    // - TEST -
    std::string encoded = TCPPacket::encode(type, "", content);

    // - ASSERT -
    QCOMPARE(raw, encoded);
}

void tcp_packet_test::encodeIncorrectPacket_incorrectPacketType() {
    // - DATA -
    std::string raw("<TEXT> some content");
    std::string content("some content");
    TCPPacket::PacketType type = (TCPPacket::PacketType) 534564;

    // - TEST -
    std::string encoded = "";
    try {
        std::string encoded = TCPPacket::encode(type, "", content);
    } catch (TCPException& e) {
        return;
    }

    // - ASSERT -
    QCOMPARE(raw, encoded);
}

void tcp_packet_test::decodeIncorrectFilePacket_noSemicolonAfterHeader() {
    // - DATA -
    std::string raw("<FILEtest.txt> some content");
    std::string content("some content");
    std::string filename("test.txt");
    TCPPacket::PacketType type = TCPPacket::PacketType::FILE;

    // - TEST -
    TCPPacket* actual = nullptr;
    try {
        actual = TCPPacket::decode(raw);
    } catch (TCPException& e) {
        return;
    }

    // - ASSERT -
    QCOMPARE(raw, actual->getRaw());
    QCOMPARE(content, actual->getContent());
    QCOMPARE(type, actual->getType());
    QCOMPARE(filename, actual->getFilename());

    // - CLEANUP -
    delete actual;
}

void tcp_packet_test::decodeIncorrectTextPacket_noSpaceAfterHeader() {
    // - DATA -
    std::string raw("<TEXT>some content");
    std::string content("some content");
    TCPPacket::PacketType type = TCPPacket::PacketType::TEXT;

    // - TEST -
    TCPPacket* actual = nullptr;
    try {
        actual = TCPPacket::decode(raw);
    } catch (TCPException& e) {
        return;
    }

    // - ASSERT -
    QCOMPARE(raw, actual->getRaw());
    QCOMPARE(content, actual->getContent());
    QCOMPARE(type, actual->getType());

    // - CLEANUP -
    delete actual;
}

void tcp_packet_test::decodeIncorrectFilePacket_emptyFilename() {
    // - DATA -
    std::string raw("<FILE:> some content");
    std::string content("some content");
    std::string filename("test.txt");
    TCPPacket::PacketType type = TCPPacket::PacketType::FILE;

    // - TEST -
    TCPPacket* actual = nullptr;
    try {
        actual = TCPPacket::decode(raw);
    } catch (TCPException& e) {
        return;
    }

    // - ASSERT -
    QCOMPARE(raw, actual->getRaw());
    QCOMPARE(content, actual->getContent());
    QCOMPARE(type, actual->getType());
    QCOMPARE(filename, actual->getFilename());

    // - CLEANUP -
    delete actual;
}

void tcp_packet_test::decodeIncorrectFilePacket_noContent() {
    // - DATA -
    std::string raw("<FILE:test.txt>");
    std::string content("some content");
    std::string filename("test.txt");
    TCPPacket::PacketType type = TCPPacket::PacketType::FILE;

    // - TEST -
    TCPPacket* actual = nullptr;
    try {
        actual = TCPPacket::decode(raw);
    } catch (TCPException& e) {
        return;
    }

    // - ASSERT -
    QCOMPARE(raw, actual->getRaw());
    QCOMPARE(content, actual->getContent());
    QCOMPARE(type, actual->getType());
    QCOMPARE(filename, actual->getFilename());

    // - CLEANUP -
    delete actual;
}


QTEST_APPLESS_MAIN(tcp_packet_test)

#include "tst_tcp_packet_test.moc"
