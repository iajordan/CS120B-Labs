/*	Author: Ivannvoi Jordan - ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	VIDEO LINK: https://drive.google.com/open?id=1U0sFBlEpy7HCSQCq04KpbiIV4LuyFhoC
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States { START, INIT, LED1, LED2, LED3, WAIT, DEC, INC} state;
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpA;
unsigned char tmpB;

unsigned char press;
unsigned char press2;

unsigned char stay;
unsigned char direction;
unsigned char count;


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
		count = 0x05;
		break;

		case INIT:
    press = 0x00;
    press2 = 0x00;
    stay = 0x00;
		state = LED1;
    direction = 0x00;
		count = 0x05;
		break;

		case LED1:
    press = ~PINA &0x01;
		if(!press && press2){
			press2 = 0x00;
			stay = 0x00;
		}
    if(press ){
      stay = 0x01;
    }
		if ((press ||stay) && !press2){
			state = DEC;
		}
		else {
			state = LED2;
		}
		break;

    case LED2:
    press = ~PINA &0x01;
		if(!press && press2){
			press2 = 0x00;
			stay = 0x00;
		}
    if(press){
      stay = 0x01;
    }

    if ((press ||stay) && !press2){
      state = INC;
    }
		else{
			if (direction == 0x00){
	      state = LED3;
	    }
	    else if(direction == 0x01){
	      state = LED1;
	    }
			direction = ~direction & 0x01;
		}
    break;

    case LED3:
    press = ~PINA &0x01;
		if(!press && press2){
			press2 = 0x00;
			stay = 0x00;
		}
    if(press ){
      stay = 0x01;
    }

    if ((press ||stay) && !press2){
      state = DEC;
    }
		else{
			state = LED2;
		}
    break;


		case DEC:
		tmpA = ~PINA & 0x03;
		state = WAIT;
		break;

		case INC:
		tmpA = ~PINA & 0x03;
		state = WAIT;
		break;

    case WAIT:
    press = ~PINA &0x01;
    if (stay && !press){
      stay = 0x00;
    }
    else if(press && !stay){
			if (count >= 9){
				state = INIT;
				count = 0x05;
			}
			else{
				state = LED1;
				press2 = 0x01;
	      direction = 0x00;
			}

    }
    else{
      state = WAIT;
    }
		break;


		default:
		state = START;
		break;

	}

  switch(state){
		case INIT:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(count + '0');
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

		case DEC:
		if (count > 0){
			count--;
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
		}
		break;

		case INC:
		if (count < 8 ){
			count++;
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
		}
		else {
			count++;
			LCD_ClearScreen();
			LCD_DisplayString(1, "WINNER!!!");
		}
		break;

    case WAIT:
		break;

		default:
		break;

	}

}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

  TimerSet(300);
  TimerOn();

	LCD_init();
	state = START;
  while (1) {
    TickSM();
    while(!TimerFlag);
    TimerFlag = 0;
  }
	return 0;
}
