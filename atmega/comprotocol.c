#include "comprotocol.h"

#include "uart.h"

unsigned char eValues[2];
unsigned char checksum;
unsigned char packet[8];

unsigned char incomPacket[8];

void
makePacket (uint8_t id, uint8_t inst, uint8_t * params)
{
  packet[0] = 0xff;
  packet[1] = 0xff;
  packet[2] = id;

  if (params[1]==0xff)
  {
    packet[3]=0x03;
  }else
  {
    packet[3] = 0x04;
  }
  packet[4] = inst;
  int i;
  for (i = 0; i < sizeof (params); i++)
    {
      packet[i + 5] = params[i];
    }
  defineChecksum(packet);
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

void le(uint16_t h){
      
    eValues[0] = (h >> 8);
    eValues[1] = (h & 0xff);
}

uint16_t word(uint8_t l, uint8_t h){

  uint16_t result = (h<<8)+l;
  return uint16_t;
}

void defineChecksum(){
  checksum = 0;
  int i;
  for (i = 0; i < sizeof(packet); i++)
  {
    checksum+=packet[i];
  }
  le(~(checksum));
  checksum = eValues[0];
}


