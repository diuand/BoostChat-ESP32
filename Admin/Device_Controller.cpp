#include "Device_Controller.h"

Device_Controller::Device_Controller(QObject *parent)
    : QObject{parent}
{
    connect(&_socket,&QTcpSocket::connected,this,&Device_Controller::connected);
    connect(&_socket,&QTcpSocket::disconnected,this,&Device_Controller::disconnected);
    connect(&_socket,&QTcpSocket::stateChanged,this,&Device_Controller::socket_stateChanged);
    connect(&_socket,&QTcpSocket::errorOccurred,this,&Device_Controller::errorOccurred);
    connect(&_socket,&QTcpSocket::readyRead,this,&Device_Controller::socket_readyRead);
}

void Device_Controller::Connect_to_device(QString ip, int port)
{
    if(_socket.isOpen()){
        if(ip == _ip && port == _port){
            return;
        }
        _socket.close();
    }
    _ip = ip;
    _port = port;
    _socket.connectToHost(ip,port);
}

QAbstractSocket::SocketState Device_Controller::state()
{
    return _socket.state();
}

void Device_Controller::Disonnect_from_device()
{
    _socket.close();
}

bool Device_Controller::is_connected()
{
    return _socket.state() == QAbstractSocket::ConnectedState;
}

void Device_Controller::send(QString message)
{

    _socket.write(message.toUtf8());
}

void Device_Controller::socket_stateChanged(QAbstractSocket::SocketState state)
{
    if(state  == QAbstractSocket::UnconnectedState){
        _socket.close();
    }
    emit stateChanged(state);
}

void Device_Controller::socket_readyRead()
{
    auto data = _socket.readAll();
    emit dataReady(data);
}

