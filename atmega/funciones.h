
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "uart.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "comprotocol.h"

//funciones de movimiento y acciones
void actionHandler(uint16_t *instruction);
void accionarBanda(bool status);

//numeracion de mascarilla
void initNumMascarilla();
void aumentoMascarilla(bool aumento);
