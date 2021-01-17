#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

void initServos();

void posicionServos(int pos);
