/*
 * P4_SteuerungSevenSeg.c
 *
 * Created: 02.12.2022 22:22:42
 * Author : tschn
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sevenseg.h"
#include "keys.h"

int number = 42;
int digits[] = { // ggf. als hex-Darstellung
	0b01000000, // 0
	0b01111001, // 1
	0b00100100, // 2
	0b00110000, // 3
	0b00011001, // 4
	0b00010010, // 5
	0b00000010, // 6
	0b01111000, // 7
	0b00000000, // 8
	0b00010000  // 9
};

int main(void)
{
    keys_init();
	sevenseg_init();
	sei();
    while (1) 
    {
		//PORTB = 0x01;
		//PORTD = 0x00; // Sevenseg ist low-active
    }
	return 0;
}

ISR(TIMER1_COMPA_vect) {
	// Toggle Port B
	PORTB ^= 0x01;
	if (PORTB == 0x00) {
		PORTD = digits[number%10];
	} else {
		PORTD = digits[number/10];
	}
}

ISR(PCINT1_vect) {
	if (!(PINC & (1 << PINC0))) { // SW0 gedrueckt
		if (number <= 98)
			number++;
		else
			number = 0;
	} else if (!(PINC & (1 << PINC1))) { // SW1 gedrueckt
		if (number >= 1)
			number--;
		else
			number = 99;
	}
}

