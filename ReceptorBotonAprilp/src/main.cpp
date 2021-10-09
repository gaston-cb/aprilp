#include <Arduino.h>
#include <ESP8266WiFi.h>
#define PORT_TCP_SOCKET 18220 
#define TIME_ON_RECEIVE 20 // 20 SEGUNDOS 

struct {
  bool isDelay ; 
  unsigned int delayTime ; 
}register_delay ; 


void ConnectAP() ; 
WiFiServer button_server(PORT_TCP_SOCKET); 

void setup() {
  Serial.begin(9600) ; 
  ConnectAP()  ; 
  button_server.begin() ; 
}

void loop() {
 int size_buffer = sizeof(bool) + sizeof(unsigned int) +2 ; // 2 bytes end and init comunication 
 byte buffer_receive[size_buffer] ; 
 WiFiClient cliente = button_server.available() ;  
 if (WiFi.status()==WL_CONNECTED)
 {
  if(cliente)
  {
    cliente.readBytes(buffer_receive, size_buffer) ; 
    if (buffer_receive[0]==0xAB && buffer_receive[size_buffer-1]==0xAB)
    {
      memcpy(&register_delay,&buffer_receive[1],size_buffer-2) ; 
    }
  }

 }else{
   ConnectAP() ; 
 }  



}

void ConnectAP(){
  const int timeout = 50 ; 
  int count_timeout = 0 ; 
  const char *ssid = "APRILP" ; 
  const char *psk  = "APRILPserver" ;  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 
  while (WiFi.status() != WL_CONNECTED && count_timeout<=timeout) 
  {
    count_timeout++ ; 
    delay(200);
    Serial.print('.');
  }


}