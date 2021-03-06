#include "uart.h"

void makePacket(uint8_t id, uint8_t inst, uint8_t params);

void comWrite(uint8_t id, uint8_t inst, uint8_t params);

bool checkChecksum(unsigned char value, unsigned char *checkPacket);
void defineChecksum();

unsigned char *le(uint16_t h);
uint16_t word(uint8_t l, uint8_t h);
