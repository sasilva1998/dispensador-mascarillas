
#include "funciones.h"
#include "servos.h"
#include "comprotocol.h"

#include <avr/eeprom.h>

unsigned char accionBanda = 0x04;
unsigned char accionCompuerta = 0x05;
uint8_t accionNumMascarillas = 0x02;

uint8_t numMascarillas EEMEM = 20;
char buffer[10];

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

/*
servo id 1 -> gancho
servo id 2 -> compuerta
*/

void actionHandler(uint16_t *instruction)
{ //maneja las acciones dependiendo de que envia la raspi
  serial_println_str("Ejecutando action handler");
  itoa(instruction[0], buffer, 16);
  serial_println_str(buffer);
  itoa(instruction[1], buffer, 16);
  serial_println_str(buffer);

  if (instruction[0] == 5)
  {
    cli();
    serial_println_str("abriendo compuerta");
    aumentoMascarilla(false);
    posicionServos(90);
    _delay_ms(100000);
    posicionServos(0);
    sei();
  }
  else if (instruction[0] == accionBanda)
  {
    if (instruction[1] == 1)
    {
      aumentoMascarilla(true);
      accionarBanda(1);
      _delay_ms(3000);
      accionarBanda(0);
    }
  }
}

void accionarBanda(bool status)
{
  if (status)
  {
    PORTC = (1 << PC0);
  }
  else
  {
    PORTC = (0 << PC0);
  }
}

void initNumMascarilla()
{ //inicia el numero base de las mascarillas
  eeprom_write_byte(&numMascarillas, 10);
}

void aumentoMascarilla(bool aumento)
{ //aumento o disminucion de mascarillas

  if (aumento)
  {
    uint8_t numMaskPast = eeprom_read_byte(&numMascarillas);
    numMaskPast++;
    eeprom_write_byte(&numMascarillas, numMaskPast);
    comWrite(2, accionNumMascarillas, numMaskPast);
  }
  else
  {
    uint8_t numMaskPast = eeprom_read_byte(&numMascarillas);
    numMaskPast--;
    eeprom_write_byte(&numMascarillas, numMaskPast);
    comWrite(2, accionNumMascarillas, numMaskPast);
  }
}

