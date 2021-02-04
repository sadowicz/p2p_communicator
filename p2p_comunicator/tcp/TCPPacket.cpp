#include <TCPPacket.h>

#define TEXT_PACKET_HEADER "TEXT"
#define FILE_PACKET_HEADER "FILE"
#define CONNECTION_PACKET_HEADER "CONNECTION"

TCPPacket TCPPacket::decode(std::string packet) {
    if (packet.empty()) {
        throw TCPException("Packet decoding failed: packet was empty");
    }

    packet = QByteArray::fromBase64(QByteArray::fromStdString(packet)).toStdString();

    const char* cstr = packet.c_str();
    char filename[260] = "";
    char port[20] = "";

    if (tryParseConnectionPacket(cstr, port)) {
        return TCPPacket(packet)
                .withContent(std::string(port))
                .withType(PacketType::CONNECTION);
    } else if (tryParseFilePacket(cstr, filename)) {
        return TCPPacket(packet)
                .withContent(getContentFromPacket(packet))
                .withFilename(std::string(filename))
                .withType(PacketType::FILE);
    } else if (tryParseTextPacket(cstr)) {
        return TCPPacket(packet)
                .withContent(getContentFromPacket(packet))
                .withType(PacketType::TEXT);
    } else {
        throw TCPException("Packet decoding failed: unknown error");
    }
}

std::string TCPPacket::encode(TCPPacket::PacketType type, std::string filename, std::string content) {
    if (content.empty()) {
        throw TCPException("Packet encoding failed: content cannot be empty");
    }
    std::string packet = "";
    switch (type) {
    case TEXT:          packet = std::string("<" TEXT_PACKET_HEADER "> ") + content; break;
    case FILE:          packet = std::string("<" FILE_PACKET_HEADER ":") + filename + std::string("> ") + content; break;
    case CONNECTION:    packet = std::string("<" CONNECTION_PACKET_HEADER ":" + content + std::string("> ")); break;
    default:            throw TCPException("Packet encoding failed: incorrect packet type");
    }
    std::string encodedPacket = QByteArray::fromStdString(packet).toBase64().toStdString();
    return encodedPacket;
}

std::string TCPPacket::getContentFromPacket(std::string& packet) {
    return packet.substr(packet.find('>') + 2);
}

bool TCPPacket::tryParseFilePacket(const char* cstr, char* filename) {
    char header[20] = "";
    return (sscanf(cstr, "<%19[^:]:%259[^>]>", header, filename) == 2)
            && (strcmp(header, FILE_PACKET_HEADER) == 0);
}

bool TCPPacket::tryParseTextPacket(const char* cstr) {
    char header[20] = "";
    return (sscanf(cstr, "<%19[^>]>", header) == 1)
            && (strcmp(header, TEXT_PACKET_HEADER) == 0);
}

bool TCPPacket::tryParseConnectionPacket(const char* cstr, char* port) {
    char header[20] = "";
    return (sscanf(cstr, "<%19[^:]:%9[^>]>", header, port) == 2)
            && (strcmp(header, CONNECTION_PACKET_HEADER) == 0);
}



