
// librerias
#include "comprotocol.h"
#include "uart.h"
#include "funciones.h"

unsigned char eValues[2]; //usado para guardar un int en dos bytes
unsigned char checksum;
unsigned char packet[7]; //necesitado para generar la traman

uint16_t checksumsum; //checkeo de checksum

unsigned char incPacket[4]; //datos de paquete entrante

uint16_t instructionPacket[2] = {0x00, 0x00}; //paquete de instruccion (instruccion, param)

uint8_t deviceId = 0x02; //id para identificar el micro

ISR(USART_RX_vect)
{
  comRead();                        //lectura y validación de paquete de entrada
  actionHandler(instructionPacket); //envio de la entrada a ser manejada por funcion
}

void makePacket(uint8_t id, uint8_t inst, uint8_t params)
{
  //generacion de paquete de salida
  packet[0] = 0xff;
  packet[1] = 0xff;
  packet[2] = id;
  packet[3] = 0x03;
  packet[4] = inst;
  packet[5] = params;
  defineChecksum();
  packet[6] = checksum;
}

void comWrite(uint8_t id, uint8_t inst, uint8_t params)
{
  makePacket(id, inst, params); //creacion de paquete
  int i;
  for (i = 0; i < sizeof(packet); i++) //envio de paquete
  {
    serial_print_char(packet[i]);
  }
}

uint16_t *
comRead()
{
  unsigned char incChecksum = 0x00;
  unsigned char incByte;
  unsigned char fbyte = serial_read_char(); //obtencion de byte de entradas header
  unsigned char sbyte = serial_read_char();
  if (fbyte == 0xff && sbyte == 0xff)
  {
    incByte = serial_read_char();
    incPacket[0] = incByte; // id
    incByte = serial_read_char();
    incPacket[1] = incByte; // length
    incByte = serial_read_char();
    incPacket[2] = incByte; // instruction
    incByte = serial_read_char();
    incPacket[3] = incByte; // parametro
    incByte = serial_read_char();
    incChecksum = incByte;
  }
  if (checkChecksum(incChecksum, incPacket))
  {
    if (incPacket[0] == deviceId)
    {
      instructionPacket[0] = incPacket[1];
      instructionPacket[1] = incPacket[3];
      return instructionPacket;
    }
    else
    { //regreso de paquete vacio
      instructionPacket[0] = 0;
      instructionPacket[1] = 0;
      return instructionPacket;
    }
  }
  instructionPacket[0] = 0;
  instructionPacket[1] = 0;
  return instructionPacket;
}

unsigned char *
le(uint16_t h) //conversion a little endian dos bytes
{

  eValues[0] = (h >> 8);
  eValues[1] = (h & 0xff);
  return eValues;
}

uint16_t
word(uint8_t l, uint8_t h)
{

  uint16_t result = (h << 8) + l;
  return result;
}

void defineChecksum() //creacion de checksum
{
  checksumsum = 0;
  int i;
  for (i = 2; i < (sizeof(packet) - 1); i++)
  {
    checksumsum += packet[i];
  }
  le(~(checksumsum));
  checksum = eValues[0];
}

bool checkChecksum(unsigned char value, unsigned char *checkPacket)
{ //validacion de checksum
  unsigned char checkChecksum = 0;
  int i;
  for (i = 0; i < sizeof(checkPacket); i++)
  {
    checkChecksum += checkPacket[i];
  }
  le(~(checkChecksum));
  checkChecksum = eValues[0];
  if (checkChecksum == value)
  {
    return true;
  }
  return false;
}
