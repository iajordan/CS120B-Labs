/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 6  Exercise 2
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
enum States { START, INIT, LED1, LED2, LED3, WAIT} state;
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpA;
unsigned char tmpB;
unsigned char press;
unsigned char press2;
unsigned char stay;
unsigned char direction;

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
    direction = 0x00;
		break;

		case INIT:
    tmpA = ~PINA & 0x01;
    press = 0x00;
    press2 = 0x00;
    stay = 0x00;
		state = LED1;
    direction = 0x00;
		break;

		case LED1:
    press = ~PINA &0x01;
    if(press){
      stay = 0x01;
    }
    if (press || stay){
      state = WAIT;
    }
    else {
      state = LED2;
    }
		break;

    case LED2:
    press = ~PINA &0x01;
    if(press){
      stay = 0x01;
    }
    if (press || stay){
      state = WAIT;
    }
    else if (direction == 0x00){
      state = LED3;
    }
    else {
      state = LED1;
    }
    direction = ~direction & 0x01;
    break;


    case LED3:
    press = ~PINA &0x01;
    if(press){
      stay = 0x01;
    }
    if (press || stay){
      state = WAIT;
    }
    else{
      state = LED2;
    }
    break;

    case WAIT:
    press = ~PINA &0x01;
    if (stay && !press){
      stay = 0x00;
    }
    else if(press && !stay){
      state = INIT;
      direction = 0x00;
    }
    else{
      state = WAIT;
    }

    // if(~PINA & 0x01){
    //   press2 = 0x01;
    // }
    // if (press2){
    //   state = INIT;
    //   direction = 0x00;
    // }
    // else{
    //   state = WAIT;
    // }
		break;

		default:
		state = START;
		break;

	}

  switch(state){
    case START:
		state = INIT;
    direction = 0x00;
		break;

		case INIT:
    tmpB = 0x01;
    PORTB = tmpB;
		state = LED1;
		break;

		case LED1:
		tmpB = 0x01;
    PORTB = tmpB;
		break;


    case LED2:
    tmpB = 0x02;
    PORTB = tmpB;
    break;

    case LED3:
		tmpB = 0x04;
    PORTB = tmpB;
    break;

    case WAIT:
    // tmpB = 0x01;
    // PORTB = tmpB;
		break;

		default:
		break;

	}

}


int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  state = START;
  tmpA = ~PINA;

  TimerSet(50);
  TimerOn();

  while (1) {
    TickSM();
    while(!TimerFlag){}
    TimerFlag = 0;
  }
}
