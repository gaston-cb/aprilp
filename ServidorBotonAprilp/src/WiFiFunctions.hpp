/*
 * Este archivo contiene las definiciones y  
 * parámetros de la red WiFi  
 * Modo de conexión : WiFi Direct 
 * Puerto de comunicación: socket 18220 //(puerto sugerido )
 * 
*/
#include <ESP8266WiFi.h>
#ifndef REGISTERS_DELAY
  #define REGISTERS_DELAY
  #include "registroDelayWeb.h" 
#endif



#define PORT_TCP_SOCKET 18220 
#define SSID_WIFI_DIRECT_NAME "APRILP" //your name ssid favourite 
#define PASS_SSID_DIRECT_NAME "APRILPserver" //your pass favourite 
#define MAX_CONECTION_AP 5
#define HIDDEN_WIFI 0 // 0 - NO HIDDEN AP , 1 HIDDEN AP 

WiFiServer wifiServer(PORT_TCP_SOCKET); 
WiFiClient client; 

 
extern struct clientRegister senddataClientSockets; 







void createWiFiNet()
{
    const char *ssid = SSID_WIFI_DIRECT_NAME ;  
    const char *pass = PASS_SSID_DIRECT_NAME ; 
    WiFi.mode(WIFI_AP);
    //WiFi.softAP(ssid, pass) ;
    WiFi.softAP(ssid,pass,1,HIDDEN_WIFI, MAX_CONECTION_AP);
}


void sendDataClient()
{   
    int i = 0 ; 
    int index_ip_address = 0 ;  
    for (i = 0 ; i<MAX_CONECTION_AP-1;i++){
        if(senddataClientSockets.Ipclients[i][0]!=127)
        {
            Serial.print(senddataClientSockets.Ipclients[i][0]) ; Serial.print(".") ;  
            Serial.print(senddataClientSockets.Ipclients[i][1]) ; Serial.print(".") ; 
            Serial.print(senddataClientSockets.Ipclients[i][2]) ; Serial.print(".") ; 
            Serial.println(senddataClientSockets.Ipclients[i][3]) ; 
        }

    }

    
        
    /*
    if (senddataClientSockets.Ipclients[0] != 127 ){

    }
    if(client.connect(,PORT_TCP_SOCKET)){
        //send data 

    }
    client.stop() ; */ 


}




