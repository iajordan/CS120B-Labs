/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States { START, INIT, NEXT} state;
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;


unsigned char tmpB;
// unsigned char lights;

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
		break;

		case INIT:
		state = NEXT;
		break;

		case NEXT:
		state = NEXT;
		break;

		default:
		state = START;
		break;

	}

  switch(state){
		case START:
		tmpB = 0x01;
    PORTB = tmpB;
		break;

		case INIT:
    tmpB = 0x01;
    PORTB = tmpB;
		break;

		case NEXT:
		tmpB = (tmpB == 0x04)? 0x01 : tmpB << 1;
    PORTB = tmpB;
		break;

		default:
		break;

	}

}


int main(void) {
  DDRB = 0xFF; PORTB = 0x00;
  state = START;

  TimerSet(1000);
  TimerOn();

  while (1) {
    TickSM();
    while(!TimerFlag){}
    TimerFlag = 0;
  }
}
