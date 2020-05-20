/*	Author: Ivannovi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab 9  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  VIDEO LINK: https://drive.google.com/open?id=16MmC04ZJYbgOUM2Z5QU87EuSJ_kk10rY
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

enum States { START, INIT, POWER, UP, DOWN, RELEASE} state;

enum P_States { STARTP, INITP, PRESS} p_state;

const  double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};


unsigned char tmpA;
unsigned char tmpB;
unsigned char power;
unsigned char hold;
unsigned char current_note;

void PowerSM(){
  switch(p_state){
    case STARTP:
    p_state = INITP;
    power = 0x00;
    hold = 0x00;
    break;

    case INITP:
    tmpA = ~PINA & 0x01;
    if(tmpA == 0x01){
      p_state = PRESS;
      hold = 0x01;
      power = ~power & 0x01;

    }
    else{
      p_state = INITP;
      tmpB = 0x01;

    }

    PORTB = (power << 1) | tmpB;
    break;

    case PRESS:
    tmpA = ~PINA & 0x01;
    if (tmpA == 0x01 && hold){
      p_state = PRESS;
      tmpB = 0x01;

    }
    else {
      hold = 0x00;
      p_state = INITP;
    }
    PORTB = (power << 1) | tmpB;
    break;

    default:
    p_state = START;
    break;
  }
}

void TickSM(){
	switch(state){
		case START:
		state = INIT;
    current_note = 0x00;
		break;

		case INIT:
    tmpA = ~PINA & 0x06;
    if (tmpA == 0x02){
      state = UP;
    }
    else if (tmpA == 0x04){
      state = DOWN;
    }
    else{
      state = INIT;
    }
    if (!power){
      PWM_off();
      current_note = 0;
    }
    else{
      PWM_on();
      set_PWM(notes[current_note]);
    }

		break;

    case UP:
    state = RELEASE;
    if (current_note < 7){
      current_note++;
    }
    break;

    case DOWN:
    state = RELEASE;
    if (current_note > 0){
      current_note--;
    }
    break;

    case RELEASE:
    tmpA = ~PINA & 0x06;
    if (!tmpA){
      state = INIT;
    }
    else{
      state = RELEASE;
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
    p_state = STARTP;
    state = START;
    TimerSet(50);
    TimerOn();
    PWM_off();

    while (1) {
      TickSM();
      PowerSM();
      while(!TimerFlag);
      TimerFlag = 0;

    }
    return 1;
}
