#include "clients_manager.h"

Clients_manager::Clients_manager(QObject *parent)
    : QObject{parent}
{

}

void Clients_manager::add_client(int id)
{
    QSharedPointer<Client> client_obj = QSharedPointer<Client>::create(id);
    if(connected_clients.size() != 0 ){
        foreach (const QSharedPointer<Client>& client, connected_clients) {
            if(client->id == id){
                qInfo()<< client_obj->id<<" Already exists";
                return;
            }
        }
    }
    connected_clients.append(client_obj);
    qInfo()<< client_obj->id<<"Connected";
}

void Clients_manager::remove_client(int _id)
{

    for (int i = 0; i < connected_clients.size(); ++i) {
       QSharedPointer<Client> client_obj = connected_clients.at(i);
        if (client_obj->id == _id) {
           // Remove the client from the list
           connected_clients.removeAt(i);
           qInfo() << "Client "<<_id << " Disconnected";
           return;
       }
    }

}

void Clients_manager::display_cliets()
{
    if(connected_clients.size() == 0){
       return;}
     foreach (const QSharedPointer<Client>& client, connected_clients) {
//       qInfo()<<client << " <-----> "<< client->id;
       emit c_DataReady(client);
     }
}

void Clients_manager::set_client_name(QList<QByteArray> txt_parts)
{
     foreach (const QSharedPointer<Client>& client, connected_clients) {
       if(client->id == txt_parts[1].toInt()){
           client->name = txt_parts[3];
           qInfo()<<"CLient " << client->id << " name updated: "<<client->name;
       }
     }
}

void Clients_manager::set_client_status(QList<QByteArray> txt_parts)
{
     foreach (const QSharedPointer<Client>& client, connected_clients) {
       if(client->id == txt_parts[1].toInt()){
           client->status = txt_parts[4];
           qInfo()<<"CLient " << client->id << " status updated: "<<client->status;
       }
     }
}

void Clients_manager::set_client_auto(QList<QByteArray> txt_parts)
{
     foreach (const QSharedPointer<Client>& client, connected_clients) {
       if(client->id == txt_parts[1].toInt()){
           QList<QByteArray> commands = txt_parts[4].split(',');
           foreach (QByteArray command, commands) {
               client->automat <<QString(command);
                qInfo()<<"CLient " << client->id << " auto commands added: "<<QString(command);
           }
       }
     }
}

void Clients_manager::set_client_manual(QList<QByteArray> txt_parts)
{
     foreach (const QSharedPointer<Client>& client, connected_clients) {
       if(client->id == txt_parts[1].toInt()){
           QList<QByteArray> commands = txt_parts[4].split(',');
           foreach (QByteArray command, commands) {
                client->manual <<QString(command);
                qInfo()<<"CLient " << client->id << " manual commands added: "<<QString(command);
           }
       }
     }
}


bool Clients_manager::conn_txt_verify(QList<QByteArray> txt_parts)
{
     if(txt_parts.size() == 3){
         if( txt_parts[2].toLower() == "connected" && txt_parts[0].toLower() == "client"){
             return true;
         }
     }

     return false;
}
bool Clients_manager::disc_txt_verify(QList<QByteArray> txt_parts)
{
     // verify if a client is connected to the admin
     if(connected_clients.size() == 0){
         return false;
     }

     if(txt_parts.size() == 3){
         if(txt_parts[2].toLower()  == "disconnected" && txt_parts[0].toLower() == "client"){
             // verify if the id exists in the connected clients id
             int ct{0};
             foreach (const QSharedPointer<Client>& client, connected_clients) {
                if(client->id == txt_parts[1].toInt()){
                    ct++;
                }
             }
             if(ct == 1)  return true;
         }
     }
     return false;
}
bool Clients_manager::name_def_txt_verify(QList<QByteArray> txt_parts)
{
     // verify if a client is connected to the admin
     if(connected_clients.size() == 0){
         return false;
     }
     // verify the text formulation
     if(txt_parts.size() == 4){
         if(txt_parts[0].toLower() == "client" && txt_parts[2].toLower() == "name"  ){
             // verify if the id exists in the connected clients id
             int ct{0};
             foreach (const QSharedPointer<Client>& client, connected_clients) {
                if(client->id == txt_parts[1].toInt()){
                    ct++;
                }
             }
             if(ct == 1)  return true;
         }
     }

     return false;
}
bool Clients_manager::commands_verify_m(QList<QByteArray> txt_parts)
{
     // verify if a client is connected to the admin
     if(connected_clients.size() == 0){
         return false;
     }
     if(txt_parts.size() >= 4){
         // verify the text formulation
         if(txt_parts[0].toLower() == "client" && txt_parts[3].toLower() == "=" && txt_parts[2].toLower() == "manual"){
             // verify if the id exists in the connected clients id
             int ct{0};
             foreach (const QSharedPointer<Client>& client, connected_clients) {
                if(client->id == txt_parts[1].toInt()){
                    ct++;

                }
             }
             if(ct == 1)  return true;

         }
     }

     return false;



}
bool Clients_manager::commands_verify_a(QList<QByteArray> txt_parts)
{
     // verify if a client is connected to the admin
     if(connected_clients.size() == 0){
         return false;
     }
     if(txt_parts.size() >= 4){
         // verify the text formulation
         if(txt_parts[0].toLower() == "client" && txt_parts[3].toLower() == "=" && txt_parts[2].toLower() == "automat"){
             // verify if the id exists in the connected clients id
             int ct{0};
             foreach (const QSharedPointer<Client>& client, connected_clients) {
                if(client->id == txt_parts[1].toInt()){
                    ct++;

                }
             }
             if(ct == 1)  return true;
         }
     }

     return false;



}
bool Clients_manager::commands_verify_s(QList<QByteArray> txt_parts)
{
     // verify if a client is connected to the admin
     if(connected_clients.size() == 0){
         return false;
     }
     if(txt_parts.size() >= 4){
         // verify the text formulation
         if(txt_parts[0].toLower() == "client" && txt_parts[3].toLower() == "=" && txt_parts[2].toLower() == "status"){
             // verify if the id exists in the connected clients id
             int ct{0};
             foreach (const QSharedPointer<Client>& client, connected_clients) {
                if(client->id == txt_parts[1].toInt()){
                    ct++;

                }
             }
             if(ct == 1)  return true;

         }

     }

     return false;



}
void Clients_manager::add_client_history(QString data)
{

     // Split the QByteArray using space as the delimiter
     QList<QString> txt_parts = data.split(' ');
     // verify if a client is connected to the admin
     if(connected_clients.size() == 0){
         return;
     }
     // verify the text formulation
     if(txt_parts[0].toLower() == "client" && txt_parts[1].toLower().toInt() < 1000 ){
         // verify if the id exists in the connected clients id
         foreach (const QSharedPointer<Client>& client, connected_clients) {
             if(client->id == txt_parts[1].toInt()){
                 client->message_history.append(data);
             }
         }
     }


}
