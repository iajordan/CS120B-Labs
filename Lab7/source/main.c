/*	Author: sp4
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, INIT, INC , DEC, CLEAR, WAIT} state;

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpA;
unsigned char tmpB;

unsigned char count = 0x00;
unsigned char once;
unsigned char type;



void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
  TCCR1B = 0x00;
}

void TimerISR(){
  TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}


void TickSM(){
  switch(state){
    case START:
      state = INIT;
      count =0x00;
      once = 0x00;
      LCD_ClearScreen();
      break;
    case INIT:
      tmpA = ~PINA & 0x03;
      LCD_Cursor(1);
      if (tmpA == 0x01){
        type = 0x01;
      }
      else if (tmpA == 0x02){
        type = 0x02;
      }
      else if (tmpA == 0x03){
        type = 0x03;
      }
      if(type == 0x01){
        tmpA = ~PINA;
        if (count < 9 && once){
          count++;
          once = 0x00;
        }
        else if (count < 9 && (tmpA == 0x01 )){
          count++;
        }
        else{
          state = INIT;
        }
      }
      else if (type == 0x02){
        tmpA = ~PINA;
        if (count > 0 && once){
          count --;
          once = 0x00;
        }
        else if (count > 0 && (tmpA == 0x02 )){
          count--;
        }
        else{
          state = INIT;
        }
      }
      else if (type == 0x03){
        tmpA = ~PINA;
        if (tmpA == 0x03){
          count = 0x00;
        }

        state =  INIT;
      }
      else{
        state = INIT;
      }
      LCD_WriteData(count + '0');
      break;

    default:
      state = START;
      break;
  }
}
//
// int main(void) {
//   DDRC = 0xFF; PORTC = 0x00;
//   DDRD = 0xFF; PORTD = 0x00;
//
//   LCD_init();
//   LCD_DisplayString(1,"Hello World");
//   while (1) { continue; }
//   return 1;
// }

int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRC = 0xFF; PORTC = 0x00;
  DDRD = 0xFF; PORTD = 0x00;

  state = START;
  tmpA = ~PINA;
  count = 0x00;

  TimerSet(50);
  TimerOn();

  LCD_init();


  while (1) {
    TickSM();
    while(!TimerFlag){}
    TimerFlag = 0;
  }
}
