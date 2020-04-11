/*	Author: Ivannovi Jordan
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 3  Exercise 5
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
    DDRD = 0x00; PORTD = 0xFF;

    DDRB = 0xFE; PORTB = 0x01;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char weight = 0x000;
    unsigned char airbagOff;
    unsigned char airbagOn;
    unsigned char tmpD;
    unsigned char tmpB;

    while(1){
        tmpD = PIND;
        tmpB = PINB & 0x001;
        weight = ((tmpD & 0x0FF) << 1) | tmpB;
        if (weight >= 0x046){
            airbagOn = 0x01;
            airbagOff = 0x00;
        }
        else if (weight >= 0x005){
            airbagOn = 0x00;
            airbagOff = 0x01;
        }
        else {
            airbagOn = 0x00;
            airbagOff = 0x00;
        }
        PORTB = ((weight & 0x01) << 3) | (airbagOff << 2 ) | ((airbagOn << 1) & 0x0E);
        PORTC = tmpD;
    }
    return 0;
}
