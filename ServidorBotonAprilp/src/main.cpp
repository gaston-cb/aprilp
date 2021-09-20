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


void mainPage()  ; 
void configDelayClockClient() ; 
void valorDelay() ; 
void dataDelayWeb(uint8_t num, WStype_t type, uint8_t * payload, size_t length) ; 



ESP8266WebServer server(80);    // puerto HTTP 
WebSocketsServer webSocket = WebSocketsServer(81);  //PUERTO ESTANDAR WS 

clientRegister senddataClientSockets{false ,0U }; 




bool sendingDataClientSocket = false; 
bool isDelayOn = false ;      //retraso de señal 




void setup() {
  Serial.begin(115200) ; 
  createWiFiNet();
  server.on("/", mainPage);
  server.on("/delay.html",  configDelayClockClient);
  server.on("/del", HTTP_POST , valorDelay);
  webSocket.begin();
  webSocket.onEvent(dataDelayWeb);
  server.begin();
}

void loop() {
  webSocket.loop();
  server.handleClient();
  if (sendingDataClientSocket == true)
  {
    sendingDataClientSocket = false;  
    Serial.print("tiempo delay: ") ; Serial.println(senddataClientSockets.timeDelay) ; 
    Serial.print(" isdelay: ") ; Serial.println(senddataClientSockets.isDelay) ; 
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
    
