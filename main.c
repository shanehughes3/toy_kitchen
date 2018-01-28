#define F_CPU 1000000UL // default 8MHz/8 system clock
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define UCL_LED PD4
#define OVEN_LED PD5

typedef enum action {
	INCREMENT,
	RESET
} Action;

uint8_t ovf_count_handle(Action action) {
	static uint8_t ovf_count = 0;
	switch (action) {
	case INCREMENT:
		++ovf_count;
		break;
	case RESET:
		ovf_count = 0;
		break;
	}
	return ovf_count;
}

// interrupt on button input (to wake from sleep)
ISR(PCINT_vect)
{
	// re-enable watchdog interrupt (if disabled)
	WDTCSR |= _BV(WDIE);
	ovf_count_handle(RESET);
}


// polling timer overflow
ISR(TIMER0_COMPA_vect)
{
	static uint8_t ucl_btn_state = 0xff;
	static uint8_t oven_btn_state = 0xff;
	ucl_btn_state = (ucl_btn_state << 1) | (PINB & _BV(PB0));
	oven_btn_state = (oven_btn_state << 1) | ((PINB & _BV(PB1)) >> 1);
	if (ucl_btn_state == 0b10000000) {
		PORTD ^= _BV(UCL_LED);
	}
	if (oven_btn_state == 0b10000000) {
		PORTD ^= _BV(OVEN_LED);
	}
}

// watchdog timer overflow
ISR(WDT_OVERFLOW_vect)
{
	if (ovf_count_handle(INCREMENT) > 1) {
		// turn off leds (set to high)
		PORTD = _BV(PD4) | _BV(PD5);
		// reset count, go to sleep
		ovf_count_handle(RESET);
		// disable watchdog interrupt
		WDTCSR |= ~_BV(WDIE);
		sei();
		sleep_mode();
	} else {
		// re-enable interrupt (cleared on trigger)
		WDTCSR |= _BV(WDIE);
	}
}

void main(void)
{
	// PD4 and 5 as output
	DDRD = _BV(UCL_LED) | _BV(OVEN_LED); 
	// default both outputs to high
	PORTD = _BV(PD4) | _BV(PD5);
	// enable pull-up on inputs
	PORTB = _BV(PB0) | _BV(PB1);
	// enable pin change interrupts
	GIMSK |= _BV(PCIE);
	PCMSK = _BV(PCINT0) | _BV(PCINT1);
	// set timer0 compare for about 2.8ms
	OCR0A = 44;
	// enable timer0 compare A interrupt
	TIMSK |= _BV(OCIE0A);
	// set timer0 to reset on compare A match
	TCCR0A |= 0b010;
	// start timer, prescaler to clk/64
	TCCR0B |= 0b011;
	// watchdog timer setup - enable interrupt, wdt-clk/1024 (8s interval)
	WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0);
	
	sei();

	// inital power-down
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	
	// stay busy before timeout
	while(1);
}
      
