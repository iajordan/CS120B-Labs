/*	Author: Ivannovi Jordan -ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  VIDEO LINK: https://drive.google.com/open?id=1Qx4y2-dorg-D6E8zeZsnaDnz9Z7mbogO
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


void ADC_init(){
  ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);

}



int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  DDRD = 0xFF; PORTD = 0x00;
  unsigned char tmpB ;
  unsigned char tmpD ;

  ADC_init();
  while(1){
    unsigned short x = ADC;
    tmpB = (char)x ;
    tmpD = (char)(x >> 8) ;
    PORTB = tmpB;
    PORTD = tmpD;

  }
}
