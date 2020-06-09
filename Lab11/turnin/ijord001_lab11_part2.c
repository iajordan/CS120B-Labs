/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 11  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Video Link: https://drive.google.com/open?id=1Pb-YJ2CNPLM3bukHItwKzxIIE3DTk41d
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



const unsigned char message[37] = {'C', 'S', '1', '2', '0', 'B', ' ', 'i', 's',' ', 'L', 'e','g', 'e', 'n', 'd',
'.', '.','.', ' ', 'w', 'a','i','t', ' ', 'f', 'o', 'r', ' ', 'i', 't' ,' ', 'D', 'A', 'R', 'Y', '!'};

const unsigned char spaces[16] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ', ' ', ' ' , ' ', '.'};

unsigned char messageCount = 1;
unsigned char spacesCount = 1;

enum LCDTick_States {initMessage,print_spaces, printMessage};

int LCDTick(int state){

  switch(state){

  }
  switch(state){
    case initMessage:
    LCD_ClearScreen();
    state = print_spaces;
    break;

    case print_spaces:
    for (int i = 1; i <= 16; i++){
      LCD_Cursor(i);
      if (i >= 16 - spacesCount){
        LCD_WriteData(message[spacesCount + i - 16]);
      }
      else{
        LCD_WriteData(spaces[spacesCount + i - 2]);
      }
    }
    if (spacesCount >= 15){
      state = printMessage;
      spacesCount = 1;
    }

    spacesCount++;
    break;

    case printMessage:
    for (int i = 1; i <= 16; i++){
      LCD_Cursor(i);
      LCD_WriteData(message[messageCount + i - 2]);

    }
    if(messageCount >= 35 ){
      messageCount = 1;
      state = print_spaces;
    }
    messageCount++;
    break;
    default:
    state = initMessage;
    break;
  }
  return state;
}

int main(){
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  DDRC = 0xF0; PORTC = 0x0F;
  DDRD = 0xFF; PORTD = 0x00;

  static struct _task task1;
  struct _task *tasks[] = {&task1};
  const unsigned short numTasks = sizeof(tasks) /sizeof(task*);

  const char start = -1;
  task1.state = start;
  task1.period = 200;
  task1.elapsedTime = task1.period;
  task1.TickFct = &LCDTick;


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
