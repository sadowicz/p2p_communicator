#include <TCPPacket.h>

#define TEXT_PACKET_HEADER "TEXT"
#define FILE_PACKET_HEADER "FILE"
#define CONNECTION_PACKET_HEADER "CONNECTION"

TCPPacket TCPPacket::decode(std::string packet) {
    if (packet.empty()) {
        throw TCPException("Packet decoding failed: packet was empty");
    }

    packet = QByteArray::fromBase64(QByteArray(packet.c_str())).toStdString();

    const char* cstr = packet.c_str();
    char filename[260] = "";
    char port[20] = "";

    if (tryParseConnectionPacket(cstr, port)) {
        return TCPPacket(packet)
                .withContent(std::string(port))
                .withType(PacketType::CONNECTION);
    } else if (tryParseFilePacket(cstr, filename)) {
        const char* content = getContentFromRaw(cstr);
        return TCPPacket(packet)
                .withContent(std::string(content + 1))
                .withFilename(std::string(filename))
                .withType(PacketType::FILE);
    } else if (tryParseTextPacket(cstr)) {
        const char* content = getContentFromRaw(cstr);
        return TCPPacket(packet)
                .withContent(std::string(content + 1))
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
    return QByteArray(packet.c_str()).toBase64().toStdString();
}

const char* TCPPacket::getContentFromRaw(const char* cstr) {
    const char* content = strchr(cstr, '>') + 1;

    if (content == NULL || content[0] == '\0') {
        throw TCPException("Packet decoding failed: no content");
    }
    if (content[0] != ' ') {
        throw TCPException("Packet decoding failed: no space after header");
    }
    return content;
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



