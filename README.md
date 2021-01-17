# Documentacion para futuro debuging

## servo control
para positivo

2189 -> 0 en servo positivo
3198 -> 180 en servo positivo

para negativo

3799 -> 0
2800 -> 180

## comunicacion

length -> no es le

The length of the Packet(Instruction, Parameter, Checksum fields). Length = number of Parameters + 2. Para nuestro caso, la instruccion y el checksum consumen un byte cada uno, por lo que el largo corresponde al numero de parametros que es máximo 2.

https://emanual.robotis.com/docs/en/dxl/protocol1/

tomo al atmega como id=1
y al arduino como id=0

## para reuniones

https://meet.jit.si/proyectoembebidos


## workflow

arduino 1 entrada que es para ingresar mascarilla
1 salida que es para aviso de finalizacion de proceso
-> le avisa a la raspberry y la raspi avisa al atmega

atmega
1 entrada para abrir compuerta
otra entrada de la banda transportadora

1 salido el numero de mascarillas

almacenar el numero de mascarillas en eeprom

