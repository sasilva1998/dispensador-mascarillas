
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
  pin_init();
  initServos();
  posicionServos(0);
  serial_begin();
  initNumMascarilla();
  serial_println_str("Encendido");
  while (1)
  {
  }

  return 0;
}
