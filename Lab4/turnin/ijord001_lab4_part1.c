/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, PRESS, RELEASE} state;
unsigned char tmpA;
unsigned char tmpB;

void Tick(){

  switch(state){
    case START:
      state = RELEASE;
      break;
    case RELEASE:
      tmpA = PINA;
      state = (tmpA)? PRESS: RELEASE;
      break;
    case PRESS:
      tmpA = PINA;
      state = (tmpA)? PRESS: RELEASE;
      break;
    default:
      state = START;
      break;
  }
  switch (state){
    case START:
      tmpB = 0x01;
      PORTB = tmpB;
      break;
    case RELEASE:
      tmpA = PINA;
      PORTB = tmpB & 0x03;
      break;
    case PRESS:
      PORTB = (~tmpB & 0x03);
      break;
    default:
      break;
  }
}

int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;

  tmpA = PINA;
  tmpB = 0x01;
  state = START;
  while (1) {
    Tick();
    }
  return 1;
}
