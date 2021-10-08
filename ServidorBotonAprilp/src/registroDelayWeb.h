
struct clientRegister
{
    bool isDelay ; 
    unsigned int timeDelay ; 
    uint8_t Ipclients[4][4] ;    // ip clients esp8266 --  
}; 

// garantizar portabilidad entre arquitecturas !  
typedef struct {
    unsigned int data: 24 ; 
}uint24_t ; 