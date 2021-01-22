
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
  unsigned char fbyte = 0;
  unsigned char sbyte = 0;
  uint16_t incomingPacket[2];
  char buffer[10];
  while (1)
  {

    fbyte = serial_read_char();
    sbyte = serial_read_char();
    _delay_ms(300);

    // itoa(fbyte, buffer, 16);
    // serial_println_str(buffer);
    // itoa(sbyte, buffer, 16);
    // serial_println_str(buffer);

    if (fbyte != 0 && sbyte != 0)
    {
      incomingPacket[0] = fbyte;
      incomingPacket[1] = sbyte;
      actionHandler(incomingPacket);
    }
  }

  return 0;
}
