
//mac: bc:dd:c2 || -- b2:96:92 
struct clientRegister
{
    bool isDelay ; 
    unsigned int timeDelay ; 
    unsigned char numEspConnected ; 
    /* format: IPadres - Mac adress */ 
    uint8_t MAC_Ipclients[4][4] ;    //xx.xx.xx.xx --- ip clients esp8266 --  
}; 