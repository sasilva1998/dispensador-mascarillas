#include "comprotocol.h"

#include "uart.h"

unsigned char eValues[2];
unsigned char checksum;
unsigned char packet[8];

uint16_t checksumsum;

unsigned char incPacket[5];

uint16_t instructionPacket[2];

uint8_t deviceId = 0x01;

void makePacket(uint8_t id, uint8_t inst, uint8_t *params)
{
  packet[0] = 0xff;
  packet[1] = 0xff;
  packet[2] = id;

  if (params[1] != 0x00)
  {
    packet[3] = 0x03;
  }
  else
  {
    packet[3] = 0x04;
  }
  packet[4] = inst;
  int i;
  for (i = 0; i < sizeof(params); i++)
  {
    packet[i + 5] = params[i];
  }
  defineChecksum();
  packet[7] = checksum;
}

void comWrite(uint8_t id, uint8_t inst, uint8_t *params)
{
  makePacket(id, inst, params);
  int i;
  for (i = 0; i < sizeof(packet); i++)
  {
    serial_print_char(packet[i]);
  }
}

uint16_t *
comRead()
{

  unsigned char incChecksum;
  unsigned char incByte;
  unsigned char fbyte = serial_read_char();
  unsigned char sbyte = serial_read_char();
  if (fbyte == 0xff && sbyte == 0xff)
  {
    incByte = serial_read_char();
    incPacket[0] = incByte;
    incByte = serial_read_char();
    incPacket[1] = incByte;
    incByte = serial_read_char();
    incPacket[2] = incByte;
    incByte = serial_read_char();
    incPacket[3] = incByte;
    incByte = serial_read_char();
    incPacket[4] = incByte;
    incByte = serial_read_char();
    incChecksum = incByte;
  }
  if (checkChecksum(incChecksum, incPacket))
  {
    if (incPacket == deviceId)
    {
      instructionPacket[0] = incPacket[0];
      instructionPacket[1] = word(incPacket[3], incPacket[4]);
      return instructionPacket;
    }
    else
    {
      instructionPacket[0] = 0;
      instructionPacket[1] = 0;
      return instructionPacket;
    }
  }
}

unsigned char *
le(uint16_t h)
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

void defineChecksum()
{
  checksumsum = 0;
  int i;
  for (i = 2; i < (sizeof(packet)-1); i++)
  {
    checksumsum += packet[i];
  }
  le(~(checksumsum));
  checksum = eValues[0];
}

bool checkChecksum(unsigned char value, unsigned char *checkPacket)
{
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
