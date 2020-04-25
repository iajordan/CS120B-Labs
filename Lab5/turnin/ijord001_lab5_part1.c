/*	Author: sp4
 *  Partner(s) Name:Ivannvoi Jordan ijord001@ucr.edu
 *	Lab Section:28
 *	Assignment: Lab 5  Exercise 1
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
  DDRC = 0xFF; PORTC = 0x00;
  unsigned char gague ;
  unsigned char lowFuel ;
  unsigned char tmpA ;

  while(1){
      tmpA = ~PINA & 0x0F;
      if (tmpA >= 13){
        gague = 0x3F;
        lowFuel = 0x00;
      }
      else if(tmpA >= 10){
        gague = 0x3E;
        lowFuel = 0x00;
      }
      else if (tmpA >= 7){
        gague = 0x3C;
        lowFuel = 0x00;
      }
      else if (tmpA >= 5){
        gague = 0x38;
        lowFuel = 0x00;
      }
      else if (tmpA >= 3){
        gague = 0x30;
        lowFuel = 0x40;

      }
      else if(tmpA >= 1){
        gague = 0x20;
        lowFuel = 0x40;
      }
      else {
        gague = 0x00;
        lowFuel = 0x00;
      }
      PORTC = (gague & 0x3F) | lowFuel ;
    }
      return 1;
  }
