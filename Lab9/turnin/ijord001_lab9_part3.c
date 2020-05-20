/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 9  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  VIDEO LINK: https://drive.google.com/open?id=1ckLZVNShFXsm03E1Aiq0-NrhOlWwPeKr
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


unsigned char tmpB;

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


void set_PWM(double frequency){
  static double current_frequency;
  if(frequency != current_frequency){
    if(!frequency){
      TCCR3B &= 0x08;

    }
    else{
      TCCR3B |= 0x03;
    }

    if(frequency < 0.954){
      OCR3A = 0xFFFF;
    }
    else if (frequency > 31250){
      OCR3A = 0x0000;
    }
    else{
      OCR3A = (short) ( 8000000 / (128 * frequency)) - 1;
    }
    TCNT3 = 0;
    current_frequency = frequency;
  }
}

void PWM_on(){
  TCCR3A = (1 << COM3A0);
  TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
  set_PWM(0);
}

void PWM_off(){
  TCCR3A = 0x00;
  TCCR3B = 0x00;
}

enum States { START, INIT, PLAY, WAIT} state;

const  double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

const int song[20] = {0,0,1,2,3,4,5,6,  5,5,6,7, 7,7,7,  6,5,6,  4,3};

const int time[20] = {10,15,5,2,2,2,2,25,   4,3,2,15, 2,2,4,  2,2,5,  3,20};
                          // c4    d4       e4      f4      g4     a4      b4        c5
                          // 0     1        2       3        4      5       6         7

unsigned char tmpA;
unsigned char current_note;
unsigned char key;
unsigned char t;
unsigned char i;


void TickSM(){
	switch(state){
		case START:
		state = INIT;
    current_note = 0x00;
    key = 0x00;
    t = 0x00;
    i = 0x00;
		break;

		case INIT:
    tmpA = ~PINA & 0x01;

    if (tmpA){
      PWM_on();
      state = PLAY;

    }
    else{
      PWM_off();
      state = INIT;
    }

		break;

    case PLAY:
    if (key > 19){
      state = WAIT;
      key = 0;
      current_note = 0;
       t= 0;
      PWM_off();
    }
    else {
      unsigned int  count = time[t];
      PWM_on();
      state = PLAY;
      if (i < count){
        set_PWM(notes[song[key]]);
        i++;
      }
      else{
        key++;
        t++;
        i = 0;
      }
    }
    break;

    case WAIT:
    tmpA = ~PINA & 0x01;
    if (!tmpA){
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
    DDRB = 0xFF; PORTB = 0x00;
    state = START;
    TimerSet(50);
    TimerOn();
    PWM_off();

    while (1) {
      TickSM();
      while(!TimerFlag);
      TimerFlag = 0;

    }
    return 1;
}
