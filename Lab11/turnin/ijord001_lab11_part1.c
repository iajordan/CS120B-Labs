/*	Author: Ivannvoi Jordan - ijord001@urc.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 11  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Video Link:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "io.h"
#include "scheduler.h"
#include "keypad.h"
#endifcp

enum keypadPress_States {init, wait};

int keypadPress(int state){
  unsigned char x;

  switch(state){
    case init:
    state = wait; break;
    case wait:
    state = wait; break;
    default: state = init; break;
  }
  switch(state){
    case init: break;
    case wait:
    x = GetKeypadKey();
    switch (x){
      case '\0': PORTB = 0x1F; break;
      case '1' : PORTB = 0x01; break;
      case '2' : PORTB = 0x02; break;
      case '3' : PORTB = 0x03; break;
      case '4' : PORTB = 0x04; break;
      case '5' : PORTB = 0x05; break;
      case '6' : PORTB = 0x06; break;
      case '7' : PORTB = 0x07; break;
      case '8' : PORTB = 0x08; break;
      case '9' : PORTB = 0x09; break;
      case 'A' : PORTB = 0x0A; break;
      case 'B' : PORTB = 0x0B; break;
      case 'C' : PORTB = 0x0C; break;
      case 'D' : PORTB = 0x0D; break;
      case '*' : PORTB = 0x0E; break;
      case '0' : PORTB = 0x00; break;
      case '#' : PORTB = 0x0F; break;
      default: PORTB = 0x1B; break;
    }
    break;
  }
  return state;
}


int main(){
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  DDRC = 0xF0; PORTC = 0x0F;

  static struct _task task1;
  struct _task *tasks[] = {&task1};
  const unsigned short numTasks = sizeof(tasks) /sizeof(task*);

  const char start = -1;

  task1.state = start;
  task1.period = 100;
  task1.elapsedTime = task1.period;
  task1.TickFct = &keypadPress;
  unsigned long GCD = tasks[0]->period;
  unsigned short i;
  for (i = 0; i < numTasks; i++){
    GCD = findGCD(GCD, tasks[i]->period);
  }

  TimerSet(GCD);
  TimerOn();

  while(1){
    for(i = 0; i< numTasks; i++){

      if(tasks[i] -> elapsedTime == tasks[i]->period){
        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
        tasks[i]->elapsedTime = 0;
      }
      tasks[i]->elapsedTime += GCD;

    }
    while(!TimerFlag);
    TimerFlag = 0;
  }
  return 0;
}
