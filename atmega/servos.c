#include "servos.h"
#include <stdlib.h>
void initServos()
{
  // se usa 16 mhz

  DDRB |= 1 << PINB1; // Set pin 9 on arduino to output
  DDRB |= 1 << PINB2;
  DDRB |= 1 << PINB3;
  /* 1. Set Fast PWM mode 14: set WGM11, WGM12, WGM13 to 1 */
  /* 3. Set pre-scaler of 8 */
  /* 4. Set Fast PWM non-inverting mode */
  TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
  /* 2. Set ICR1 register: PWM period */
  ICR1 = 39999;
}

void posicionServos(int pos)
{
  if (pos >= 0)
  {
    pos = (pos + 2) * 5.55;
    pos = pos + 2989;
    OCR1A = pos;
  }
  if (pos < 0)
  {
    pos = (abs(pos)) * 5.55;
    pos = 2999 - pos;
    OCR1A = pos;
  }
}
