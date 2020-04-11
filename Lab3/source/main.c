/*	Author: Ivannovi Jordan
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 3  Exercise 3
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

     unsigned char weight = 0x000;
     unsigned char airbag;
     unsigned char tmpD;
     unsigned char tmpB;

     while(1){
         tmpD = PIND;
         tmpB = PINB & 0x001;
         weight = ((tmpD & 0x0FF) << 1) | tmpB & 0x001 ;
         if (weight >= 70){
             airbag= 0x02;
         }
         else if (weight > 5){
             airbag = 0x04;
         }
         else {
             airbag = 0x00;
         }
         PORTB = (airbag );
     }
     return 0;
 }
