
/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *
 * VIDEO LINK: https://drive.google.com/open?id=1YlTJQ_jdJFLXvlQf2Wk4gT2CdFYHYp5o
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, INIT, INC , DEC, CLEAR, WAIT} state;
unsigned char tmpA;
unsigned char count = 0x00;

void Tick(){
  switch(state){
    case START:
      state = INIT;
      count =0x00;
      break;
    case INIT:
      tmpA = ~PINA & 0x03;
      if(tmpA == 0x01){
        state = INC;
      }
      else if (tmpA == 0x02){
        state = DEC;
      }
      else if (tmpA == 0x03){
        state = CLEAR;
      }
      else{
        state = INIT;
      }
      break;
    case INC:
      state = WAIT;
      if (count < 9){
        count++;
      }
      PORTC = count;
      break;

    case DEC:
      state = WAIT;
      if (count > 0){
        count--;

      }
      PORTC = count;
      break;

    case CLEAR:
      tmpA = ~PINA;
      if (tmpA == 0x03){
        count = 0x00;
      }
      PORTC = count;
      state =  WAIT;
      break;

    case WAIT:
      tmpA = ~PINA;
      if(tmpA == 0x03){
        state = CLEAR;
      }
      else if (!tmpA){
        state = INIT;
      }
      else{
        state = WAIT;
      }
      break;

    default:
      state = START;
      break;
  }
}

int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRC = 0xFF; PORTC = 0x00;

  tmpA = ~PINA;
  count = 0x00;
  state = START;
  while (1) { Tick(); }
  return 1;
}
