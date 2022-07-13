/* 
 *  ATtiny85_K_Type_Thermocouple.ino
 *  Ttiny85_KTT.ino
 *  original copy, cut OLED related code
 *  try and use Attiny85, LGC-12002, 4 digits LED moudle or LCD1602
 *  Sketch uses 3030 bytes (46%) of program storage space. Maximum is 6586 bytes.
 *  Global variables use 117 bytes (22%) of dynamic memory, leaving 395 bytes for local variables. Maximum is 512 bytes.
 *  xiaolaba
*/


// original copy and source code,
/* Tiny Thermocouple Thermometer

   David Johnson-Davies - www.technoblogy.com - 5th March 2019
   ATtiny85 @ 1 MHz (internal oscillator; BOD disabled)
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
*/

/*
Compiling the program

I compiled the program using Spence Konde's ATTiny Core [11]. 
Choose the ATtiny25/45/85 option under the ATTinyCore heading on the Board menu. 
Then check that the subsequent options are set as follows (ignore any other options):

Chip: "ATtiny85"
Clock: "1 MHz (internal)"
B.O.D: "B.O.D. Disabled"
Timer 1 Clock: "CPU"
*/


//#include <Wire.h>
#include <avr/sleep.h>   

//#include "lcd1602.h"
//#include "oled.h"

#include "PT6961.h"
#define colon_off 0 // no show colon
#define colon_on 1 // show colon

// ATtiny13, MicroCore, https://github.com/MCUdude/MicroCore#boards-manager-installation
// Arduibo pin names
//   2, PB2, IC pin#7
//   1, PB1, IC pin#6
//   0, PB0, IC pin#5
#define DIN 2
#define CLK 1
#define CS  0

PT6961 LGC12002 (DIN, CLK, CS);




// Readings **********************************************

void SetupInternal () {
  ADMUX = 0<<REFS2 | 2<<REFS0 | 15<<MUX0;  // Temperature and 1.1V reference
  ADCSRA = 1<<ADEN | 1<<ADIE | 4<<ADPS0;   // Enable ADC, interrupt, 62.5kHz clock
  ADCSRB = 0<<ADTS0;                       // Free running
  set_sleep_mode(SLEEP_MODE_ADC);
}


  
unsigned int ReadInternal () {
  sleep_enable();
  sleep_cpu();                             // Start in ADC noise reduction mode
  return ADC;
}



void SetupThermocouple () {
  ADMUX = 0<<REFS2 | 2<<REFS0 | 7<<MUX0;   // PB4 (ADC2) +, PB3 (ADC3) was 7
  ADCSRA = 1<<ADEN | 1<<ADIE | 4<<ADPS0;   // Enable ADC, interrupt, 62.5kHz clock
  ADCSRB = 0<<ADTS0;                       // Free running 
  set_sleep_mode(SLEEP_MODE_ADC);
}


unsigned int ReadThermocouple () {
  sleep_enable();
  sleep_cpu();                             // Start in ADC noise reduction mode
  return ADC;
}


// Fixed points for linear interpolation
int Temperature[9] = { 0, 1684, 3370, 4995, 6613, 8269, 9993, 11805, 13751 };

// Converts a x4 ADC reading from 0 to 4095 to a temperature in degrees
int Convert (int adc) {
  int n = adc>>9;
  unsigned int difference = Temperature[n+1] - Temperature[n];
  unsigned int proportion = adc - (n<<9);
  unsigned int extra = ((unsigned long)proportion * difference)>>9;
  return (Temperature[n] + extra + 5)/10;
}

 
ISR(ADC_vect) {
// ADC conversion complete
}



// Setup **********************************************

// Calibrations
const int ADCOffset = -7;
const int InternalOffset = -1;

void setup() {

//  Wire.begin();
//  InitDisplay();
//  ClearDisplay();

//  useOled ();
//  useLCD1602();
    LGC12002.initDisplay();

}
  
void loop () {
  //
  // Internal
  SetupInternal();
  ReadInternal();                          // Throw away first reading
  int internal = 0;
  for (int i=0; i<16; i++) internal = internal + ReadInternal();
  internal = (internal>>4) - 276 + InternalOffset;
////  Scale = 1;
//  PlotTemperature(internal, 3, 6, 1); // temperature, line, column, Scale
//  blinkLCD(); // xiaolaba testing
    LGC12002.sendNum(internal,colon_on);

  delay(1500);

  //
  // Thermocouple
  SetupThermocouple();
  ReadThermocouple();                      // Throw away first reading
  int reading = 0;
  for (int i=0; i<16; i++) reading = reading + ReadThermocouple();
  reading = Convert(max((reading>>2) + ADCOffset*4, 0));
////  Scale = 2;
//  PlotTemperature(reading + internal, 0, 0, 2);  // temperature, line, column, Scale
//  blinkLCD(); // xiaolaba testing
    LGC12002.sendNum(reading,colon_off);

  delay(1500);
}
