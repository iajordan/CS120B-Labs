
/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 4  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, LOCKED, UNLOCKED} state;
unsigned char tmpA;
unsigned char status;
unsigned char previous;

void Tick(){

  switch(state){
    case START:
      state = LOCKED;
      previous = 0x00;
      break;

    case LOCKED:
      tmpA = PINA;
      if(tmpA == 0x04){
        previous = tmpA;
      }
      if(tmpA == 0x02 && previous == 0x04){
        state = UNLOCKED;
      }
      else{
        state = LOCKED;
      }
      break;

    case UNLOCKED:

      tmpA = PINA;
      if (tmpA == 0x04){
        status = 0x01;
        previous = tmpA;
      }
      if(tmpA == 0x02 && previous == 0x04){
        state = LOCKED;
      }
      else{
        state = UNLOCKED;
      }
      break;

    default:
      state = START;
      break;
  }


  switch (state){
    case START:
      status = 0x00;
      break;
    case LOCKED:
      status = 0x00;
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
