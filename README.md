# Dispensador y desinfectador de mascarillas

## Setup
En este repo encontrara todo lo relacionado a lo que se hizo para el proyecto mencionado, que fue simulado por el problema de covid.

Para ello considerar que en los distintos directorios se encuentra lo siguiente:

* `atmega`: puede en este directorio encontrar todos los códigos que se hicieron en C para la simulación del microcontrolador atmega328p y también se encuentra el archivo de proteus usado para la simulación así como el archivo `.hex` que corresponde al compilado del código a usar.
* `arduino`: se encuentra la simulación de proteus y dentro de esta el código usado para la misma.
* `raspi`: en esta carpeta se encuentran todos los códigos y algunas pruebas sobre la comunicación por mqtt.
* `diseño3d`: se puede encontrar el diseño 3D que se planeaba usar para el dispensador.

Adicionalmente se adjunta aqui el documento en overleaf https://www.overleaf.com/read/pnkwkxpyvcqj

Y también se agrega el video de youtube donde se explica también el proyecto.

https://www.youtube.com/watch?v=SHhLXYO--00&feature=youtu.be

Cabe admeás recalcar que se usa google assistant y los servicios de IFTTT Adafruit IO y Ubidots.


## Documentacion para futuro debuging



### servo control
para positivo

2189 -> 0 en servo positivo
3198 -> 180 en servo positivo

para negativo

3799 -> 0
2800 -> 180

### comunicacion

length -> no es le

The length of the Packet(Instruction, Parameter, Checksum fields). Length = number of Parameters + 2. Para nuestro caso, la instruccion y el checksum consumen un byte cada uno, por lo que el largo corresponde al numero de parametros que es máximo 2.

https://emanual.robotis.com/docs/en/dxl/protocol1/

tomo al atmega como id=1
y al arduino como id=0

#### checksum
Instruction Checksum = ~( ID + Length + Instruction + Parameter1 + … Parameter N )

### para reuniones

https://meet.jit.si/proyectoembebidos


### workflow

arduino 1 entrada que es para ingresar mascarilla
1 salida que es para aviso de finalizacion de proceso
-> le avisa a la raspberry y la raspi avisa al atmega

atmega
1 entrada para abrir compuerta
otra entrada de la banda transportadora

1 salido el numero de mascarillas

almacenar el numero de mascarillas en eeprom

ID 0 atmega
ID 1 arduino

trama ejemplo
[255,255,1,2,5,248]