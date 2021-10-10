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
  bool isDelay ; 
  unsigned int delayTime ; 
}register_delay ; 

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
        memcpy(&register_delay,&buffer_receive[1],size_buffer-2) ; 
      }
    }
  }
 }else{
   ConnectAP() ; // si se desconecta -- vuelve a conectarse 
 }  
 
 // seleccion de tiempo en base a datos recibidos por el servidor 
 // se realiza aparte, ya que recibe cuelgues del microprocesador
 // si se realiza dentro de la rutina de recepcion, se corren riesgos 
 // de colgado del microcontrolador  
 if (isDatareceived==true)
 {
   isDatareceived = false ; 
   if(register_delay.isDelay==false) {
     isOnToy = FIRST_MODE_TOY_ON ; 
     t0 = millis() ; 
     digitalWrite(GPIO_ESP01_PORT,LOW) ; 
   }else if(register_delay.isDelay==true){
    isOnToy = SECOND_MODE_TOY_ON  ; 
    tdelayOn = millis() ; 
   }   
 }
 

 // si el modo es FIRST_MODE_TOY_ON, entonces queda encendido durante 20 segundos 
 // si el modo es SECOND_MODE_TOY_ON, entonces, espera un tiempo preseteado, y luego 
 // cambia a FIRST_MODE_TOY, y enciende el juguete durante 20 segundos  
 if (millis()-t0>20000 && isOnToy == FIRST_MODE_TOY_ON  ) 
 {
  digitalWrite(GPIO_ESP01_PORT,HIGH) ; 
  isOnToy = MODE_TOY_OFF  ; 
 }else if (millis()-tdelayOn>= register_delay.delayTime*1000   &&     isOnToy ==  SECOND_MODE_TOY_ON  ){
    //register_delay.delayTime*1000 es tiempo en milisegundos -- se comparan milisegundos 
    isOnToy = FIRST_MODE_TOY_ON ; 
    t0 = millis() ; 
    digitalWrite(GPIO_ESP01_PORT,LOW) ; 
 }    

}

void ConnectAP(){
  const int timeout = 50 ; 
  int count_timeout = 0 ; 
  const char *ssid =  SSID_WIFI_SERVER; 
  const char *psk  =  SSID_PASS_WIFI ;  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);

  while (WiFi.status() != WL_CONNECTED && count_timeout<=timeout) 
  {
    count_timeout++ ; 
    delay(200);
  }


}