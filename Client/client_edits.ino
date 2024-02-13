// main.cpp

#include <Arduino.h>
#include "Template.h"

// wifi & server settings
String board_name = "ESP32-GEAM";
String commands_auto = "automat = NONE" ;
String commands_manual = "manual = OPEN,CLOSE" ;
String status = "status = manual-CLOSE" ;

// Input (butoane + senzor)
const int close_btn = 14;  
const int open_btn = 15;  
const int automat = 27;  
 
const int online = 26; 
const int START = 25; 
const int senzor_p = 4; 

// variablilele butoanelor
int close_btn_state = 0;  
int open_btn_state = 0;
int automat_state = 0;  
int START_state = 0;
int online_state = 0;  
int senzor_p_state = 0;

// outputuri
const int out_inchis_geam = 18;
const int out_deschis_geam = 23;

bool mod_automat = false;
bool geam_inchis = true;
bool sistem_pornit = false;

String server_data;
int state = 0;
String server_command;


void setup() {
  // inputs
  pinMode(close_btn, INPUT);
  pinMode(open_btn, INPUT);
  pinMode(automat, INPUT);
  pinMode(START, INPUT);
  pinMode(online, INPUT);
  pinMode(senzor_p, INPUT);
  Serial.begin(9600);

  //outputs
  pinMode(out_inchis_geam, OUTPUT);
  pinMode(out_deschis_geam, OUTPUT);
  digitalWrite(out_inchis_geam, HIGH);
  digitalWrite(out_deschis_geam, HIGH);
}

void loop() {
  // read the state of the pushbutton value:
  close_btn_state = digitalRead(close_btn);
  open_btn_state = digitalRead(open_btn);
  automat_state = digitalRead(automat);
  START_state = digitalRead(START);
  online_state = digitalRead(online);
  senzor_p_state = digitalRead(senzor_p);
  if(server_connected){
    server_data = Read_from_server();
    if(server_data != "No data readed from Server"){
      if(server_data.indexOf("ESP32-GEAM:OPEN") != -1){
        Serial.println("DESCHIDE GEAM : ONLINE");
        server_command = "deschide";
      }else if(server_data.indexOf("ESP32-GEAM:CLOSE") != -1){
        Serial.println("INCHIDE GEAM : ONLINE");
        server_command = "inchide";
      }else if(server_data.indexOf("ESP32-GEAM:automat") != -1){
        Serial.println("AUTOMAT GEAM : ONLINE");
        server_command = "automat";
      }else if(server_data.indexOf("ESP32-GEAM:status") != -1){
        Serial.println("STATUS GEAM : ONLINE");
        while(!send_data_v(status)){
          Serial.println("RETRYING TO SEND STATUS");
        }
      }else if(server_data.indexOf("ESP32-GEAM:stop") != -1){
        Serial.println("STOP GEAM : ONLINE");
        sistem_pornit = false;
         digitalWrite(out_inchis_geam, LOW);
        digitalWrite(out_deschis_geam, HIGH);
        geam_inchis = true;
      }
      else if(server_data.indexOf("ESP32-GEAM:start") != -1){
        Serial.println("START GEAM : ONLINE");
        sistem_pornit = true;
      }
      else if(server_data.indexOf("Devices-Connected") != -1){
        Serial.println("ADMIN RECONNECT : ONLINE");
        while(!Board_init(board_name,commands_auto,commands_manual,status)){
          Serial.println("TRY TO RECONNECT TO ADMIN");
        }

      }
      
    }
  }

   // start the sistem
  if(START_state == HIGH){
    sistem_pornit = !sistem_pornit;
    if(sistem_pornit){
      Serial.println("SISTEM PORNIT");
      delay(1000);
    }else{
      Serial.println("SISTEM OPRIT");
      digitalWrite(out_inchis_geam, LOW);
      digitalWrite(out_deschis_geam, HIGH);
      geam_inchis = true;
      delay(1000);
    }
     
  }
  

  if(sistem_pornit){
    if(state == 0){
      if(Board_init(board_name, commands_auto, commands_manual, status)){
        state = 1;
      }
      else{ state = 2; }
    }
    // ONLINE
    if(state == 2 && online_state == HIGH){
      Serial.println("GOING ONLINE");
      state = 0;
    }

    // inchidere geam manual
    if ((close_btn_state == HIGH && !geam_inchis) || (server_command == "inchide" && !geam_inchis)) {
      Serial.println("INCHIDE MANUAL");
      mod_automat = false;
      geam_inchis = true;
      digitalWrite(out_inchis_geam, LOW);
      digitalWrite(out_deschis_geam, HIGH);
      if(state == 1 && status != "status = manual-CLOSE"){
        status = "status = manual-CLOSE";
        server_command = "";
      }
    } 
    // deschidere geam manual
    else if((open_btn_state == HIGH && geam_inchis) || (server_command =="deschide" && geam_inchis)) {
      Serial.println("DESCHIDE MANUAL");
      mod_automat = false;
      geam_inchis = false;
      digitalWrite(out_inchis_geam, HIGH);
      digitalWrite(out_deschis_geam, LOW);
      if(state == 1 && status != "status = manual-OPEN"){
        status = "status = manual-OPEN";
        server_command = "";
      }
    }
    // mod automat init + inchidere geam
    if(automat_state == HIGH || server_command == "automat" ){
      Serial.println("MOD AUTOMAT");
      if(!geam_inchis){
        digitalWrite(out_inchis_geam, LOW);
        digitalWrite(out_deschis_geam, HIGH);
        geam_inchis = true;
        delay(16000);
      }
      if(status != "status = automat-NONE"){
        status = "status = automat-NONE";
      }
      if(state == 1 ){
          server_command = "";
        }
      delay(1000);
      mod_automat = true;
    }
    // mod automat
    if(mod_automat){
      // Serial.println("MOD AUTOMAT INTRAT");
      // daca nu ploua si geamu e inchis
      if(senzor_p_state == HIGH && geam_inchis){
        Serial.println("DESCHIDE AUTOMAT");
        digitalWrite(out_inchis_geam, HIGH);
        digitalWrite(out_deschis_geam, LOW);
        geam_inchis = false;
      }
      // daca ploua si geamu e deschis
      if(senzor_p_state == LOW && !geam_inchis){
        Serial.println("INCHIDE AUTOMAT");
        digitalWrite(out_inchis_geam, LOW);
        digitalWrite(out_deschis_geam, HIGH);
        geam_inchis = true;
      }
    }
  }
}
