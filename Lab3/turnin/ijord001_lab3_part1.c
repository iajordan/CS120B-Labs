/*	Author: Ivannovi Jordan ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 3  Exercise 1
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
DDRB = 0x00; PORTB = 0xFF;

DDRC = 0xFF; PORTC = 0x00;

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;

unsigned char cnt;

while(1){
    tmpA = PINA;
    tmpB = PINB;

    cnt = ((tmpA & 0x80)>> 7) + ((tmpA & 0x40)>> 6) + ((tmpA & 0x20)>> 5) +
    ((tmpA & 0x10)>> 4) + ((tmpA & 0x08)>> 3) + ((tmpA & 0x04)>> 2) +
    ((tmpA & 0x02)>> 1) + ((tmpA & 0x01)) + ((tmpB & 0x80)>> 7) +
    ((tmpB & 0x40)>> 6) + ((tmpB & 0x20)>> 5) + ((tmpB & 0x10)>> 4) +
    ((tmpB & 0x08)>> 3) + ((tmpB & 0x04)>> 2) + ((tmpB & 0x02)>> 1) + ((tmpB & 0x01)) ;

    PORTC = cnt;
  }
    return 0;
}
