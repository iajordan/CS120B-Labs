/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  VIDEO LINK: https://drive.google.com/open?id=16Dv3UwgbDAbhyaOv2MAZzzTB491OGsyu
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

enum States { START, INIT, C4, D4, E4, OFF} state;

unsigned char tmpA;
unsigned char tmpB;


void TickSM(){
	switch(state){
		case START:
		state = INIT;
		break;

		case INIT:
    tmpA = ~PINA & 0x07;
    if (tmpA == 0x01){
      state = C4;
    }
    else if (tmpA == 0x02){
      state = D4;
    }
    else if (tmpA == 0x04){
      state = E4;
    }
    else{
      state = INIT;
    }
		break;

		case C4:
    tmpA = ~PINA & 0x07;
    if (tmpA == 0x01){
      state = C4;
    }
    else{
      state = OFF;
    }

		break;

    case D4:
    tmpA = ~PINA & 0x07;
    if (tmpA == 0x02){
      state = D4;
    }
    else{
      state = OFF;
    }

    break;

    case E4:
    tmpA = ~PINA & 0x07;
    if (tmpA == 0x04){
      state = E4;
    }
    else{
      state = OFF;
    }

    break;

    case OFF:
    state = INIT;
    break;

		default:
		state = START;
		break;

	}

  switch(state){

		case INIT:

    tmpB = 0x00;
    PORTB = tmpB;
		break;

		case C4:
    PWM_on();
    set_PWM(261.63);
    tmpB = 0x01;
    PORTB = tmpB;
		break;

    case D4:
    PWM_on();
    set_PWM(293.66);
    tmpB = 0x02;
    PORTB = tmpB;
    break;

    case E4:
    PWM_on();
    set_PWM(329.63);
    tmpB = 0x04;
    PORTB = tmpB;
    break;

    case OFF:
    PWM_off();
    break;

		default:
		break;

	}

}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    state = START;
    TimerSet(100);
    TimerOn();
    PWM_off();

    while (1) {
      TickSM();
      while(!TimerFlag);
      TimerFlag = 0;

    }
    return 1;
}
