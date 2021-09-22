#include <Arduino.h>
#include <ESP8266WiFi.h>
void ConnectAP() ; 

void setup() {
  Serial.begin(9600) ; 
  ConnectAP()  ; 

}

void loop() {
  // put your main code here, to run repeatedly:
}

void ConnectAP(){
  const char *ssid = "APRILP" ; 
  const char *psk  = "APRILPserver" ;  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);
   Serial.print('.');
  }


}