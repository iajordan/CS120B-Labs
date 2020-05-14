/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  VIDEO LINK: https://drive.google.com/open?id=16Rea5G09bS0jFTSo2562Vt3gfTx6W1dl
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { START, INIT, INC , DEC, CLEAR} state;

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpA;
unsigned char tmpB;

unsigned char count;
unsigned char press;

unsigned i;


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
		press = 0x00;
		i = 0x00;
    break;

    case INIT:

		tmpA = ~PINA & 0x03;
    if (tmpA == 0x01){
      state = INC;
    }
    else if (tmpA == 0x02){
			state = DEC;
    }
    else if (tmpA == 0x03){
      state = CLEAR;
    }
    else{
			state = INIT;
		}
		PORTB = count;
    break;

		case DEC:
		tmpA = ~PINA & 0x03;
		if (tmpA){
			press = 0x01;
		}
		if (press && tmpA){
			state = DEC;
			if (count > 7 && i > 10){
				count--;
				i = 0x00;
			}
			i ++;
		}
		else{
			if (count > 0){
				count--;
			}
			state = INIT;
		}

		PORTB = count;
		break;

		case INC:
		tmpA = ~PINA & 0x03;
		if (tmpA){
			press = 0x01;
		}
		if (press && tmpA){
			state = INC;
			if (count < 9 && i > 10){
				count++;
				i = 0x00;
			}
			i++;
		}
		else{
			if (count < 9){
				count++;
			}
			state = INIT;
		}
		POERB = count;
		break;

		case CLEAR:
		state = INIT;
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

	LCD_init();

  while (1) {
    TickSM();
    while(!TimerFlag){}
    TimerFlag = 0;
  }
}
