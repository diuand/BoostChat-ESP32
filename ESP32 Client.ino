#include <WiFi.h>

const char* ssid = "-------"; //wifi name
const char* password = "--------"; //wifi pass
const char* serverIP = "-------------";  //server ip
const int serverPort = 55004; 

WiFiClient client;

String board_name = "ESP32";


// Sending a string to the server
void Send_to_server(String data){
   if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");
    client.print(data);
    Serial.println("===============================");
    Serial.print("Send ");
    Serial.println(data);
  } else {
    Serial.println("Connection to server failed");
  }
}

// reading a string from server
String Read_from_server()
{
  if (client.available())
  {
    String data = client.readStringUntil('\n');
    data.trim();
    return data;
  }
  return "";
}


void setup() {
  Serial.begin(9600);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}




bool Verify_boardname(String board_name){
  Send_to_server(board_name);
  while(!client.available()){
    Serial.println("Waiting... ");
  }
  String server_data = Read_from_server();
  if(server_data == board_name){
    Serial.println("Board_name OK");
    Serial.println(server_data);
    return true;
  }
  Serial.println("Board_name ERR");
  Serial.println(server_data);
  return false;
}

int state = 0;
void state_machine(){
  switch (state) {
    // initial verification state
    case 0:
      if(Verify_boardname(board_name))
        state = 1;
      else 
        state = 100;
      break;

    case 1:
      if (!client.connected())
        state = 100;
      else
        state = 100;
      break;

    // Disconnecting state
    case 100:
      Serial.println(F("Disconnecting from server..."));
      client.stop();
      state = -1;
      break;

    default:
      break;
  }
}



void loop() {
  state_machine();
}
