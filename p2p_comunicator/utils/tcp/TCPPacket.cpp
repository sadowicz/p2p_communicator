#include <tcp/TCPPacket.h>

#define TEXT_PACKET_HEADER "TEXT"
#define FILE_PACKET_HEADER "FILE"

TCPPacket TCPPacket::decode(std::string packet) {
    if (packet.empty()) {
        throw TCPException("Packet decoding failed: packet was empty");
    }
    const char* cstr = packet.c_str();
    char filename[260] = "";

    if (tryParseFilePacket(cstr, filename)) {
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

    if (type == PacketType::TEXT) {
        return strbuilder() + "<" TEXT_PACKET_HEADER "> " + content + strbuilder::end();

    } else if (type == PacketType::FILE) {
        return strbuilder() + "<" FILE_PACKET_HEADER ":" + filename + "> " + content + strbuilder::end();

    } else {
        throw TCPException("Packet encoding failed: incorrect packet type");
    }
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
    char header[10] = "";
    int parsingResult = sscanf(cstr, "<%9[^:]:%259[^>]>", header, filename);

    if (parsingResult == 2) {
        if (strcmp(header, FILE_PACKET_HEADER) != 0) {
            throw TCPException("Packet decoding failed: packet header incorrect, use " FILE_PACKET_HEADER);
        } else {
            return true;
        }
    }
    return false;
}

bool TCPPacket::tryParseTextPacket(const char* cstr) {
    char header[10] = "";
    int parsingResult = sscanf(cstr, "<%9[^>]>", header);

    if (parsingResult == 1) {
        if (strcmp(header, TEXT_PACKET_HEADER) != 0) {
            throw TCPException("Packet decoding failed: packet header incorrect, use " TEXT_PACKET_HEADER);
        } else {
            return true;
        }
    }
    return false;
}



