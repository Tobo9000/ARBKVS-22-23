/*
 * P2_Polling.c
 *
 * Created: 02.11.2022 15:21:35
 * Author : tschn
 *
 * siehe auch: https://aticleworld.com/how-to-set-clear-and-toggle-a-single-bit-in-cc/
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL // Taktfrequenz 16Mhz
#include <util/delay.h>

void init();

enum status { d0_blinkt, d9_blinkt, d0_leuchtet, d9_leuchtet };

/* Verkabelung:
 * SW1 an PD2
 * SW2 an PD3
 * PB0 an D0 (LED)
 * PB1 an D9 (LED)
 */
int main(void)
{
	init();
	enum status status = d0_blinkt;
	int on = PORTB0;
	int off = PORTB1;
	
    while (1) 
    {
		if (!(PIND & (1 << PIND2))) {			// SW1 gedrueckt
			_delay_ms(5);						// moegl. "wackeln" abfangen
			off = PORTB1;
			on = PORTB0;
			if (status == d0_blinkt || status == d9_leuchtet) status = d0_leuchtet;
			else if (status == d0_leuchtet || status == d9_blinkt) status = d0_blinkt;
		} 
		else if (!(PIND & (1 << PIND3))) {		// SW2 gedrueckt
			_delay_ms(5);						// moegl. "wackeln" abfangen
			off = PORTB0;
			on = PORTB1;
			if (status == d0_blinkt || status == d9_leuchtet) status = d9_blinkt;
			else if (status == d0_leuchtet || status == d9_blinkt) status = d9_leuchtet;
		}
		
		if (status == d0_blinkt || status == d9_blinkt) {
			PORTB &= ~(1UL << off);	// loesche off-Bit (clear nth bit)
			PORTB ^= (1 << on);		// XOR (Bit togglen)
		} 
		else {
			PORTB &= ~(1UL << off);	
			PORTB |= (1 << on);		// LED ein, falls noch nicht ein
		}
		
		_delay_ms(200);
    }
	
	return 0;
}

void init() {
	DDRD = 0x00;	// PIND als Input (PD2 fuer SW1 und PD3 fuer SW2)
	PORTD = 0xff;	// Pullup-Widerstaende
	
	DDRB = 0xff;	// PORTB als Output (PB0 fuer D0, PB1 fuer D9)
	PORTB = 0x00;	// alle LEDs aus
}

