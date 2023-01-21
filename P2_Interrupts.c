/*
 * P2_Interrupts.c
 *
 * Created: 02.11.2022 14:55:35
 * Author : tschn
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL // Taktfrequenz 16Mhz
#include <util/delay.h>

// enum zustaende { d0_blinkt, d9_blinkt, d0_leuchtet, d9_leuchtet };
//volatile enum zustaende status = d0_blinkt;

volatile int on = PORTB0;
volatile int off = PORTB1;
volatile int blinken = 1; // 0 = false, 1 = true

void init() {
	DDRD = 0x00;		// PIND als Input (PD2 fuer SW1 und PD3 fuer SW2)
	PORTD = 0xff;		// Pullup-Widerstaende
	
	DDRB = 0xff;		// PORTB als Output (PB0 fuer D0, PB1 fuer D9)
	PORTB = 0x00;		// alle LEDs aus
	
	//EICRA = 0b00001111; // Interrupt nur bei fallender Flanke (schalter LOW-Active)
	EICRA |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
	//EIMSK = 0b00000011; // externe Interrupts aktivieren
	EIMSK = (1 << INT0) | (1 << INT1);
	sei();
}

ISR (INT0_vect) {
	cli();
	if (on == PORTB0)
		blinken = (blinken == 0)? 1 : 0;
	
	off = PORTB1;
	on = PORTB0;
	
	/*
	if (status == d0_blinkt || status == d9_leuchtet) status = d0_leuchtet;
	else if (status == d0_leuchtet || status == d9_blinkt) status = d0_blinkt;
	*/
	sei();
}

ISR (INT1_vect) {
	cli();
	if (on == PORTB1)
		blinken = (blinken == 0)? 1 : 0;
	
	off = PORTB0;
	on = PORTB1;
	
	/*
	if (status == d0_blinkt || status == d9_leuchtet) status = d9_blinkt;
	else if (status == d0_leuchtet || status == d9_blinkt) status = d9_leuchtet;
	*/
	sei();
}

/* Verkabelung:
 * SW1 an PD2 -> PD2 ist int0_vect Interrupt
 * SW2 an PD3 -> PD3 ist int1_vect Interrupt
 * PB0 an D0 (LED)
 * PB1 an D9 (LED)
 */
int main(void)
{
	init();
	while (1) {
		
		if (blinken /*status == d0_blinkt || status == d9_blinkt*/) {
			PORTB &= ~(1UL << off);
			PORTB ^= (1 << on);
		} else {
			PORTB &= ~(1UL << off);
			PORTB |= (1 << on);
		}
		_delay_ms(200);
		
	}
	return 0;
}