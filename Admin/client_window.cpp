#include "client_window.h"
#include "ui_client_window.h"
#include "ui_client_commands.h"
#include <QHostAddress>
#include <QStyle>
#include <QDebug>
#include <QMetaEnum>
Client_Window::Client_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client_Window)
{
    ui->setupUi(this);
    set_device_controller();
}

Client_Window::~Client_Window()
{
    delete ui;
}


void Client_Window::on_InIPAddress_textChanged(const QString &arg1)
{
    QString state = "0";
    if(arg1 == "..."){
        state = "";
    }else{
        QHostAddress address(arg1);


        if(QAbstractSocket::IPv4Protocol == address.protocol()){
            state = "1";
        }
        //    apply the changes
        ui->InIPAddress->setProperty("state", state);
        //    change the ui
        ui->InIPAddress->style()->polish(ui->InIPAddress);
    }


}



void Client_Window::device_connected()
{
    ui->lstConsole->addItem("Connected to device");
    ui->btn_connect->setText("Disconnect");
    ui->gr_send_data->setEnabled(true);

    // verify if other devices are connected to the server
    auto message = "Devices-Connected";
    _controller.send(message);

}

void Client_Window::device_disconnected()
{
    ui->lstConsole->addItem("Disconnected from device");
    ui->btn_connect->setText("Connect");
     ui->gr_send_data->setEnabled(false);
}

void Client_Window::device_stateChanged(QAbstractSocket::SocketState state)
{
    QMetaEnum meta_enum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    ui->lstConsole->addItem(meta_enum.valueToKey(state));
}

void Client_Window::device_errorOccurred(QAbstractSocket::SocketError error)
{
    QMetaEnum meta_enum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    ui->lstConsole->addItem(meta_enum.valueToKey(error));
}

void Client_Window::device_dataReady(QByteArray data)
{

    ui->lstConsole->addItem(QString(data));

    // Split the QByteArray using space as the delimiter
    QList<QByteArray> parts = data.split(' ');
    // display the text to the client that send it
    _manager.add_client_history(QString(data));
    // update the client text in the client window
    if(client_windows_opend.size() > 0){
        for (const auto& key : client_windows_opend.keys()) {
            if(parts[0].toLower() == "client" && parts[1].toInt() == key->id ){
                Client_commands* _value = client_windows_opend.value(key);
                Ui::Client_commands* clientCommandsUI =_value->getUI();
                // Check if chat_history is not null before accessing it
                if (clientCommandsUI->chat_history) {
//                    update the history message
                    clientCommandsUI->chat_history->addItem(QString(data));
                    // update the last message
                    clientCommandsUI->lstConsole->clear();
                    clientCommandsUI->lstConsole->addItem(QString(data));
                }

            }
        }
    }

    // do commands with the text
    if(_manager.conn_txt_verify(parts)){

        _manager.add_client(parts[1].toInt()); // add client

    }
    else if( _manager.disc_txt_verify(parts) ) { // remove client
        _manager.remove_client(parts[1].toInt());

    }
    else if(_manager.name_def_txt_verify(parts)){ // client name
        _manager.set_client_name(parts);

    }
    else if(_manager.commands_verify_a(parts)){ // auto commands
        _manager.set_client_auto(parts);

    }
    else if(_manager.commands_verify_m(parts)){ // manual commands
        _manager.set_client_manual(parts);

    }
    else if(_manager.commands_verify_s(parts)){ // status
        _manager.set_client_status(parts);

    }
    else{
        qWarning() << "Nici conn nici disconn" << data;
    }
    //display clients
    ui->lst_Clients->clear();
    _manager.display_cliets();


}

void Client_Window::device_c_DataReady(QSharedPointer<Client> client)
{
    ui->lst_Clients->addItem("Client " + QString::number(client->id) + " online: " + client->name);
}

void Client_Window::set_device_controller()
{
    connect(&_controller,&Device_Controller::connected,this,&Client_Window::device_connected);
    connect(&_controller,&Device_Controller::disconnected,this,&Client_Window::device_disconnected);
    connect(&_controller,&Device_Controller::errorOccurred,this,&Client_Window::device_errorOccurred);
    connect(&_controller,&Device_Controller::stateChanged,this,&Client_Window::device_stateChanged);
    connect(&_controller,&Device_Controller::dataReady,this,&Client_Window::device_dataReady);
    connect(&_manager,&Clients_manager::c_DataReady,this,&Client_Window::device_c_DataReady);
}

void Client_Window::on_btn_connect_clicked()
{
    if(_controller.is_connected()){
        _controller.Disonnect_from_device();
    }
    else{
        auto ip = ui->InIPAddress->text();
        auto port = ui->spinPort->value();
        _controller.Connect_to_device(ip,port);
    }
}





void Client_Window::on_btn_send_data__clicked()
{
    auto message = ui->InData_2->text().trimmed();
    _controller.send(message);
}


void Client_Window::on_lst_Clients_itemClicked(QListWidgetItem *item)
{

    // display the client command box
    Client_commands *newClientCommands = new Client_commands(this);
//   Connect the signal to the slot in Client_Window
//    connect(newClientCommands, &Client_commands::windowClosing, this, &Client_Window::handleClientCommandsClosing);
    connect(newClientCommands, &Client_commands::window_opend, this, &Client_Window::on_client_window_opend);


    newClientCommands->show();

    // set the box to the client
    QString id = item->text().split(" ")[1];
    foreach (const QSharedPointer<Client>& client, _manager.connected_clients) {
        if (id.toInt() == client->id) {
            client_windows_opend.insert(client, newClientCommands);

            // Connect the signal to the slot in Client_Window
            connect(newClientCommands, &Client_commands::send_to_client, this, &Client_Window::handleSendToClient);

            newClientCommands->start_client_box(client);
        }
    }
}

void Client_Window::on_client_window_opend(QSharedPointer<Client> client)
{
    qInfo()<<"OPEND " << client_windows_opend;
    ui->lstConsole->addItem("Client_commands window opened");
}
void Client_Window::handleClientCommandsClosing()
{
    // Do something when the Client_commands window is closing
    qDebug() << "Client_commands window is closing";

}

void Client_Window::handleSendToClient(QString message)
{
    // Handle the message received from the Client_commands window
    qInfo() << "Message received from Client_commands: " << message;

    // Now you can send this message to the connected device or perform any other actions as needed
    _controller.send(message);


}
