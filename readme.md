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
<!--Diagrama de conexión con esp01 --> 


Para realizar el test del circuito, para la carga del sketch, se debe instalar la placa ESP8266 dentro del [Arduino IDE](https://www.luisllamas.es/programar-esp8266-con-el-ide-de-arduino/ "guía para instalación esp8266 en arduino IDE"). Una vez instalada la placa ESP8266, dentro del arduino IDE, se debe elegir un código de ejemplo, y proceder de la siguiente forma, para poder cargar el código: 
    
    1 Presionar el boton de reset
    2 Presionar simultameamente el otro pulsador (GPIO 0)
    3 Soltar el boton de reset, manteniendo presionado el pulsador GPIO0 
    4 Soltar el boton de GPIO 0.

Una vez realizados estos pasos, se presiona el boton "upload" en el arduino IDE, y se debe cargar el sketch. Una vez finalizada la carga, se debe presionar nuevamente el boton de reset, para que el programa empiece a correr. 



# Estructura del proyecto

Aquí, se mencionan los archivos principales, y carpetas utilizadas para el desarrollo del proyecto. Estos son: 


## herramientas de software 

|Herramienta    | versión |       
|Python         | 3.9.6   |    
|vscode         | 1.60.2  |    
|Platformio     |1.918.108| 

Para conocer su versión de python, debe ejecutar el siguiente comando, en la consola de su sistema: 
    
  `python --version`

Si esta instalado, deberá verse la versión del mismo. Si usted no lo tiene instalado en su sistema, no podrá agregar lista de macs address ordenadas. Si usted desea usar este sistema, con otros tipos de chips que no sean de la compañia espressif, se recomienda que lo instale. 
    


## configuración de Platformio ide 

En cada proyecto nuevo, se crea un archivo de configuración, este se denomina *platformio.ini*. La configuración para el cliente es la siguiente: 

Para el servidor es la siguiente: 


# Servidor pulsador 
<!--
El servidor, crea la red WiFi en este caso, con los siguientes parámetros: 

Adicionalmente, posee un servidor web, con los archivos 

Estos archivos, se puede ingresar desde cualquier celular/pc, solo basta conectarse a la red WiFi creada por el mismo. La IP debe ingresar es "192.168.4.1". 
-->
```C++
    Serial.begin(960)
    Serial.println("hello world") ; 
    // this is a comment 
    const char b = 0x33
    //test using multiline in markDown 
```




## Reconocimientos de MAC ADDRESS de espressif
el archivo python ordena las MACS de menor a mayor, para luego realizar una busqueda rápida en el procesamiento de las macs address. Se utiliza python 3.9. 

- [Lista de MAC Adress](https://maclookup.app/vendors/espressif-inc " lista de mac address de espressif")   

# MEJORAS 
# BUGS 