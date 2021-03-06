#include <Arduino.h>
#include <ESP8266WiFi.h>
#define PORT_TCP_SOCKET    18220 
#define TIME_ON_RECEIVE    20 // 20 SEGUNDOS 
#define FIRST_MODE_TOY_ON  1 
#define SECOND_MODE_TOY_ON 2 
#define MODE_TOY_OFF       0 
#define GPIO_ESP01_PORT    0 
#define SSID_WIFI_SERVER   "APRILP"  
#define SSID_PASS_WIFI     "APRILPserver"   

// registro de almacenamiento de modo y datos 
// mantiene los valores en memoria en base a los 
//datos recibidos por el servidor  
struct {
  bool isTimeON ; 
  unsigned int timeON  ; 
}register_on ; 

unsigned long int t0 = 0 ; 
unsigned long int tdelayOn = 0 ; 

void ConnectAP() ; 


WiFiServer button_server(PORT_TCP_SOCKET); 

bool isDatareceived = false ; 
uint8_t  isOnToy = 0 ; 

void setup() 
{
  ConnectAP()  ; 
  button_server.begin() ; //servidor TCP mediante sockets a la escucha 
  pinMode(GPIO_ESP01_PORT,OUTPUT) ; 
  digitalWrite(GPIO_ESP01_PORT,HIGH) ; 
  
}

void loop() {
 int size_buffer = sizeof(bool) + sizeof(unsigned int) +2 ; // 2 bytes end and init comunication 
 
 byte buffer_receive[size_buffer] ; 
 WiFiClient cliente = button_server.available() ;  
/*
 * codigo de recepción TCP 
*/ 
 if (WiFi.status()==WL_CONNECTED)
 {
  if(cliente)
  {
    cliente.readBytes(buffer_receive, size_buffer) ; 
    if (buffer_receive[0]==0xAB && buffer_receive[size_buffer-1]==0xAB)
    {
       // evita que al presionar varias veces en el servidor se repita, y se vuelva a encender 
       // permite una unica rutina 
      if (isOnToy == MODE_TOY_OFF){ 
        isDatareceived = true ; 
        memcpy(&register_on,&buffer_receive[1],size_buffer-2) ; 
      }
    }
  }
 }else{
   ConnectAP() ; // si se desconecta -- vuelve a conectarse 
 }  
 // END RX TCP 


 if (isDatareceived==true)
 {
   isDatareceived = false ; 
   if(register_on.isTimeON==false) {
    // enciende el juguete sin control 
    // consultar por timeout !  
    digitalWrite(GPIO_ESP01_PORT,LOW)  ; 
   }else if(register_on.isTimeON==true){
      digitalWrite(GPIO_ESP01_PORT,LOW)  ; 
      t0 = millis()  ; 
   }   
 }
 
 // tiempo para apagar el encendido 
 if (register_on.isTimeON==true){
   if (millis()-t0>=(register_on.timeON)*1000){
     register_on.isTimeON = false ; 
     digitalWrite(GPIO_ESP01_PORT,HIGH) ; 
   }
 }




}

void ConnectAP(){
  const int timeout = 50 ; 
  int count_timeout = 0 ; 
  const char *ssid =  SSID_WIFI_SERVER; 
  const char *psk  =  SSID_PASS_WIFI  ;  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);

  while (WiFi.status() != WL_CONNECTED && count_timeout<=timeout) 
  {
    count_timeout++ ; 
    delay(200);
  }

}