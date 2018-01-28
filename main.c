#define F_CPU 1000000UL // default 8MHz/8 system clock
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define UCL_LED PD4
#define OVEN_LED PD5

// under-cabinet light button
EMPTY_INTERRUPT(INT0_vect);

// oven light button
EMPTY_INTERRUPT(INT1_vect);

// polling timer overflow
ISR(TIMER0_COMPA_vect)
{
	static uint8_t ucl_btn_state = 0xff;
	static uint8_t oven_btn_state = 0xff;
	ucl_btn_state = (ucl_btn_state << 1) | ((PIND & _BV(PD2)) >> 2);
	oven_btn_state = (oven_btn_state << 1) | ((PIND & _BV(PD3)) >> 3);
	if (ucl_btn_state == 0b10000000) {
		PORTD ^= _BV(UCL_LED);
	}
	if (oven_btn_state == 0b10000000) {
		PORTD ^= _BV(OVEN_LED);
	}
}

void main(void)
{
	// set INT1 and INT0 to falling edge
	MCUCR |= 0b1010;
	// PD4 and 5 as output
	DDRD = _BV(UCL_LED) | _BV(OVEN_LED); 
	// default both outputs to high and enable pull-up on inputs
	PORTD = _BV(PD4) | _BV(PD5) | _BV(PD2) | _BV(PD3);
	// enable INT0 and INT1 interrupts for wake-up
	GIMSK |= _BV(INT0) | _BV(INT1);
	// set timer0 compare for about 2.8ms
	OCR0A = 44;
	// enable timer0 compare A interrupt
	TIMSK |= _BV(OCIE0A);
	// set timer0 to reset on compare A match
	TCCR0A |= 0b010;
	// reset timer0
	TCNT0 = 0;
	// start timer, prescaler to clk/64
	TCCR0B |= 0b011;
	sei();
	/* while(1) { */
	/* 	if ((PORTD & _BV(UCL_LED)) != 0 && */
	/* 	    (PORTD & _BV(OVEN_LED)) != 0) { */
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
	/* 	} */
	/* } */
			while(1);
}
      
