
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "servos.h"
#include "uart.h"


int main(void)
{

    initServos();
    serial_begin();
    while (1)
    {
        posicionServos(1,90);
        posicionServos(2,90);
        _delay_ms(1000);
        serial_print_char(0xff);
    }

    return 0;
}
