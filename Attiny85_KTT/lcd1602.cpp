// xiaolaba
// uses LCD1602 and code
// include the library code:

#include "lcd1602.h"

#include <LiquidCrystal.h>

#include <arduino.h>



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 1, en = 2, d4 = 3, d5 = 4, d6 = 5, d7 = 6; // dummy test for code compiling only, no actual target or test
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void useLCD1602()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hi KTT xioalaba");
}

void blinkLCD(){
  // Turn off the blinking cursor:
  lcd.noBlink();
  delay(3000);
  // Turn on the blinking cursor:
  lcd.blink();
  delay(3000);
}
