#include "comprotocol.h"

#include "uart.h"

unsigned char eValues[2];
unsigned char checksum;
unsigned char packet[8];

unsigned char incPacket[5];

uint8_t deviceId = 0x01;

uint8_t incId;
uint8_t incInst;
unsigned char incParams[2];

void
makePacket (uint8_t id, uint8_t inst, uint8_t * params)
{
  packet[0] = 0xff;
  packet[1] = 0xff;
  packet[2] = id;

  if (params[1] == 0xff)
    {
      packet[3] = 0x03;
    }
  else
    {
      packet[3] = 0x04;
    }
  packet[4] = inst;
  int i;
  for (i = 0; i < sizeof (params); i++)
    {
      packet[i + 5] = params[i];
    }
  defineChecksum (packet);
  packet[7] = checksum;
}

void
comWrite (uint8_t id, uint8_t inst, uint8_t * params)
{
  makePacket (id, inst, params);
  int i;
  for (i = 0; i < sizeof (packet); i++)
    {
      serial_print_char (packet[i]);
    }
}

void
comRead ()
{
  unsigned char incChecksum;
  unsigned char incByte;
  unsigned char fbyte = serial_read_char ();
  unsigned char sbyte = serial_read_char ();
  if (fbyte == 0xff && sbyte == 0xff)
    {
      incByte = serial_read_char ();
      incPacket[0] = incByte;
      incByte = serial_read_char ();
      incPacket[1] = incByte;
      incByte = serial_read_char ();
      incPacket[2] = incByte;
      incByte = serial_read_char ();
      incPacket[3] = incByte;
      incByte = serial_read_char ();
      incPacket[4] = incByte;
      incByte = serial_read_char ();
      incChecksum = incByte;
    }
  if (checkChecksum (incChecksum, incPacket))
    {
      //TODO
    }

}

void
le (uint16_t h)
{

  eValues[0] = (h >> 8);
  eValues[1] = (h & 0xff);
}

uint16_t
word (uint8_t l, uint8_t h)
{

  uint16_t result = (h << 8) + l;
  return result;
}

void
defineChecksum ()
{
  checksum = 0;
  int i;
  for (i = 0; i < sizeof (packet); i++)
    {
      checksum += packet[i];
    }
  le (~(checksum));
  checksum = eValues[0];
}

bool
checkChecksum (unsigned char value, unsigned char *checkPacket)
{
  unsigned char checkChecksum = 0;
  for (i = 0; i < sizeof (checkPacket); i++)
    {
      checkChecksum += packet[i];
    }
  le (~(checkChecksum));
  checkChecksum = eValues[0];
  if (checkChecksum == value)
    {
      return true;
    }
  return false;
}
