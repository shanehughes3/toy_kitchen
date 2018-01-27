#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define UCL_LED PD3
#define OVEN_LED PD4

// under-cabinet light button
ISR(INT0_vect)
{
	PORTD ^= _BV(UCL_LED);
}

// oven light button
ISR(INT1_vect)
{
	PORTD ^= _BV(UCL_LED);
}

void main(void)
{
	MCUCR |= 0b1010; // set INT1 and INT0 to falling edge
	DDRD = 0b11000; // PD3 and PD4 to output
	PORTD = 0b11000; // pull outputs high - will sink current when on
	sei();
	while(1)
		;
}
      
