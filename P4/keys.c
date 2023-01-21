/*
 * keys.c
 *
 * Created: 02.12.2022 22:36:22
 *  Author: tschn
 */ 
#include "keys.h"

void keys_init() {
	DDRC = 0x00;
	PORTC = 0xFF; // Pullups
	// Pin change interrupts
	// siehe zB hier:
	// https://thewanderingengineer.com/2014/08/11/arduino-pin-change-interrupts/
	PCICR |= 0b00000010;
	PCMSK1 |= 0b00000011; // PC0 and PC1 interrupts
}
