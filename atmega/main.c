
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "servos.h"
#include "uart.h"
#include "config.h"
#include "funciones.h"

int main(void)
{

  initServos();
  serial_begin();
  pin_init();
  initNumMascarilla();
  while (1)
  {
  }

  return 0;
}
