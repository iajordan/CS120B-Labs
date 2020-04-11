/*	Author: Ivannovi Jordan
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 3  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
int main(void){

DDRA = 0x00; PORTA = 0xFF;

DDRB = 0xFF; PORTB = 0x00;
DDRC = 0xFF; PORTC = 0x00;

unsigned char tmpC;
unsigned char tmpB;

while(1){
    tmpB = (PINA & 0xF0)>> 4;
    tmpC = (PINA & 0x0F) << 4;

    PORTB = tmpB;
    PORTC = tmpC;
  }
    return 0;
}
