;
; P1_Lauflicht.asm
;
; Created: 28.09.2022 13:36:20
; Author : tschn
;

.include "m328pdef.inc"		; Controller Konstanten
.CSEG						; "hier kommt Code"

; Interrupt-Vektoren
; (bei reset zu start springen)
.ORG 0x000
	rjmp START

START:
	; Stack initialisieren
    ldi r16, LOW(RAMEND)
	out SPL, r16
	ldi r16, HIGH(RAMEND)
	out SPH, r16

	; PD0 bis PD7 als Output
	ldi r16, 0b11111111 	; SET HIGH fuer Output
	out DDRD, r16			; Data Direction Register (Port D) => alle D-Ports als Output festlegen
	
	ldi r19, 0b00000011		; PB0 und PB1 als Output fuer LED 9 u. 10 setzen
	out DDRB, r19			

	ldi r16, 0b00000001		; "Laufregister", Startkonfiguration (PD0-Led leuchtet)
	ldi r17, 0b00000001		; rechtes Ende definieren
	ldi r18, 0b10000000		; linkes Ende definieren

	out PORTD, r16			; erste LED an
	rcall DELAY	

; Schleife bis zum linken Ende
LEFT:
	lsl r16					; eine Pos. nach links schieben
	out PORTD, r16			; anzeigen
	rcall DELAY
	cp r18,r16				; linkes Ende erreicht? (r18 == r16)
	breq END_B				; ja => bei den B-Ports weiter
	rjmp LEFT				; nein => weiter nach links

; fuehrt den Richtungswechsel bei den B-Ports durch
END_B:
	ldi r16, 0		
	out PORTD, r16			; D komplett aus
	sbi PORTB, 0			; sbi = Set Bit in I/O Register (0-tes Bit auf 1)
	rcall DELAY				
	cbi PORTB, 0			; Clear Bit in I/O Register
	sbi PORTB, 1			; usw...
	rcall DELAY
	cbi PORTB, 1
	sbi PORTB, 0
	rcall DELAY
	cbi PORTB, 0
	ldi r16, 128			
	out PORTD, r16			; LED 8 wieder ein, damit wieder lsr gemacht werden kann
	rcall DELAY
	rjmp RIGHT

; Schleife bis zum rechten Ende
RIGHT:
	lsr r16
	out PORTD, r16
	rcall DELAY
	cp r17,r16
	breq LEFT
	rjmp RIGHT

; DELAY soll 200ms (1/5 Sek.) dauern
; bei 16MHz => 3.200.000 Zyklen notwendig (-4 Overhead von rcall / ret)
; vgl. http://darcy.rsgc.on.ca/ACES/TEI4M/AVRdelay.html
DELAY:
    ldi  r19, 17
    ldi  r20, 60
    ldi  r21, 202
L1: dec  r21
    brne L1
    dec  r20
    brne L1
    dec  r19
    brne L1
	ret
