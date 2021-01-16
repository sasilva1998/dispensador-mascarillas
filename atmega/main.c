#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "servos.h"

int main(void)
{

    initServos();

    while (1)
    {
        posicionServos(1,90);
        posicionServos(2,90);
        _delay_ms(100);
    }

    return 0;
}
