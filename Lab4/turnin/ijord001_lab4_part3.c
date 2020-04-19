
/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 4  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, LOCKED, POUND , UNLOCKED} state;
unsigned char tmpA;
unsigned char status;
unsigned char previous;

void Tick(){

  switch(state){
    case START:
      state = LOCKED;
      status = 0x00;
      previous = 0x00;
      break;
    case LOCKED:
      tmpA = PINA;
      if(tmpA == 0x04){
        state = POUND;
        previous = tmpA;
      }
      else{
        state = LOCKED;
        previous = 0x00;
      }
      break;

    case POUND:
      tmpA = PINA;
      if(tmpA == 0x02 && previous == 0x04){
        state = UNLOCKED;

      }
      else{
        state = LOCKED;
      }
      break;

    case UNLOCKED:
      if (tmpA == 0x80){
        state = LOCKED;
      }
      else {
        state = UNLOCKED;
      }
      break;

    default:
      state = START;
      break;
  }
  switch (state){
    case LOCKED:
      PORTC = status;
      break;

    case POUND:
      PORTC = status;
      break;

    case UNLOCKED:
      status = 0x01;
      PORTC = status;
      break;

    default:
      break;
  }
}

int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRC = 0xFF; PORTC = 0x00;

  tmpA = PINA;
  status = 0x00;
  previous = 0x00;
  state = START;

  while (1) { Tick(); }
  return 1;
}
