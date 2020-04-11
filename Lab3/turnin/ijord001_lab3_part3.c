/*	Author: Ivannovi Jordan ijord001@ucr.edu
 *  Partner(s) Name:
 *	Lab Section: 28
 *	Assignment: Lab 3  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 #include <avr/io.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #endif
 int main(void){

 DDRA = 0x00; PORTA = 0xFF;
 DDRC = 0xFF; PORTC = 0x00;

 unsigned char tmpA;
 unsigned char gague;
 unsigned char lowFuel;
 unsigned char fasten;

 while(1){
     tmpA = PINA & 0x0F;
     fasten = (((PINA & 0x70) == 0x30) << 7);
     if (tmpA >= 0x0D){
       gague = 0x3F;
       lowFuel = 0x00;
     }
     else if(tmpA >= 0x0A){
       gague = 0x3E;
       lowFuel = 0x00;
     }
     else if (tmpA >= 0x07){
       gague = 0x3C;
       lowFuel = 0x00;
     }
     else if (tmpA >= 0x05){
       gague = 0x38;
       lowFuel = 0x00;
     }
     else if (tmpA >= 0x03){
       gague = 0x30;
       lowFuel = 0x40;

     }
     else{
       gague = 0x20;
       lowFuel = 0x40;
     }
     PORTC = (gague & 0x3F) | lowFuel | fasten;
   }
     return 0;
 }
