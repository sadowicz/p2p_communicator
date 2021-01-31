#pragma once

#include <string>

#include <QJsonObject>
#include <QDateTime>
#include <QFile>
#include <QDir>

#include <QObject>
#include <config/Config.h>
#include <util/LoggerHelper.h>
#include <TCPPacket.h>
#include <Logger.h>

using namespace std;

namespace contacts {

class Message : public QObject{
        Q_OBJECT

public:
    enum Type {
        TEXT, FILE
    };

    static Message* createTextMessage(string content, QObject* parent = nullptr);
    static Message* createFileMessage(string filename, QObject* parent = nullptr);
    Message(TCPPacket packet, QObject* parent = nullptr);
    Message(QJsonObject& object, QObject* parent = nullptr);
    QJsonObject serialize();
    void save();
    string getTimestamp() const;

    string getContent() const {
        return content;
    }

    string getFilename() const {
        return filename;
    }

    Type getType() const {
        return type;
    }

    string getAddress() const {
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
    Logger log;

    Message(Type type, string filename, string content, string address, QObject* parent = nullptr);

};


}

Q_DECLARE_METATYPE(contacts::Message*)
