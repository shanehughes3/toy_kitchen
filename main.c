#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define UCL_LED PD4
#define OVEN_LED PD5

// under-cabinet light button
ISR(INT0_vect)
{
	PORTD ^= _BV(UCL_LED);
}

// oven light button
ISR(INT1_vect)
{
	PORTD ^= _BV(OVEN_LED);
}

void main(void)
{
	MCUCR |= 0b1010; // set INT1 and INT0 to falling edge
	DDRD = (1 << PD4) | (1 << PD5);
	PORTD = (1 << PD4) | (1 << PD5);
	GIMSK |= (1 << INT0) | (1 << INT1);
	sei();
	while(1)
		;
}
      
