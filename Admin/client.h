#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(int _id,QObject *parent = nullptr);
    void stopped();
    void running();


signals:

private:

public:
    QString name = "Unknown";
    int id{0};
    QList<QString> message_history;
    QString status = "NONE";
    QList<QString> automat;
    QList<QString> manual;

};

#endif // CLIENT_H
