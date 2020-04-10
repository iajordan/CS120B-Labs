/*	Author: Ivannovi Jordan
 *  Partner(s) Name: 
 *	Lab Section: 28
 *	Assignment: Lab 2  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;

    DDRD = 0xFF; PORTD = 0x00;

    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;

    unsigned char totalweight;
    unsigned char over;
    unsigned char distribution;

    while(1){
	tmpA = PINA ;
	tmpB = PINB ;
	tmpC = PINC ;

	totalweight = (tmpA + tmpB + tmpC);
	over = (totalweight > 140) ? 0x01: 0x00;
	distribution = ((tmpA - tmpC > 80)|| (tmpC - tmpA > 80)) ? 0x02: 0x00;
	PORTD = (totalweight & 0xFC) | over | distribution;


    }
    return 0;
}


