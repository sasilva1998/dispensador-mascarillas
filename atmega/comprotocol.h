#include "uart.h"

uint8_t* makePacket(uint8_t id, uint8_t inst, uint8_t* params, uint8_t length);

void comWrite(uint8_t id, uint8_t inst, uint8_t* params);
uint8_t* comRead();

bool checkChecksum(uint8_t value,uint8_t* packet);
uint8_t defineChecksum(uint8_t* packet);

uint8_t* le(uint8_t h);
uint8_t word(uint8_t l, uint8_t h);