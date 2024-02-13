#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QStyle>
#include "Device_Controller.h"
#include "clients_manager.h"
#include <QListWidgetItem>
#include "client_commands.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client_Window; }
QT_END_NAMESPACE

class Client_Window : public QMainWindow
{
    Q_OBJECT

public:
    Client_Window(QWidget *parent = nullptr);
    ~Client_Window();

private slots:
    void on_InIPAddress_textChanged(const QString &arg1);

    void device_connected();
    void device_disconnected();
    void device_stateChanged(QAbstractSocket::SocketState);
    void device_errorOccurred(QAbstractSocket::SocketError);
    void device_dataReady(QByteArray data);
    void device_c_DataReady(QSharedPointer<Client>);

    void on_btn_connect_clicked();


    void on_btn_send_data__clicked();

    void on_btn_send_data_1_clicked();

    void on_lst_Clients_itemClicked(QListWidgetItem *item);

    void on_client_window_opend(QSharedPointer<Client> client);
     void handleClientCommandsClosing();
    void handleSendToClient(QString message);
private:
    Ui::Client_Window *ui;
    Device_Controller _controller;
    Clients_manager _manager;
    QMap<QSharedPointer<Client>, Client_commands*> client_windows_opend;


//    methods
    void set_device_controller();

};
#endif // CLIENT_WINDOW_H
