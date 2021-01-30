#pragma once

#include <string>

#include <QJsonObject>
#include <QDateTime>
#include <QFile>
#include <QDir>

#include <QObject>
#include <tcp/TCPPacket.h>
#include <config/Config.h>
#include <util/strbuilder.h>

using namespace std;

class Message : public QObject{
    Q_OBJECT
public:
    enum Type {
        TEXT, FILE
    };

    Message(TCPPacket packet, QObject* parent = nullptr);
    Message(QJsonObject& object, QObject* parent = nullptr);
    QJsonObject serialize();
    void downloadFile();
    void save();
    string getTimestamp();

    string& getContent() {
        return content;
    }

    string& getFilename() {
        return filename;
    }

    Type getType() {
        return type;
    }

    string& getAddress() {
        return address;
    }

    Message& withAddress(const string& sender) {
        this->address = sender;
        return *this;
    }

private:
    QDateTime timestamp;
    Type type;
    string content;
    string filename;
    string address;

    Message(const string& address, QDateTime timestamp, Type type, const string& content, const string& filename) : address(address), timestamp(timestamp), type(type), content(content), filename(filename) {}

};

Q_DECLARE_METATYPE(Message*)
