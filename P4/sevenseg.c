/*
 * sevemseg.c
 *
 * Created: 02.12.2022 22:30:19
 *  Author: tschn
 */ 
#include "sevenseg.h"

void sevenseg_init() {
	DDRD = 0xFF;
	//PORTD = digits[8];
	PORTD = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	// Timer initialisieren
	
	// 50 HZ -----------------------------------------
	// WGM12 => CTC (Clear Timer on Compare Match)
	// CS12 => 256 Prescaler
	//TCCR1B |= ((1 << WGM12) | (1 << CS12));
	// Output Compare Match A Interrupt Enable
	//TIMSK1 |= (1 << OCIE1A);
	
	// 50Hz = (16.000.000 / ((1249 + 1) * 256))
	//OCR1A = 1249;
	// ist mir zu langsam, man sieht es noch flackern!
	
	// 80 Hz ----------------------------------------
	// Prescaler 64
	TCCR1B |= ((1 << WGM12) | (1 << CS11) | (1 << CS10));
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 3124;
	
}