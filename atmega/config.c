#include "config.h"


void
pin_init ()
{
  DDRD = 0xFF;
  DDRC = 0xFF;
}
