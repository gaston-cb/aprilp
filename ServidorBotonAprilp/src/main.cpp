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
#include "macaddress.h"
extern "C" {
  #include "user_interface.h"
}
#define NUMBERS_MACS_ESPRESSIF 96 
#define IP_LOCALHOST {127, 0 , 0 ,1}  

void mainPage()  ; 
void configDelayClockClient() ; 
void valorDelay() ; 
void dataDelayWeb(uint8_t num, WStype_t type, uint8_t * payload, size_t length) ; 
void newConnectClient(WiFiEventSoftAPModeStationConnected sta_info) ; 
void obtainIPClients() ; 
int isMacEspressif(uint24_t macsAdress)   ; 

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
const uint24_t macsAddressEspressif[NUMBERS_MACS_ESPRESSIF]{
MAC_ADDRESS_ESPRESSIF_0, 
MAC_ADDRESS_ESPRESSIF_1, 
MAC_ADDRESS_ESPRESSIF_2,
MAC_ADDRESS_ESPRESSIF_3,
MAC_ADDRESS_ESPRESSIF_4, 
MAC_ADDRESS_ESPRESSIF_5,
MAC_ADDRESS_ESPRESSIF_6,
MAC_ADDRESS_ESPRESSIF_7,
MAC_ADDRESS_ESPRESSIF_8,
MAC_ADDRESS_ESPRESSIF_9 ,
MAC_ADDRESS_ESPRESSIF_10 ,
MAC_ADDRESS_ESPRESSIF_11 ,
MAC_ADDRESS_ESPRESSIF_12 ,
MAC_ADDRESS_ESPRESSIF_13 ,
MAC_ADDRESS_ESPRESSIF_14 ,
MAC_ADDRESS_ESPRESSIF_15 ,
MAC_ADDRESS_ESPRESSIF_16 ,
MAC_ADDRESS_ESPRESSIF_17 ,
MAC_ADDRESS_ESPRESSIF_18 ,
MAC_ADDRESS_ESPRESSIF_19 ,
MAC_ADDRESS_ESPRESSIF_20 ,
MAC_ADDRESS_ESPRESSIF_21 ,
MAC_ADDRESS_ESPRESSIF_22 ,
MAC_ADDRESS_ESPRESSIF_23 ,
MAC_ADDRESS_ESPRESSIF_24 ,
MAC_ADDRESS_ESPRESSIF_25 ,
MAC_ADDRESS_ESPRESSIF_26 ,
MAC_ADDRESS_ESPRESSIF_27 ,
MAC_ADDRESS_ESPRESSIF_28 ,
MAC_ADDRESS_ESPRESSIF_29 ,
MAC_ADDRESS_ESPRESSIF_30 ,
MAC_ADDRESS_ESPRESSIF_31 ,
MAC_ADDRESS_ESPRESSIF_32 ,
MAC_ADDRESS_ESPRESSIF_33 ,
MAC_ADDRESS_ESPRESSIF_34 ,
MAC_ADDRESS_ESPRESSIF_35 ,
MAC_ADDRESS_ESPRESSIF_36 ,
MAC_ADDRESS_ESPRESSIF_37 ,
MAC_ADDRESS_ESPRESSIF_38 ,
MAC_ADDRESS_ESPRESSIF_39 ,
MAC_ADDRESS_ESPRESSIF_40 ,
MAC_ADDRESS_ESPRESSIF_41 ,
MAC_ADDRESS_ESPRESSIF_42 ,
MAC_ADDRESS_ESPRESSIF_43 ,
MAC_ADDRESS_ESPRESSIF_44 ,
MAC_ADDRESS_ESPRESSIF_45 ,
MAC_ADDRESS_ESPRESSIF_46 ,
MAC_ADDRESS_ESPRESSIF_47 ,
 MAC_ADDRESS_ESPRESSIF_48 ,
 MAC_ADDRESS_ESPRESSIF_49 ,
 MAC_ADDRESS_ESPRESSIF_50 ,
 MAC_ADDRESS_ESPRESSIF_51 ,
 MAC_ADDRESS_ESPRESSIF_52 ,
 MAC_ADDRESS_ESPRESSIF_53 ,
 MAC_ADDRESS_ESPRESSIF_54 ,
 MAC_ADDRESS_ESPRESSIF_55 ,
 MAC_ADDRESS_ESPRESSIF_56 ,
 MAC_ADDRESS_ESPRESSIF_57 ,
 MAC_ADDRESS_ESPRESSIF_58 ,
 MAC_ADDRESS_ESPRESSIF_59 ,
 MAC_ADDRESS_ESPRESSIF_60 ,
 MAC_ADDRESS_ESPRESSIF_61 ,
 MAC_ADDRESS_ESPRESSIF_62 ,
 MAC_ADDRESS_ESPRESSIF_63 ,
 MAC_ADDRESS_ESPRESSIF_64 ,
 MAC_ADDRESS_ESPRESSIF_65 ,
 MAC_ADDRESS_ESPRESSIF_66 ,
 MAC_ADDRESS_ESPRESSIF_67 ,
 MAC_ADDRESS_ESPRESSIF_68 ,
 MAC_ADDRESS_ESPRESSIF_69 ,
 MAC_ADDRESS_ESPRESSIF_70 ,
 MAC_ADDRESS_ESPRESSIF_71 ,
 MAC_ADDRESS_ESPRESSIF_72 ,
 MAC_ADDRESS_ESPRESSIF_73 ,
 MAC_ADDRESS_ESPRESSIF_74 ,
 MAC_ADDRESS_ESPRESSIF_75 ,
 MAC_ADDRESS_ESPRESSIF_76 ,
 MAC_ADDRESS_ESPRESSIF_77 ,
 MAC_ADDRESS_ESPRESSIF_78 ,
 MAC_ADDRESS_ESPRESSIF_79 ,
 MAC_ADDRESS_ESPRESSIF_80 ,
 MAC_ADDRESS_ESPRESSIF_81 ,
 MAC_ADDRESS_ESPRESSIF_82 ,
 MAC_ADDRESS_ESPRESSIF_83 ,
 MAC_ADDRESS_ESPRESSIF_84 ,
 MAC_ADDRESS_ESPRESSIF_85 ,
 MAC_ADDRESS_ESPRESSIF_86 ,
 MAC_ADDRESS_ESPRESSIF_87 ,
 MAC_ADDRESS_ESPRESSIF_88 ,
 MAC_ADDRESS_ESPRESSIF_89 ,
 MAC_ADDRESS_ESPRESSIF_90 ,
 MAC_ADDRESS_ESPRESSIF_91 ,
 MAC_ADDRESS_ESPRESSIF_92 ,
 MAC_ADDRESS_ESPRESSIF_93 ,
 MAC_ADDRESS_ESPRESSIF_94 ,
 MAC_ADDRESS_ESPRESSIF_95 
} ; 


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
  // test busqueda binaria 
  uint24_t testing = { 0x78E625} ; 
  int i = isMacEspressif(testing) ; 
  Serial.println("resultingsearch: ") ; 
  Serial.println(i) ; 

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
  IPAddress ipclient ; 
  uint24_t macaddress_clients ; 
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
    macaddress_clients.data =  (station_list->bssid[0])<<16 ;  
    macaddress_clients.data = (macaddress_clients.data) | (station_list->bssid[1])<<8  ; 
    macaddress_clients.data = (macaddress_clients.data) | (station_list->bssid[2])  ; 
    Serial.print("macAdress_str ") ; Serial.println(macaddress_clients.data,HEX) ; 
    ipclient = IPAddress((&station_list->ip)->addr) ; //ip 
    Serial.print(ipclient[0])   ; Serial.print('.') ; 
    Serial.print(ipclient[1])   ; Serial.print('.') ; 
    Serial.print(ipclient[2])   ; Serial.print('.') ; 
    Serial.println(ipclient[3]) ; 
   
    if (isMacEspressif(macaddress_clients)==1) // exist espressif dispositive connected 
    {
        //llenar registros con Ips 
    }
      station_list = STAILQ_NEXT(station_list, next);
  }
  wifi_softap_free_station_info();
}


/*
 * Busqueda dicotómica 
 * return -1 : no encontrado 
 * return  1 : encontrado 
*/

int isMacEspressif(uint24_t mac_client ){

  int inf = 0 ; 
  int sup = NUMBERS_MACS_ESPRESSIF-1  ; //array 0 to NUMBER_MACS_ESPRESSIF-1 
  int center  ; 
  if (mac_client.data<MAC_ADDRESS_ESPRESSIF_0 || mac_client.data>MAC_ADDRESS_ESPRESSIF_95){
    return -1 ; 
  } 

  while(inf<=sup)
  {
    center = (inf+sup)/2 ; 
    if(macsAddressEspressif[center].data == mac_client.data)
    {

      return 1 ; 
    }else if(macsAddressEspressif[center].data<mac_client.data)
    {
      inf = center + 1 ; 
    }else {
      sup = center - 1 ; 
    }
  }
  return -1 ; // is mac espressif -- true 
}

