/*
 * WindSensor.c
 *
 * Created: 24.05.2019 2:42:30
 * Author : 1
 */ 
#define F_CPU 8000000L
#include <inttypes.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "uart.h"

#define LED_BIT PB5
long unsigned int wind_data[4];
char state1;
char state2;
char state3;
char state4;

ISR(PCINT2_vect) {

	if (state1 != (PIND & (1 << PD2))) {wind_data[0]++;state1 = (PIND & (1 << PD2));}
	if (state2 != !(PIND & (1 << PD3))) {wind_data[1]++;state2 = (PIND & (1 << PD3));}
	if (state3 != !(PIND & (1 << PD4))) {wind_data[2]++;state3 = (PIND & (1 << PD4));}
	if (state4 != !(PIND & (1 << PD5))) {wind_data[3]++;state4 = (PIND & (1 << PD5));}

}

int main(void)
{
  uint8_t i = 0;
  PORTD = 0b11111100;	
  PCMSK2 = 0b11111100;
  PCICR = 0b00000100;
  DDRB |= (1 << LED_BIT);
  wind_data[0] = 0;
  wind_data[1] = 0;
  wind_data[2] = 0;
  wind_data[3] = 0;
  
  for (i = 0; i < 5; i++) {
	  PORTB |= (1 << LED_BIT);
	  _delay_ms(50);
	  PORTB &= ~(1 << LED_BIT);
	  _delay_ms(50);
  }

  init_uart();

  sei();
	
  uart_puts("WindSensor v1.0");
  _delay_ms(100);
  while (1) 
  {
	  char data[100];
	  sprintf(data,"%lu %lu %lu %lu \r\n", wind_data[0],wind_data[1], wind_data[2], wind_data[3]);
	  uart_puts(data);
	  //uart_puts("WindSensor v1.0");
	   _delay_ms(1000);
  }
}

