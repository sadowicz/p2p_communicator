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

class Contact : public QAbstractListModel{

    Q_OBJECT

public:

    Contact() {}
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

    // overrided model functions
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    std::string name;
    std::string address;
    int port;
    std::vector<Message*> history;

};
