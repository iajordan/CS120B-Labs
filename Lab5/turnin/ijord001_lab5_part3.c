
/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * VIDEO LINK: https://drive.google.com/open?id=1qYFDoIj77jWTyFb5qLRqJ2WMk4Ky344d
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, INIT, CBOTH, CMID, CFLASH,ZERO , WAIT} state;
unsigned char tmpA;
unsigned char lights;
unsigned char flag;
unsigned char temp;
unsigned char count;


void Tick(){
  switch(state){
    case START:
      state = INIT;
      lights =0x00;
      flag = 0x00;
      temp = 0x00;
      count = 0x00;
      break;
    case INIT:
      tmpA = ~PINA & 0x01;
      if (lights == 0x3F){
        state = ZERO;
      }
      else if (tmpA && (flag == 0x00)){
        state = CBOTH;
      }
      else if (tmpA && (flag == 0x01)){
        state = CMID;
      }
      else if (tmpA && (flag == 0x02)){
        state = CFLASH;
      }
      else{
        state = INIT;
      }
      break;

    case CBOTH:
      state = WAIT;
      temp = temp << 1;
      temp = temp | 0x01;

      lights = lights >> 1;
      lights = lights | 0x20;
      lights = temp | lights;
      PORTC  = lights;
      break;

    case CMID:
      state = WAIT;
      temp = temp >> 1;
      temp = temp | 0x04;

      lights = lights << 1;
      lights = lights | 0x08;
      lights = temp | lights;
      PORTC  = lights;
      break;

    case CFLASH:
      state = WAIT;
      if (count == 0){
        lights = 0x2A;
        count++;
      }
      else {
        lights = ~lights & 0x3F;
        count++;
      }
      if (count == 8){
        lights = 0x3F;
        count = 0;
      }
      PORTC  = lights;
      break;

    case ZERO:
      tmpA = ~PINA;

      if (tmpA){
        lights = 0x00;
        temp = 0x00;
        state = WAIT;
        flag += 1;
        if (flag > 2){
          flag = 0x00;
        }
      }
      else {
        state = ZERO;
      }
      PORTC = lights;
      break;

    case WAIT:
      tmpA = ~PINA;

      if (!tmpA){
        state = INIT;
      }
      else {
        state = WAIT;
      }
      PORTC = lights;
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
  lights = 0x00;
  temp = 0x00;
  count = 0x00;
  state = START;
  while (1) { Tick(); }
  return 1;
}
