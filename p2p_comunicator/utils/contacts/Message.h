#pragma once

#include <string>

#include <QJsonObject>
#include <QDateTime>

#include <tcp/TCPPacket.h>
#include <config/Config.h>
#include <util/strbuilder.h>

using namespace std;

class Message {

public:
    enum Type {
        TEXT, FILE
    };

    static Message* fromTCPPacket(TCPPacket* packet);
    static Message* deserialize(QJsonObject& object);
    Message(QDateTime timestamp, Type type, string content, string filename) : timestamp(timestamp), type(type), content(content), filename(filename) {}
    void serialize(QJsonObject& object);
    void downloadFile();
    string getTimestamp();

    string getContent() {
        return content;
    }

    string getFilename() {
        return filename;
    }

    Type getType() {
        return type;
    }

private:
    QDateTime timestamp;
    Type type;
    string content;
    string filename;

};
