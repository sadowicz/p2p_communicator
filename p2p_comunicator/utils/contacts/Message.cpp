#include <contacts/Message.h>

Message::Message(TCPPacket packet) {
    this->type = packet.getType() == TCPPacket::PacketType::TEXT
            ? Message::Type::TEXT
            : Message::Type::FILE;
    this->timestamp = QDateTime::currentDateTime();
    this->content = packet.getContent();
    this->filename = packet.getFilename();
}

Message::Message(QJsonObject& object) {
    this->type = object["type"].toString().toStdString() == "TEXT"
            ? Message::Type::TEXT
            : Message::Type::FILE;
    this->filename = object["filename"].toString().toStdString();
    this->content = object["content"].toString().toStdString();
    this->timestamp = QDateTime::fromString(object["timestamp"].toString());
}

void Message::downloadFile() {
    // string fullPath = strbuilder() << Config::get("downloads-directory") << "/" << filename << strbuilder::end();
    //TODO: download file
}

QJsonObject Message::serialize() {
    QJsonObject object{};
    object["type"] = getType() == Message::Type::TEXT
            ? QString("TEXT")
            : QString("FILE");
    object["timestamp"] = QString(getTimestamp().c_str());
    if (getType() == Message::Type::TEXT) {
        object["content"] = QString(getContent().c_str());
    } else {
        object["filename"] = QString(getFilename().c_str());
    }
    return object;
}

string Message::getTimestamp() {
    return timestamp.toString().toStdString();
}
