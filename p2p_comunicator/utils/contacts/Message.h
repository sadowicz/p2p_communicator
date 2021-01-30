#pragma once

#include <string>

#include <QJsonObject>
#include <QDateTime>
#include <QFile>
#include <QDir>

#include <QObject>
#include <config/Config.h>

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
    string* address;
    Logger log;

};

}

Q_DECLARE_METATYPE(Message*)
