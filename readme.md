# Encendido remoto  -  version 1.0 

El proyecto, controla el encendido remoto (a distancia) de un circuito, en base a un receptor. Este receptor, se conecta a una red WiFi creada por un pulsador. Este pulsador al ser presionado, le envía la orden de encendido a todos los clientes (receptores), el cual realiza la activación de todos juntos. El servidor, y cliente, es alimentado mediante dos pilas AA de 1.5 V. También, puede utilizarse para encender cualquier otro tipo de circuito, siempre y cuando, no supere la corriente máxima admitida por el optoacoplador. El sistema diseñado, responde al siguiente esquema: 

![diagrama general](images/diagramabloques.png)

La institución, que probará y realizará los test correspondientes a software y hardware, es APRILP (Asociacion pro rehabilitación infantil La Plata), y se usarán en juguetes, para chicos con discapacidad. Estos test, se documentarán para una proxima versión. Se aceptan sugerencias/modificación para realizar mejoras/upgrades del código. 

# Materiales 
   
   La siguiente lista de materiales, es la utilizada en este proyecto, y depende del circuito a encender. La elección del optoacoplador, depende de la corriente que maneje el circuito a encender. En este caso, se utiliza el PC817, por estar disponible comercialmente en Argentina.

1. ESP01 (al menos 2) 
2. PC817 (optoacoplador) uno por cada cliente que desee
3. Resistencias
    1. 1K (una por cada cliente) 
    2. 10K  (una por cada cliente) 
4. Portapilas 
5. Programador ESP01 o simil 
6. Pilas AA (opcional,puede utilizarse cualquier otra fuente de  alimentación. En este proyecto, se utiliza este tipo de pilas). Se usan dos por dispositivo

Este proyecto, puede realizarse con cualquier microcontrolador de la linea de esp8266/esp32, modificando los puertos correspondientes dentro del código. Se sugiere usar el ESP01, por su bajo coste en comparación con los demas 

# Programación ESP01 

Si usted conoce el método de programación del ESP01, puede saltearse la presente sección. 

La programación puede realizarse de dos maneras: 

    1. Usando un adaptador CH340 a USB. Estos se pueden comprar comercialmente, y no se requiere el armado de un circuito externo
    2. Usando un adaptador USB a UART. Por ejemplo PL2303 o CP2102 

En este caso, se usa el adaptador CP2102. El circuito que debe armarse, responde al siguiente diagrama: 
![programadoresp01](images/programmeresp01.png "prog esp01")


Para realizar el test del circuito, para la carga del sketch, se debe instalar la placa ESP8266 dentro del [Arduino IDE](https://www.luisllamas.es/programar-esp8266-con-el-ide-de-arduino/ "guía para instalación esp8266 en arduino IDE"). Una vez instalada la placa ESP8266, dentro del arduino IDE, se debe elegir un código de ejemplo, y proceder de la siguiente forma, para poder cargar el código: 
    
    1 Presionar el boton de reset
    2 Presionar simultameamente el otro pulsador (GPIO 0)
    3 Soltar el boton de reset, manteniendo presionado el pulsador GPIO0 
    4 Soltar el boton de GPIO 0.

Una vez realizados estos pasos, se presiona el boton "upload" en el arduino IDE, y se debe cargar el sketch. Una vez finalizada la carga, se debe presionar nuevamente el boton de reset, para que el programa empiece a correr. 



# Estructura del proyecto

Aquí, se mencionan los archivos principales, y carpetas utilizadas para el desarrollo del proyecto. Estos son: 
```
 ├── sortedMACS.py
 ├── delay.html  
 ├── index.html
 ├── ReceptorBotonAprilp
        └── src 
 ├── ServidorBotonAprilp    
        └── src 
```
Los archivos [delay.html](delay.html) e [index.html](index.html), poseen la página principal del servidor. Estos códigos se encuentran replicados en ServidorBotonAprilp/src/webPages. 

La página [index.html](index.html), solamente tiene un boton, que es capaz de activar los retardos, y redirige a [delay.html](delay.html). Esta, se conecta mediante webSockets con el servidor, y le envia información del retardo. 

El archivo [sortedMACS.py](sortedMACS.py "lista ordenada de macs"), genera un archivo "macaddress.h" en la siguente ruta ServidorBotonAprilp/include/macaddress.h. Si no dispone de un interprete de python en su sistema, puede utilizar el archivo por defecto. El objetivo de este archivo, es ordenar todas las macs de espressif, de menor a mayor, para realizar busqueda dicotómica. 



## Software utilizado 

| herramienta | version  |
|-------------|----------|
| Python      | 3.9.6    |
| vscode      | 1.60.2   |
| Platformio  | 1.918.108|

Para conocer su versión de python, debe ejecutar el siguiente comando, en la consola de su sistema: 
    
  `python --version`

Si esta instalado, deberá verse la versión del mismo. Si usted no lo tiene instalado en su sistema, no podrá agregar lista de macs address ordenadas. Si usted desea usar este sistema, con otros tipos de chips que no sean de la compañia espressif, se recomienda su instalación. 

## configuración de Platformio ide 
Si usted clona este repositorio, la configuración de los archivos .ini, ya esta realizada. 
Platformio IDE, puede instalarse como una extensión de vscode. Para instalarlo siga [esta guía](https://platformio.org/install/ide?install=vscode "instalación platformio ide"). Cada proyecto nuevo, posee un archivo de configuración, denominado **platformio.ini**. Este archivo debe configurarse para el programa servidor como para el cliente. 

Archivo de configuración para el servidor
```ini
    [env:esp01_1m]
    platform = espressif8266
    board = esp01_1m
    framework = arduino
    lib_deps = links2004/WebSockets @ 2.3.6
    monitor_speed = 115200 
    upload_protocol = esptool
    board_build.flash_mode = dout 
```
Archivo de configuración para el cliente
```ini
    [env:esp01_1m]
    platform = espressif8266
    board = esp01_1m
    framework = arduino
    monitor_speed = 115200 
    upload_protocol = esptool
    board_build.flash_mode = dout 
```
# Servidor pulsador 

El servidor pulsador, reconoce a los botones clientes, mediante sus direcciones MACS. En este proyecto, se supone que todas las MAC-ADRRESS, pertenecen a la compañia espressif. Para crear este archivo se utliza el archivo sortedMACS.py

## Configuración de MACS ADDRESS de espressif 
El archivo python ordena las MACS de menor a mayor, para luego realizar una busqueda rápida en el procesamiento de las macs address. Se utiliza python 3.9.6. Se ingresan las MAC ADDRESS dentro del archivo sortedMACS.py, dentro de una lista denominada macAddress.  Una lista completa puede obtenerse desde [aquí](https://maclookup.app/vendors/espressif-inc " lista de mac address de espressif"). Una vez ingresada, se utiliza el método sort, y se genera el archivo macaddress.h, en ServidorBotonAprilp/include/. El código del archivo sortedMACS.py 


```Python 
   from io import FileIO
   macAddress = [
       addr1, 
       addr2,
       .... 
       addrn]
    macAddress.sort() ;  
    file = open('ServidorBotonAprilp/include/macaddress.h','w')
    for el in macAddress: 
    file.write("#define MAC_ADDRESS_ESPRESSIF_"+str(i)+" "+"0x"+str(str(hex(el)).split('0x')[1]).upper()+      '\n') 
    hexadecimal_number_Str = str(hex(el)).split('0x') ;  
    print(hexadecimal_number_Str[1])  
    i = i + 1  
```
## Definición de estructuras de datos 

El reconocimiento de las macs, requiere de 24 bits (3 bytes), denominadas OUI o vendoor. A partir de ellas, se reconoce el dispositivo. Se debe definir una estructura de datos, de 24bits. La definición de la estructura viene dada por: 
```C
    typedef struct 
    {
        unsigned int data: 24 ; 
    }   uint24_t ; 
```
La definición, que almacena las IP, y el estado de los retaros, se define de la siguiente manera: 
```cpp
    struct clientRegister
    {
        bool isDelay ; 
        unsigned int timeDelay ; 
        uint8_t Ipclients[4][4] ;    // ip clients esp8266 --  
    }; 
```
Estas definiciones, se realizan en el archivo [registroDelayWeb](ServidorBotonAprilp/src/registroDelayWeb.h "registros de datos")

## Desarrollo de software servidor

Se ha realizado en módulos, con las funciones distribuidas en tres archivos: 
 - main.cpp 
 - webPages.h 
 - WiFiFunctions.hpp 

El archivo, webPages.h, contiene las páginas html, delay.html e index.html. El archivo WiFiFunctions tiene las siguientes funciones 

|función     |retorno |descripción   | 
|--------    | ------------|------------ | 
|`createWiFiNet()`    |void | crea la red wifi | 
|`sendDataClient()`    |void | envia los datos de retardo y tiempo a los clientes esp8266 conectados|

El archivo main.cpp, contiene las siguientes funciones: 

|función     |retorno |descripción   | 
|--------    | ------------|------------ | 
|`mainPage()`|void| pagina web al conectarse por HTTP-  index.html| 
|`configDelayClockClient()` |void| sirve la página delay.html en el servidor   |
|`dataDelayWeb(uint8_t num, WStype_t type, uint8_t * payload, size_t length) `|ninguno |usada para el manejo de webSockets, se obtienen los valores de retardo de forma casi instantanea|
|`newConnectClient(WiFiEventSoftAPModeStationConnected sta_info) ` |void| ejecuta la función newConnectClient, al conectarse un cliente. Sin uso en la versión actual, se espera darle algún uso en futuras versiones. 
|`obtainIPClients()`  |void|obtiene las IPs de los clientes del tipo espressif conectados| 
|`isMacEspressif(uint24_t macsAdress)`  |int|busqueda binaria, si retorna 1, se encontro la mac dentro de los clientes, si retorna -1, no se encontro la mac| 


Las librerias utilizadas son:
- WebSocketsServer.h 
- ESP8266WebServer.h 



# MEJORAS 
# BUGS 