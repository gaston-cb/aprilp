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
    const  int tam_buffer = sizeof(bool) + sizeof(unsigned int) + 2  ; 
    byte buffer_data [tam_buffer] ; 
    buffer_data[0] = 0xAB ; 
    buffer_data[tam_buffer-1] = 0xAB ; 
    int i = 0 ; 
    IPAddress ip ;   
    for (i = 0 ; i<MAX_CONECTION_AP-1;i++)
    {
        
        if(senddataClientSockets.Ipclients[i][0]!=127)
        {
            memcpy(&buffer_data[1], (byte *) &senddataClientSockets,sizeof(bool) + sizeof(unsigned int) ) ; 
            ip= IPAddress (senddataClientSockets.Ipclients[i][0],
                           senddataClientSockets.Ipclients[i][1], 
                           senddataClientSockets.Ipclients[i][2],
                           senddataClientSockets.Ipclients[i][3] ) ; 
            if (client.connect(ip ,PORT_TCP_SOCKET)){
                client.write(buffer_data,sizeof(buffer_data)) ;               
            }
            client.stop() ; 
        }
    }

}




