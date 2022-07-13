// xiaolaba

#include <Wire.h>
#include <arduino.h>

void InitDisplay (void);
void ClearDisplay (void);
int Stretch (int x);
void PlotChar(int c, int line, int column);
uint8_t DigitChar (unsigned int number, unsigned int divisor);
void PlotTemperature (int temp, int line, int column, int Scale_in);
void useOled (void);
