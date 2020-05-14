/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  VIDEO LINK DEMO: https://drive.google.com/open?id=1k4GwVifKcpqoTGHjV3_lrfDguK1qTvoB
 *	VIDEO LINK AUTO GRADER: https://drive.google.com/open?id=11Bv_Rc4j12P__-m3Z6B8qtZMmERqcvmW
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States { START, INIT, INC , DEC, CLEAR, DEC1, INC1} state;

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpA;
unsigned char tmpB;

unsigned char count;
unsigned char press;

unsigned char i;


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
    count =0x07;
		press = 0x00;
		i = 0;
    PORTB = count;
    break;

    case INIT:
		tmpA = ~PINA & 0x03;
    if (tmpA == 0x01){
      state = INC;
    }
    else if (tmpA == 0x02){
			if (count >= 7){
				state =DEC;
			}
			else{
				state = DEC1;
			}
    }
    else if (tmpA == 0x03){
      state = CLEAR;
    }
    else{
			state = INIT;
		}
    break;

		case DEC:
		tmpA = ~PINA & 0x03;
		if (tmpA){
			press = 0x01;
		}
		if (i > 10){
			if (press && tmpA && count > 7){
				state = DEC;
			}
		}

		if (press && !tmpA){
			state = DEC1;
		}
		else{
			state = INIT;
		}
		break;


		case DEC1:
		state = INIT;
		break;


		case INC:
		tmpA = ~PINA & 0x03;
		if (tmpA){
			press = 0x01;
		}
		if (i > 10){
			if (press && tmpA && count  < 9){
				state = INC;
			}
			i++;
		}

		if (press && !tmpA){
			state = INC1;
		}
		else{
			state = INIT;
		}
		break;

		case INC1:
		state = INIT;
		break;

		case CLEAR:
		state = INIT;
		break;

    default:
    state = START;
    break;
  }

	switch(state){
    case INIT:
		PORTB = count;
		break;

		case DEC:
		if (count > 7){
			count--;
		}
		PORTB = count;
		break;

		case DEC1:
		if (count > 0){
			count--;
		}
		press = 0x00;
		PORTB = count;
		break;

		case INC1:
		if (count < 9){
			count++;
		}
		press = 0x00;
		PORTB = count;
		break;

		case INC:
		if (count < 9 ){
			count++;
		}
		PORTB = count;
		break;

		case CLEAR:
		count = 0;
		PORTB = count;
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
  tmpA = ~PINA;
  count = 0x07;

  TimerSet(100);
  TimerOn();

  while (1) {
    TickSM();
    while(!TimerFlag){}
    TimerFlag = 0;
  }
}
