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

void mainPage()  ; 
void configDelayClockClient() ; 



ESP8266WebServer server(80); 



bool isDelayOn = false ;      //retraso de señal 
unsigned int delayTime = 0 ;  //tiempo de retraso de señal -- activación mediante página web 




void setup() {
  Serial.begin(115200) ; 
  createWiFiNet();
  server.on("/", mainPage);
  server.on("/delay.html",  configDelayClockClient);
  
  Serial.println("wificreated") ; 
  server.begin();
}

void loop() {
  //activate delay using webPage -- default none 
  server.handleClient();

}


void mainPage() {
  String s = MAIN_PAGE; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
  Serial.println("volver_atras") ; 
}

void configDelayClockClient(){
  String s = delayHMTL; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
  Serial.println("delay") ; 

}