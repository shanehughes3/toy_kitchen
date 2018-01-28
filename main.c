#define F_CPU 1000000UL // default 8MHz/8 system clock
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define UCL_LED PD4
#define OVEN_LED PD5

uint8_t ucl_btn_state = 0xff;
uint8_t oven_btn_state = 0xff;

// under-cabinet light button
EMPTY_INTERRUPT(INT0_vect);

// oven light button
EMPTY_INTERRUPT(INT1_vect);

// polling timer overflow
ISR(TIMER0_COMPA_vect)
{
	ucl_btn_state = (ucl_btn_state << 1) | ((PIND & PD2) >> 2);
	oven_btn_state = (oven_btn_state << 1) | ((PIND & PD3) >> 3);
	if (ucl_btn_state == 0b10000000) {
		PORTD ^= 1 << UCL_LED;
	}
	if (oven_btn_state == 0b10000000) {
		PORTD ^= 1 << OVEN_LED;
	}
}

void main(void)
{
	// set INT1 and INT0 to falling edge
	MCUCR |= 0b1010;
	// PD4 and 5 as output
	DDRD = (1 << UCL_LED) | (1 << OVEN_LED); 
	// default both outputs to high and enable pull-up on inputs
	PORTD = (1 << PD4) | (1 << PD5) | (1 << PD2) | (1 << PD3);
	// enable INT0 and INT1 interrupts
	GIMSK |= (1 << INT0) | (1 << INT1);
	// set timer0 compare for about 2.8ms
	OCR0A = 44;
	// enable timer0 compare A interrupt
	TIMSK |= 1 << OCIE0A;
	// set timer0 to reset on compare A match
	TCCR0A |= 0b010;
	// reset timer0
	TCNT0 = 0;
	// start timer, prescaler to clk/64
	TCCR0B |= 0b011;
	sei();
	while(1)
		;
}
      
