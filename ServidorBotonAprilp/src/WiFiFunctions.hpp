/*
 * Este archivo contiene las definiciones y  
 * parámetros de la red WiFi  
 * Modo de conexión : WiFi Direct 
 * Puerto de comunicación: socket 18220 //(puerto sugerido )
 * 
*/
#include <ESP8266WiFi.h>



#define PORT_TCP_SOCKET 18220 
#define SSID_WIFI_DIRECT_NAME "APRILP" //your name ssid favourite 
#define PASS_SSID_DIRECT_NAME "APRILPserver" //your pass favourite 


void createWiFiNet()
{
    const char *ssid = SSID_WIFI_DIRECT_NAME ;  
    const char *pass = PASS_SSID_DIRECT_NAME ; 
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass) ;
}


void connectClient(){
    
}


