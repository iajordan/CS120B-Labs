
/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 4  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, INIT, INC , DEC} state;
unsigned char tmpA;
unsigned char count;

void Tick(){

  switch(state){
    case START:
      state = INIT;
      count =0x07;
      break;
    case INIT:
      tmpA = PINA;
      if(tmpA == 0x01){
        state = INC;
      }
      else if (tmpA == 0x02){
        state = DEC;
      }
      else if (tmpA == 0x03){
        count = 0x00;
        state = INIT;
      }
      else{
        state = INIT;
      }
      break;
    case INC:
      state = INIT;
      if (count < 9){
        count++;
      }
      break;

    case DEC:
      state = INIT;
      if (count > 0){
        count--;
      }
      break;

    default:
      state = START;
      break;
  }
  switch (state){
    case INIT:
      PORTC = count;
      break;

    case INC:
      PORTC = count;
      break;

    case DEC:

      PORTC = count;
      break;

    default:
      break;
  }
}

int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRC = 0xFF; PORTC = 0x00;

  tmpA = PINA;
  count = 0x07;
  state = START;
  while (1) { Tick(); }
  return 1;
}
