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