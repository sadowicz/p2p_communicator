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

class Message : public QObject {
        Q_OBJECT

public:
    enum Type {
        TEXT, FILE
    };

    enum Sender{
        ME, CONTACT
    };

    static Message* createTextMessage(string* content, QObject* parent = nullptr);
    static Message* createFileMessage(string filename, QObject* parent = nullptr);
    Message(TCPPacket packet, QObject* parent = nullptr);
    Message(QJsonObject& object, QObject* parent = nullptr);
    QJsonObject serialize();
    void save();
    string getTimestamp() const;

    void setSavedState(bool saved);
    bool isSaved();

    string* getContent() const {
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

    ~Message() {
        delete content;
    }


private:
    QDateTime timestamp;
    Type type;
    Sender sender;
    string* content;
    string filename;
    Logger log;
    bool saved;

    Message(Type type, string filename, string* content, Sender sender, QObject* parent = nullptr, bool saved = true);

};


}

Q_DECLARE_METATYPE(contacts::Message*)
