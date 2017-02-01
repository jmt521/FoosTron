#include "Arduino.h"
#include "SingleSevSeg.h"

SingleSevSeg::SingleSevSeg(bool _displayType, int* _pinLayout)
{
  displayType = _displayType;
  pinLayout = _pinLayout;

  for(int i=0; i<NUM_PINS; i++)
  {
    pinMode(pinLayout[i], OUTPUT);
  }
}

void SingleSevSeg::display(int digit)
{
  Serial.print("Printing ");
  Serial.println(digit, DEC);

  bool* segMap = SEGMENT_MAP[digit];

  for(int i=0; i<NUM_PINS; i++)
  {
   Serial.print(segMap[i], DEC);
   Serial.print("|");

    if(displayType == COMMON_CATHODE)
    {
      digitalWrite(pinLayout[i], segMap[i]);
    } else {
      digitalWrite(pinLayout[i], !segMap[i]);
    }
  }
  Serial.println(" ");
}
