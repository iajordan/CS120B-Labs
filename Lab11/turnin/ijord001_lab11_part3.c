/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 11  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Video Link: https://drive.google.com/open?id=1qUhQX_nyH1MdrxJv4GnwYntb4SgtQO08
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
#endif

unsigned char keyOutput;

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
    if (x != '\0'){
      keyOutput = x;
    }
    break;
  }
  return state;
}

enum LCDTick_States {init2, wait2};

int LCDout(int state){

  switch(state){
    case init2:
    state = wait2;
    LCD_ClearScreen();
    if (keyOutput != '\0'){
      state = wait2;
    }
    else{
      state = init2;
    }
    break;
    case wait2:
    LCD_Cursor(1);
    LCD_WriteData(keyOutput);
    break;
    default: state = init; break;
  }
  return state;
}


int main(){
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  DDRC = 0xF0; PORTC = 0x0F;
  DDRD = 0xFF; PORTD = 0x00;

  static struct _task task1, task2;
  struct _task *tasks[] = {&task1, &task2};
  const unsigned short numTasks = sizeof(tasks) /sizeof(task*);

  const char start = -1;

  task1.state = start;
  task1.period = 10;
  task1.elapsedTime = task1.period;
  task1.TickFct = &keypadPress;

  task2.state = start;
  task2.period = 10;
  task2.elapsedTime = task2.period;
  task2.TickFct = &LCDout;

  unsigned long GCD = tasks[0]->period;

  unsigned short i;
  for (i = 0; i < numTasks; i++){
    GCD = findGCD(GCD, tasks[i]->period);
  }

  TimerSet(GCD);
  TimerOn();
  LCD_init();



  while(1){


    for(i = 0; i< numTasks; i++){

      if(tasks[i] -> elapsedTime == tasks[i]->period){
        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
        tasks[i]->elapsedTime = 0;
      }
      tasks[i]->elapsedTime += 1;

    }
    while(!TimerFlag);
    TimerFlag = 0;
  }
  return 0;
}
