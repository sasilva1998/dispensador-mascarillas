#include "comprotocol.h"

uint8_t *header[2] = { 0xFF, 0xFF };

uint8_t *
makePacket (uint8_t id, uint8_t inst, uint8_t * params, uint8_t length)
{
  uint8_t *packet[4 + length];
  packet[0] = 0xff;
  packet[1] = 0xff;
  packet[2] = id;
  packet[3] = length;
  packet[4] = inst;

  int i;
  for (i = 0; i < sizeof (params); i++)
    {
      packet[i + 5] = params[i];
    }
  uint8_t *tempPacket = packet;
  packet[4 + length] = defineChecksum (tempPacket);
}
