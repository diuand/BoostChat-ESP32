#ifndef CLIENTS_MANAGER_H
#define CLIENTS_MANAGER_H

#include <QObject>
#include <QDebug>
#include "client.h"
class Clients_manager : public QObject
{
    Q_OBJECT
public:
    explicit Clients_manager(QObject *parent = nullptr);

//    client manipulation
    void add_client(int id);
    void remove_client(int _id);
    void display_cliets();
    void set_client_name(QList<QByteArray> txt_parts);
    void set_client_status(QList<QByteArray> txt_parts);
    void set_client_auto(QList<QByteArray> txt_parts);
    void set_client_manual(QList<QByteArray> txt_parts);


//    input text verification
    bool temp_text_verify(QList<QByteArray> txt_parts);
    bool conn_txt_verify(QList<QByteArray> txt_parts);
    bool disc_txt_verify(QList<QByteArray> txt_parts);
    bool name_def_txt_verify(QList<QByteArray> txt_parts);
    bool commands_verify_m(QList<QByteArray> txt_parts);
    bool commands_verify_a(QList<QByteArray> txt_parts);
    bool commands_verify_s(QList<QByteArray> txt_parts);

//    client history
    void add_client_history(QString data);


signals:
    void c_DataReady(QSharedPointer<Client>);
public:
    QList<QSharedPointer<Client>>  connected_clients;
    int _clients_limit {100};
};

#endif // CLIENTS_MANAGER_H
