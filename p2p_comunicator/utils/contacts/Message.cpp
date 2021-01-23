#include <contacts/Message.h>

Message* Message::fromTCPPacket(TCPPacket* packet) {
    Message::Type type = packet->getType() == TCPPacket::PacketType::TEXT
            ? Message::Type::TEXT
            : Message::Type::FILE;
    Message* msg = new Message(QDateTime::currentDateTime(), type, packet->getContent(), packet->getFilename());
    return msg;
}

void Message::downloadFile() {
    string fullPath = (strbuilder() + Config::get("downloads-directory") + "/" + filename).get();
    //TODO: download file
}

Message* Message::deserialize(QJsonObject& object) {
    Message::Type type = object["type"].toString().toStdString() == "TEXT"
            ? Message::Type::TEXT
            : Message::Type::FILE;
    string filename = object["filename"].toString().toStdString();
    string content = object["content"].toString().toStdString();
    QDateTime ts = QDateTime::fromString(object["timestamp"].toString());
    return new Message(ts, type, content, filename);
}

void Message::serialize(QJsonObject& object) {
    object["type"] = getType() == Message::Type::TEXT
            ? QString("TEXT")
            : QString("FILE");
    object["timestamp"] = QString(getTimestamp().c_str());
    if (getType() == Message::Type::TEXT) {
        object["content"] = QString(getContent().c_str());
    } else {
        object["filename"] = QString(getFilename().c_str());
    }
}

string Message::getTimestamp() {
    return timestamp.toString().toStdString();
}
