
//mac: bc:dd:c2 || -- b2:96:92 
struct clientRegister
{
    bool isDelay ; 
    unsigned int timeDelay ; 
    uint8_t numberClients;     
    /* format: IPadres - Mac adress */ 
    uint8_t MAC_Ipclients[4][10] ; //xx. xx . xx . xx + mm:mm:mm:mm:mm:mm  
}; 