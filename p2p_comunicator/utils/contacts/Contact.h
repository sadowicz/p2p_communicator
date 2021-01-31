#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QAbstractListModel>
#include <QPointer>

#include <contacts/Message.h>
#include <vector>
namespace contacts {

class Contact : public QAbstractListModel{

    Q_OBJECT

public:

    Contact() : name(""), address(""), port(0), active(false), unreadMsg(false) {}
    Contact(std::string name, std::string address, unsigned port, QObject* parent = nullptr);

    void read(const QJsonObject &json);
    void write(QJsonObject &json);

    void addToHistory(Message* message);
    std::vector<Message*>& getHistory() {
        return history;
    }

    std::string getName() const;
    std::string getAddress() const;
    int getPort() const;
    bool isActive() const;
    bool hasUnreadMsg() const;
    void setActiveState(bool state);
    void setUnreadMsgState(bool state);

    // overrided model functions
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    std::string name;
    std::string address;
    unsigned port;
    std::vector<Message*> history;
    bool active;
    bool unreadMsg;
};

}
