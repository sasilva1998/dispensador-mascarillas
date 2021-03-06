
#include "uart.h"

#define MAX_STR 50
/******************************************************************************************************************************/
/*                                                Funciones privadas o internas de la libreria						*/
/******************************************************************************************************************************/
volatile unsigned char rx_buffer[MAX_STR] = {0};

volatile unsigned char current_size = 0;
bool isReady = false;

/******************************************************************************************************************************/
/*                                                               Funciones públicas de la libreria						*/
/******************************************************************************************************************************/
/////////////////////////////////////////////
//inicialización del módulo USART AVR modo asíncrono
//en una función, a 8bits,a 9600 baudios, sin bit de paridad
//1 bit de parada
/////////////////////////////////////////////////////
void serial_begin()
{
  cli();
  UCSR0A = 0b00000000;
  UCSR0B = 0b00011000; //habilitar interrupcion por recepcion / transmisión y recepción habilitados a 8 bits
  UCSR0C = 0b00000110; //asíncrono, sin bit de paridad, 1 bit de parada a 8 bits
  UBRR0 = 103;         //para una velocidad de 9600 baudios con un //oscilador de 8Mhz
  sei();
}

///////////////////////////////////////////////
//recepción de datos del módulo USART AVR modo asíncrono
////////////////////////////////////////////////////////////
unsigned char
serial_read_char()
{
  if (UCSR0A & (1 << 7))
  {              //si el bit7 del registro UCSR0A se ha puesto a 1
    return UDR0; //devuelve el dato almacenado en el registro UDR0
  }
  else        //sino
    return 0; //retorna 0
}

///////////////////////////////////////////////
//transmisión de datos del módulo USART AVR modo asíncrono
///////////////////////////////////////////////
void serial_print_char(unsigned char caracter)
{
  if (caracter == 0)
    return;
  while (!(UCSR0A & (1 << 5)))
    ;              // mientras el registro UDR0 esté lleno espera
  UDR0 = caracter; //cuando el el registro UDR0 está vacio se envia el //caracter
}

//////////////////////////////////////////////
//transmisión de cadenas de caracteres con el módulo USART AVR modo asíncrono
///////////////////////////////////////////////
void serial_print_str(char *cadena)
{ //cadena de caracteres de tipo char
  while (*cadena != 0x00)
  { //mientras el último valor de la cadena sea diferente
    //al caracter nulo
    serial_print_char(*cadena); //transmite los caracteres de cadena
    cadena++;                   //incrementa la ubicación de los caracteres en cadena
    //para enviar el siguiente caracter de cadena
  }
}

void serial_println_str(char *cadena)
{
  serial_print_str(cadena);
  serial_print_char('\r');
  serial_print_char('\n');
}

char *
get_RX_buffer()
{
  current_size = 0;
  isReady = false;
  return (char *)rx_buffer;
}

bool is_data_ready()
{
  return isReady;
}
