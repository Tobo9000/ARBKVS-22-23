/*
 * P3_Multitasking.c
 *
 * Created: 22.11.2022 15:13:44
 * Author : tschn
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL	// Taktfrequenz 16MHz
#include <util/delay.h>
#include <avr/interrupt.h>

// a) volatile:
// verhindert Compileroptimierung
// Variable wird im RAM statt in einem Register gespeichert
// darf (kann) nicht außerhalb des aktuellen Verwendungsortes geändert werden

// b)
// uint32_t => Overflow nach 4.294.967.296 ms => ungefähr 49,7 Tage
volatile uint32_t msCounter; // uint32_t => bis 4.294.967.295
// d) zB Sekundenzähler / Minutenzähler usw.
// wenn Sekundenzähler bei 60 => Minutenzähler + 1 und Sekundenzähler wieder auf 0...
volatile uint32_t secCounter; // increment bei 1000ms von msCounter

#define MS_COUNTER_MAX 4294967295 // = 2^32 - 1

// Interrupt bei 1ms
ISR(TIMER1_COMPA_vect) {
	msCounter++;
	if (msCounter == 1000) secCounter++;
}

// Interrupt bei Timer-Overflow
ISR(TIMER1_OVF_vect) {
}

// Wartet bis der Millisekundenzähler den Wert ms erreicht hat.
void waitUntil(uint32_t ms) {
	if (ms <= MS_COUNTER_MAX) {
		while (msCounter < ms) {}
	}
}

// Wartet unter Nutzung des Millisekundenzählers für ms Millisekunden.
void waitFor(uint32_t ms) {
	uint32_t waitTill = ms + msCounter;
	waitUntil(waitTill);
}

/*
Verkabelung:
PD0 an D0 (Led)
*/
int main(void)
{
    // PORTD als Output (PD0)
	DDRD = 0xff;
	// Output zunaechst 0
	PORTD = 0x00;
	
	// -------------------------------------------------------------
	// Timer/Counter Control Register B (S. 173 Datasheet):
	
	// Bits 7		6		5		4		3		2		1		0
	//      ICNC1 / ICES1 /       / WGM13 / WGM12 / CS12  / CS11  / CS10
	
	// CS10/11/12 = Clock Select 1n [n = 0..2], welche Clock soll genutzt werden?
	
	// 3 Bits => (CS12, CS11, CS10)
	//0 0 0 -> no clock source (Timer stopped)
	//0 0 1 -> /1 no prescaling
	//0 1 0 -> /8 From prescaler
	//0 1 1 -> /64 From prescaler     ==> wird hier genutzt (CS11/CS10)
	//1 0 0 -> /256 From prescaler
	//1 0 1 -> /1024 From prescaler
	//1 1 0 -> External clock source on T1 pin. Clock on falling edge
	//1 1 1 -> External clock source on T1 pin. Clock on rising edge
	
	// WGM = Waveform Generation Mode => control counting sequence of the counter...
	// WGM12 => Vergleichswert => wann soll Datenregister wieder auf 0 gesetzt werden?
	// da nur WGM12 gesetzt => nutze Mode 4 (CTC) (siehe S. 172) => TOP = OCR1A
	
	TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12);
	// setze Bits     1,            0 und          3
	
	// Timer-Interrupts (1) aktivieren (S. 184)
	// Output Compare A Match Interrupt Enable
	TIMSK1 = (1 << OCIE1A);
	// setze Bit 1
	
	// Output Compare Register 1 A
	// Match = Output compare interrupt
	// 16Mio Hz => 16Mio. Takte pro Sekunde => 16.000 Takte pro ms
	// 16.000 / 64 (prescaler) = 250 => also 250 Takte pro ms nach prescaler
	// 249 damit schon bei 1ms Takt der Interrupt kommt
	OCR1A = 249;
	
	// Interrupts global aktivieren
	sei();
	
	PORTD ^= (1 << PIND0);
	waitUntil(2000);
	PORTD ^= (1 << PIND0);
	waitUntil(5000);
	
    while (1) 
    {
		PORTD ^= (1 << PIND0);
		waitFor(1000);
    }
}

