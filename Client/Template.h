#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <WiFi.h>

extern int state;
extern const char* ssid;
extern const char* password;
extern const char* serverIP;
extern const int serverPort;
extern WiFiClient client;
extern String board_name;
extern bool server_connected;
extern String commands;

void Conn_to_wifi();
void Conn_to_server();
void Send_to_server(String data);
String Read_from_server();
bool Server_test_ok(String board_name);
bool Board_init(String board_name,String commands_auto,String commands_manual,String status);
bool send_data_v(String data);
#endif
