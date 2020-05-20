/*	Author: Ivannovi Jordan -ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Video Link: https://drive.google.com/open?id=1xjEE_Pofuw0cnBYEaLdneCSt7vidn4v5
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
  unsigned short MAX = 799;
  unsigned short MIN = 39;

  ADC_init();
  while(1){
    unsigned short x = ADC;

    if (x >= MAX/2){
      tmpB = 0x01;
    }

    else {
      tmpB = 0x00;
    }

    PORTB = tmpB;
  }
}
