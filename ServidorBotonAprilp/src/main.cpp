/*
 *  Servidor para manejo remoto de juguetes 
 *  Autor: Gaston Valdez 
 *  Version 1.0 
 *  Este software se utiliza en conjunto con el cliente 
 *  que debe ponerse en el juguete/sistema a controlar 
 * 
*/

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "webPages.h"
#include "WiFiFunctions.hpp"
#include <WebSocketsServer.h>
#include "registroDelayWeb.h" 
extern "C" {
  #include "user_interface.h"
}

#define ADDRESS_MAC_ESP_0 0xBCu
#define ADDRESS_MAC_ESP_1 0xDDu
#define ADDRESS_MAC_ESP_2 0xC2u  
#define IP_LOCALHOST {127, 0 , 0 ,1}  

void mainPage()  ; 
void configDelayClockClient() ; 
void valorDelay() ; 
void dataDelayWeb(uint8_t num, WStype_t type, uint8_t * payload, size_t length) ; 
void newConnectClient(WiFiEventSoftAPModeStationConnected sta_info) ; 
void obtainIPClients() ; 


ESP8266WebServer server(80);    // puerto HTTP 
WebSocketsServer webSocket = WebSocketsServer(81);  //PUERTO ESTANDAR WS 
clientRegister senddataClientSockets{
  false , // isDelay 
  0U ,    // timeDelay 
  0, 
  {
    IP_LOCALHOST,  
    IP_LOCALHOST, 
    IP_LOCALHOST,
    IP_LOCALHOST
  } 
}; 



char last_mac[18] ;  
bool sendingDataClientSocket = false; 
bool isDelayOn = false ;      //retraso de señal 
bool waitDHCP  = false ; 



void setup() {
  static WiFiEventHandler e1; //dispatcher 
  Serial.begin(115200) ; 
  createWiFiNet();
  server.on("/", mainPage);
  server.on("/delay.html",  configDelayClockClient);
  server.on("/del", HTTP_POST , valorDelay);
  e1 = WiFi.onSoftAPModeStationConnected(newConnectClient);
  webSocket.begin();
  webSocket.onEvent(dataDelayWeb);
  server.begin();
}

unsigned long t0 = 0 ; 

void loop() {
  webSocket.loop();
  server.handleClient();
  if (sendingDataClientSocket == true)
  {
    sendingDataClientSocket = false;  
    // ¿hay clientes conectados? 
    Serial.print("tiempo delay: ") ; Serial.println(senddataClientSockets.timeDelay) ; 
    Serial.print(" isdelay: ") ; Serial.println(senddataClientSockets.isDelay) ; 
    //send data to clients 

  }
  if (millis()-t0>30000){
    t0 = millis() ; 
    obtainIPClients() ; 
  }
}

void mainPage() 
{
  String s = MAIN_PAGE; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void configDelayClockClient()
{
  String s = delayHMTL;             //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void valorDelay()
{
  String recibido = server.arg("plain") ; 
  if (recibido=="true")
  {
    senddataClientSockets.isDelay = true ; 
    isDelayOn = true ;
  }else if(recibido=="false")
  {
    isDelayOn = false ;
    senddataClientSockets.isDelay = false ; 
  }
  server.send(200,"text/html",recibido) ; 

}


void dataDelayWeb(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  //remote
  switch(type)
  {
    case WStype_DISCONNECTED:    
      break;
    case WStype_CONNECTED: 
      webSocket.sendTXT(num, "Connected");        
      break;
    case WStype_TEXT:
      if (length==3){
        senddataClientSockets.timeDelay = int((char) payload[0]-0x30) *100  +  int((char) payload[1]-0x30) *10+ int((char) payload[2]-0x30) ; 
        sendingDataClientSocket = true ; 
      }else if(length==2){
       senddataClientSockets.timeDelay = int((char) payload[0]-0x30) *10  + int((char) payload[0]-0x30) *10  ;    
       sendingDataClientSocket = true ; 
      }
      break;
    default:
      break ; 
  } 
}
    

void newConnectClient(WiFiEventSoftAPModeStationConnected sta_info){

  sprintf(last_mac,"%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(sta_info.mac));
 /* Serial.print(sta_info.mac[0],HEX) ; Serial.println(':') ;  
  Serial.print(sta_info.mac[1],HEX) ; Serial.println(':') ; 
  Serial.print(sta_info.mac[2],HEX) ; Serial.println(':') ; 
  Serial.print(sta_info.mac[3],HEX) ; Serial.println(':') ; 
  Serial.print(sta_info.mac[4],HEX) ;Serial.println(':') ; 
  Serial.println(sta_info.mac[5],HEX) ;  */ 
  waitDHCP = true ;  
  
  
} 


void obtainIPClients(){
  Serial.println("IPS_clientesConectados") ; 
  IPAddress ipclient ; 
  struct station_info *station_list = wifi_softap_get_station_info();
  if (station_list==NULL){
    // no hay clientes conectados ! 
    return ; 
  }
  while (station_list != NULL) 
  {
    // logica de comparación de clientes para enviar datos ! 
    Serial.print(station_list->bssid[0],HEX) ; Serial.print(':') ; //mac
    Serial.print(station_list->bssid[1],HEX) ; Serial.print(':') ; 
    Serial.print(station_list->bssid[2],HEX) ; Serial.print(':') ; 
    Serial.print(station_list->bssid[3],HEX) ; Serial.print(':') ; 
    Serial.print(station_list->bssid[4],HEX) ; Serial.print(':') ; 
    Serial.println(station_list->bssid[5],HEX) ;  
    ipclient = IPAddress((&station_list->ip)->addr) ; //ip 
    Serial.print(ipclient[0])   ; Serial.print('.') ; 
    Serial.print(ipclient[1])   ; Serial.print('.') ; 
    Serial.print(ipclient[2])   ; Serial.print('.') ; 
    Serial.println(ipclient[3]) ; 
   
    if (station_list->bssid[0] == ADDRESS_MAC_ESP_0 && 
        station_list->bssid[1] == ADDRESS_MAC_ESP_1 && 
        station_list->bssid[2] == ADDRESS_MAC_ESP_2)
      {
        Serial.println("esp_connected! ") ; 
        // verificar IPS -- GUARDAR IPS     
      }
      station_list = STAILQ_NEXT(station_list, next);
  }
  wifi_softap_free_station_info();
}