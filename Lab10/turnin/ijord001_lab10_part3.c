/*	Author: Ivannvoi Jordan  -  ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 10  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Video Link: https://drive.google.com/open?id=1iv_wC5oMrZrWuuOt8fPHu-F8G_NK8JiK
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include <avr/interrupt.h>
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

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


enum T_States { START_T, INIT_T} t_state;

enum B_States { START_B, INIT_B} b_state;

enum C_States { START_C, INIT_C} c_state;

enum S_States { START_S, INIT_S, ON, OFF} s_state;

const unsigned short timerPeriod = 1;

unsigned char threeLEDs = 0x01;
unsigned char blinkingLED = 0x01;

unsigned char sound;
unsigned char tmpA;


void ThreeLEDSM(){
  switch(t_state){
    case START_T:
    t_state = INIT_T;
    break;

    case INIT_T:
    if (threeLEDs == 0x4){
      threeLEDs = 0x01;
    }
    else{
      threeLEDs = threeLEDs << 1;
    }
    break;

    default:
    t_state = START_T;
    break;

  }
}

void BlinkingLEDSM(){
 	switch(b_state){
 		case START_B:
 		b_state = INIT_B;
 		break;

    case INIT_B:
    blinkingLED = ~blinkingLED;
    break;

 		default:
 		b_state = START_B;
 		break;

 	}
}

void SoundSM(){
  switch(s_state){
    case START_S:
    s_state = INIT_S;
    sound = 0x00;
    tmpA = 0x00;
    break;

    case INIT_S:
    tmpA = ~PINA;
    if (tmpA){
      s_state = ON;
    }
    break;

    case ON:
    tmpA = ~PINA;
    if(tmpA){
      sound = ~sound & 0x01;
      s_state = ON;
    }
    else{
      s_state = INIT_S;
    }
    break;

    case OFF:
    tmpA = ~PINA;
    if (tmpA){
      sound = 0x00;
      s_state = OFF;
    }
    else{
      s_state = INIT_S;
    }
    break;

    default:
    s_state = START_S;
    break;
  }
}

void CombineLEDsSM(){
 	switch(c_state){
 		case START_C:
 		c_state = INIT_C;
 		break;

    case INIT_C:
    PORTB = ((sound &0x01) << 4) | ((blinkingLED & 0x01) << 3) | threeLEDs;
    break;

 		default:
 		c_state = START_C;
 		break;

 	}
}



int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  unsigned long BL_elapsedTime = 0;
  unsigned long TL_elapsedTime = 0;
  unsigned long S_elapsedTime = 0;


  TimerSet(1);
  TimerOn();
  t_state = START_T;
  b_state = START_B;
  c_state = START_C;
  s_state = START_S;

  BlinkingLEDSM();
  ThreeLEDSM();
  SoundSM();

  while (1) {
    if (BL_elapsedTime >= 1000){
      BlinkingLEDSM();
      BL_elapsedTime = 0;
    }
    if (TL_elapsedTime >= 300){
      ThreeLEDSM();
      TL_elapsedTime = 0;
    }
    if (S_elapsedTime >= 2){
      SoundSM();
      S_elapsedTime = 0;
    }
    CombineLEDsSM();

    BL_elapsedTime += timerPeriod;
    TL_elapsedTime += timerPeriod;
    S_elapsedTime += timerPeriod;

    while(!TimerFlag);
    TimerFlag = 0;

  }
  return 1;
}
