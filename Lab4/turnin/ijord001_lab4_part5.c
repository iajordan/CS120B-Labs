
/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 4  Exercise 5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, LOCKED, CHECK, UNLOCKED} state;
unsigned char tmpA;
unsigned char status;
unsigned char previous;
unsigned char count;
unsigned char code_in[10] = {};
unsigned char code[10] = {0x04, 0x01, 0x02, 0x01};

void Tick(){

  switch(state){
    case START:
      state = LOCKED;
      previous = 0x00;
      count = 0x00;
      break;

    case LOCKED:
      tmpA = PINA;
      if(tmpA != 0){
        code_in[count] = tmpA;
        count += 1;
      }
      if (count == 0x03){
        state = CHECK;
      }
      else{
        state = LOCKED;
      }
      break;

    case CHECK:
      for (int i = 0; i < 4; i++){
        if(code[i] == code_in[i]){
          state = UNLOCKED;
        }
        else {
          state = LOCKED;
        }
      }
      break;

    case UNLOCKED:
      tmpA = PINA;
      state = UNLOCKED;
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

    case CHECK:
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
