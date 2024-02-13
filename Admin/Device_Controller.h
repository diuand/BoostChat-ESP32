#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include <QObject>
#include <QTcpSocket>
class Device_Controller : public QObject
{
    Q_OBJECT
public:
    explicit Device_Controller(QObject *parent = nullptr);
    void Connect_to_device(QString ip,int port);
    QAbstractSocket::SocketState state();
    void Disonnect_from_device();
    bool is_connected();
    void send(QString message);

signals:
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState);
    void errorOccurred(QAbstractSocket::SocketError);
    void dataReady(QByteArray);

private slots:
    void socket_stateChanged(QAbstractSocket::SocketState state);
    void socket_readyRead();
private:
    QTcpSocket _socket;
    QString _ip;
    int _port;
};

#endif // DEVICE_CONTROLLER_H
