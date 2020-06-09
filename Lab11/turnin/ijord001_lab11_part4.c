/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 11  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *
 *  Video Link: https://drive.google.com/open?id=1JZ3wALTsGvGgsc95Um7GJm9j6El90E5s
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
unsigned char pressButton;
unsigned char releaseButton;
unsigned char cursor = 0;

enum keypadPress_States {KEYinit, press , release};

int keypadPress(int state){
  switch(state){
    case KEYinit:
    state = release;
    break;

    case press:
    if (GetKeypadKey() != '\0'){
      keyOutput = GetKeypadKey();
      state = release;
      pressButton = 0x01;
    }
    break;

    case release:
    pressButton = 0x00;
    if(GetKeypadKey() == '\0'){
      releaseButton = 0x01;
    }
    if(GetKeypadKey() != '\0' && releaseButton){
      state = press;
      releaseButton = 0x00;
      cursor ++;
    }
    break;

    default:
    state = KEYinit;
    break;
  }
  return state;
}

enum LCDTick_States {LCDinit,null, output};

int LCDout(int state){
  switch(state){
    case LCDinit:
    LCD_DisplayString(1,"CONGRATULATIONS!");
    state = null;
    break;

    case null:
    if (keyOutput != '\0'){
      state = output;
    }
    break;

    case output:
    LCD_Cursor(cursor);
    LCD_WriteData(keyOutput);
    if (cursor == 16){
      cursor = 1;
    }

    break;
    default: state = LCDinit; break;
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
  task1.period = 50;
  task1.elapsedTime = task1.period;
  task1.TickFct = &keypadPress;

  task2.state = start;
  task2.period = 50;
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
      tasks[i]->elapsedTime += GCD;

    }
    while(!TimerFlag);
    TimerFlag = 0;
  }
  return 0;
}
