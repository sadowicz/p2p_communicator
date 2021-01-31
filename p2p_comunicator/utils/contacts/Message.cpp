#include <contacts/Message.h>

using namespace contacts;

Message* Message::createTextMessage(string content, QObject* parent) {
    return new Message(TEXT, "", content, Message::ME, parent);
}

Message* Message::createFileMessage(string filename, QObject* parent) {
    return new Message(FILE, filename, "", Message::ME, parent);
}

Message::Message(Type type, string filename, string content, Sender sender, QObject* parent)
    : QObject(parent),
      timestamp(QDateTime::currentDateTime()),
      type(type),
      sender(sender),
      content(content),
      filename(filename),
      log(util::getLogger())
{}

Message::Message(TCPPacket packet, QObject* parent)
        : QObject(parent){
    log = util::getLogger();
    this->type = packet.getType() == TCPPacket::PacketType::TEXT
            ? Message::Type::TEXT
            : Message::Type::FILE;

    this->sender = Sender::CONTACT;
    this->timestamp = QDateTime::currentDateTime();
    this->content = packet.getContent();
    this->filename = packet.getFilename();
}

Message::Message(QJsonObject& object, QObject* parent)
        : QObject(parent) {
    log = util::getLogger();
    this->type = object["type"].toString().toStdString() == "TEXT"
            ? Message::Type::TEXT
            : Message::Type::FILE;

    this->sender = object["sender"].toString() == "CONTACT"
            ? Sender::CONTACT
            : Sender::ME;

    this->filename = object["filename"].toString().toStdString();
    this->content = object["content"].toString().toStdString();
    this->timestamp = QDateTime::fromString(object["timestamp"].toString());
}

QJsonObject Message::serialize() {
    QJsonObject object{};

    object["type"] = getType() == Message::Type::TEXT
            ? QString("TEXT")
            : QString("FILE");

    object["sender"] = sender == Sender::CONTACT
            ? QString("CONTACT")
            : QString("ME");

    object["timestamp"] = QString(getTimestamp().c_str());

    if (getType() == Message::Type::TEXT) {
        object["content"] = QString(getContent().c_str());
    } else {
        object["filename"] = QString(getFilename().c_str());
    }
    return object;
}

void Message::save() {
    if (type == FILE) {
        QDir dir;
        QString dirPath = Config::config("downloads-directory").c_str();
        if(!dir.exists(dirPath)) {
            dir.mkpath(dirPath);
        }

        QString filename = QString(dirPath + QString::fromStdString("/") + QString::fromStdString(getFilename()));

        ofstream file(filename.toUtf8());
        if (!file.is_open()) {
            throw new IOException("Failed saving file.");
        }
        file << content;
        file.close();

        log.info("Saved file '" + filename.toStdString() + "'");
    }
}

string Message::getTimestamp() const {
    return timestamp.toString().toStdString();
}
