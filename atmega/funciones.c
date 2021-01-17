
#include "funciones.h"
#include "servos.h"
#include "comprotocol.h"

unsigned char accionEncendidoUv = 0x14;
unsigned char accionGancho = 0x015;
unsigned char accionCompuerta = 0x16;
uint8_t accionNumMascarillas = 0x10;

uint8_t numMascarillas=10;

void recibir_encender_led()
{
  if (is_data_ready())
  {
    if (get_RX_buffer()[0] == 'H' && get_RX_buffer()[1] == 0)
    {

      serial_println_str("encendido");
      PORTB = (1 << PB0);
    }
    else
    {

      serial_println_str("apagado");
      PORTB &= ~(1 << PB0);
    }
  }
}

void recibirint()
{
  if (is_data_ready())
  {
    //serial_println_str(get_RX_buffer());
    recibido = get_RX_buffer();
    serial_println_str(recibido);
    arreglocreadoint(recibido);
    if (array_numint[0] == 1)
    {
      PORTB = (1 << PB0);
    }

    else
    {
      PORTB &= ~(1 << PB0);
    }
  }
}

void recibirfloat()
{
  if (is_data_ready())
  {
    //serial_println_str(get_RX_buffer());

    recibido = get_RX_buffer();
    serial_println_str(recibido);

    arreglocreadofloat(recibido);
    float hj = array_numfloat[0];
    //PORTB = array_numint[0];
    if (hj > 1.2)
    {
      PORTB = (1 << PB0);
      ;
    }

    else
    {
      PORTB &= ~(1 << PB0);
    }
  }
}

void arreglocreadoint(char *reci)
{
  char *token1 = strtok(reci, ",");
  int i = 0;
  // Keep printing tokens while one of the
  // delimiters present in str[].
  while (token1 != NULL)
  {
    // printf("%s\n", token);
    array_numint[i] = atoi(token1);
    token1 = strtok(NULL, ",");
    i++;
  }
}

void arreglocreadofloat(char *reci)
{

  char *token = strtok(reci, ",");
  int i = 0;
  // Keep printing tokens while one of the
  // delimiters present in str[].
  while (token != NULL)
  {
    // printf("%s\n", token);
    array_numfloat[i] = atof(token);
    token = strtok(NULL, ",");
    i++;
  }
}

void encenderLedUV(bool status)
{
  if (status)
  {
    PORTD = (1 << PB0);
  }
  else
  {
    PORTD = (0 << PB0);
  }
}

/*
servo id 1 -> gancho
servo id 2 -> compuerta
*/

void actionHandler(uint16_t *instruction)
{
  if (instruction[0] == accionGancho)
  {
    if (instruction[1] == 1)
    {
      numMascarillas++;
      comWrite(1, accionNumMascarillas, le(numMascarillas));
      posicionServos(1, 180);
    }
    else if (instruction[1] == 0)
    {
      posicionServos(1, 0);
    }
  }
  else if (instruction[0] == accionCompuerta)
  {
    if (instruction[1] == 1)
    {
      posicionServos(2, 90);
      numMascarillas--;
      comWrite(1, accionNumMascarillas, le(numMascarillas));
    }
    else if (instruction[1] == 0)
    {
      posicionServos(2, 0);
    }
  }
  else if (instruction[0] == accionEncendidoUv)
  {
    if (instruction[1] == 1)
    {
      encenderLedUV(true);
    }
    else
    {
      encenderLedUV(false);
    }
  }
}
