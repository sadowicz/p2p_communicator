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

    enum Sender{
        ME, CONTACT
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

    Sender getSender() const{
        return sender;
    }


private:
    QDateTime timestamp;
    Type type;
    Sender sender;
    string content;
    string filename;
    Logger log;

};


}

Q_DECLARE_METATYPE(contacts::Message*)
