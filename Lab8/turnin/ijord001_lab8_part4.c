/*	Author: Ivannovi Jordan -ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Video Link: https://drive.google.com/open?id=1i5OQTgZ1cUoMNa1nOUI3_gpD-WWc6Uhn
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

    if (x <= MAX / 8 ){
      tmpB = 0x01;
      PORTB = tmpB;
    }
    else if (x <= (MAX * 2) / 8 ){
      tmpB = 0x03;
      PORTB = tmpB;
    }
    else if (x <= (MAX * 3) / 8 ){
      tmpB = 0x07;
      PORTB = tmpB;
    }

    else if (x <= (MAX * 4)/ 8  ){
      tmpB = 0x0F;
      PORTB = tmpB;
    }

    else if (x <= (MAX * 5 )/ 8 ){
      tmpB = 0x1F;
      PORTB = tmpB;
    }

    else if (x <= (MAX * 6)/ 8  ){
      tmpB = 0x3F;
      PORTB = tmpB;
    }

    else if (x <= (MAX * 7)/ 8  ){
      tmpB = 0x7F;
      PORTB = tmpB;
    }

    else if (x <= (MAX * 8) / 8 ){
      tmpB = 0xFF;
      PORTB = tmpB;
    }



  }
}
