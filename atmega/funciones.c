
#include "funciones.h"
#include "servos.h"
#include "comprotocol.h"

#include <avr/eeprom.h>

unsigned char accionBanda = 0x04;
unsigned char accionCompuerta = 0x05;
uint8_t accionNumMascarillas = 0x02;
uint8_t accionNumMascarillasAgotadas = 0x07;

uint8_t numMascarillas EEMEM = 20;
uint8_t initMask EEMEM = 10;

char buffer[10];

void actionHandler(uint16_t *instruction)
{ //maneja las acciones dependiendo de que envia la raspi
  serial_println_str("Ejecutando action handler");
  // itoa(instruction[0], buffer, 16); //para debugging
  // serial_println_str(buffer);
  // itoa(instruction[2], buffer, 16);
  // serial_println_str(buffer);
  // itoa(instruction[3], buffer, 16);
  // serial_println_str(buffer);

  if (instruction[1] == 5)
  {
    serial_println_str("abriendo compuerta para salida de mascarilla");
    aumentoMascarilla(false);
    posicionServos(90);
    _delay_ms(50000);
    posicionServos(0);
  }
  else if (instruction[1] == 4)
  {
    serial_println_str("Aumentando mascarilla - accionando banda");
    aumentoMascarilla(true);
    accionarBanda(1);
    _delay_ms(50000);
    accionarBanda(0);
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
  uint8_t statusMaskInit = eeprom_read_byte(&initMask);
  if (statusMaskInit == 0)
  {
    eeprom_write_byte(&numMascarillas, 10);
    eeprom_write_byte(&initMask, 1);
  }
}

void aumentoMascarilla(bool aumento)
{ //aumento o disminucion de mascarillas

  if (aumento) //se aumenta una mascarilla y se notifica a raspi
  {
    uint8_t numMaskPast = eeprom_read_byte(&numMascarillas);
    numMaskPast++;
    eeprom_write_byte(&numMascarillas, numMaskPast);
    comWrite(2, accionNumMascarillas, numMaskPast);
  }
  else
  { //se disminuye una mascarilla
    uint8_t numMaskPast = eeprom_read_byte(&numMascarillas);
    if (numMaskPast == 1) //si solo queda una no disminuye y avisa el agotado de mascarillas
    {
      serial_println_str("Se acabaron las mascarillas");
      comWrite(2, accionNumMascarillasAgotadas, 0);
    }
    else
    {
      numMaskPast--;
      eeprom_write_byte(&numMascarillas, numMaskPast);
      comWrite(2, accionNumMascarillas, numMaskPast);
    }
  }
}
