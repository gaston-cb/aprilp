# Encendido remoto de juguetes a distancia

El proyecto, controla el encendido remoto (a distancia) de un juguete, en base a un receptor. Este receptor, se conecta a la red WiFi creada por el pulsador. Este pulsador al ser presionado, le envía la orden de encendido a todos los clientes (receptores), el cual realiza la activación de todos juntos. El servidor, y cliente, es alimentado mediante dos pilas AA de 1.5 V. También, puede utilizarse para encender cualquier otro tipo de circuito. El sistema diseñado, responde al siguiente esquema: 


La institución, que probará y realizará los test correspondientes a software y hardware, es APRILP. 
# Materiales 
    1. ESP01 (al menos 2) 
    2. PC817 (optoacoplador) uno por cada cliente que desee
    3. Resistencias
        3.1 1K \Omega
        3.2 10K  
    4. Portapilas 
    5. Programador ESP01 o simil 








## Reconocimientos de MAC ADDRESS de espressif
el archivo python ordena las MACS de menor a mayor, para luego realizar una busqueda rápida en el procesamiento de las macs address. Se utiliza python 3.9. https://maclookup.app/vendors/espressif-inc - Lista de macAdress   

## 