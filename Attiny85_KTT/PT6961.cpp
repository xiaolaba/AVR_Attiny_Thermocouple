/*
  PT6961.cpp - Library for communicating with PT6961 LED driver.
  Created by Garrett Blanton January, 16, 2013.
  Released into the public domain.
*/

// mod, xiaolaba
// 7-seg, seg name, this LED module, no dot designed
//0 - A
//1 - B
//2 - C
//3 - D
//4 - E
//5 - F
//6 - G
//7 - Colon

#include "Arduino.h"
#include "PT6961.h"





#define PORT PORTB

#define CS_LO  PORT &= ~(1<<_CS);
#define CS_HI  PORT |= (1<<_CS);


PT6961::PT6961(int DIN, int CLK, int CS)
{
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  _DIN = DIN;
  _CLK = CLK;
  _CS = CS;
}

void PT6961::initDisplay()
{
  sendCmd(_DISPLAY_6X12);
  sendCmd(_AUTO_INCREMENT);
  initRAM();
  //sendCmd(_DISPLAY_14_16);  // LED contrast
  sendCmd(_DISPLAY_2_16);
}

// Initializes RAM to all zeros
void PT6961::initRAM()
{
  //first clear 8 bytes of the display RAM
//  digitalWrite(_CS,LOW);
  CS_LO;
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  for(int i=0; i<8; i++){
    shiftOut(_DIN, _CLK, LSBFIRST, 0x00);
  }
//  digitalWrite(_CS,HIGH); 
  CS_HI;

}

/*
// Use to send command based on enumeration
void PT6961::sendCmd(char cmd)
{
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, cmd);
  digitalWrite(_CS,HIGH);  
}
*/


void PT6961::sendCmd(char cmd)
{
//  digitalWrite(_CS,LOW);
  CS_LO;
  shiftOut(_DIN, _CLK, LSBFIRST, cmd);
//  digitalWrite(_CS,HIGH); 
  CS_HI; 
}



void PT6961::sendDigit(char digit, char val)
{
//  digitalWrite(_CS,LOW);
  CS_LO;
  shiftOut(_DIN, _CLK, LSBFIRST, digit);
  shiftOut(_DIN, _CLK, LSBFIRST, val);
//  digitalWrite(_CS,HIGH); 
  CS_HI;  
}





// mod, xiaolaba
void PT6961::sendNum(int num, char colon)
{
  int digit1 = num / 1000; 
  if (digit1 == 0) {digit1 = 0x11;} // 0xxx, digit1 off, no display 
  
  int digit2 = (num % 1000) / 100; 
  if (digit1 == 0x11 && digit2 == 0 ){digit2 = 0x11;} // 00xx, digit1/2 off, no display, 
  
  int digit3 = (num % 100) / 10; 
  if (digit1 == 0x11 && digit2 == 0x11 && digit3 == 0) {digit3 = 0x11;} //  000x, digit1/2/3 off, no display,
  int digit4 = (num % 10);

  sendDigits(digit1,digit2,digit3,digit4,colon);

}

//xiaoaba
void PT6961::sendDigits(char digit1, char digit2, char digit3, char digit4, char colon)
{
  // 7-seg LED 0-9, A,b,c,d,E,F,upper-o, 
  const char DISP[18] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,  //0-9
                         0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,  //A,b,C,d,E,F
                         0x63, //upper-o
						             0x00}; // index 0x11, // 7-seg off, the digit if off
//  digitalWrite(_CS,LOW);
  CS_LO;
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  
  if(colon == 1)
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit1] | 0x80);
  }
  else
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit1]);
  }

  shiftOut(_DIN, _CLK, LSBFIRST, 0xC2);

  if(colon == 1)  
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit2] | 0x80);
  }
  else
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit2]);
  }
  
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC4);
  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit3]);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC6);
  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit4]);
//  digitalWrite(_CS,HIGH); 
  CS_HI;    
}
