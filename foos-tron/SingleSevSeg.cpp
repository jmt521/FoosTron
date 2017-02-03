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

void SingleSevSeg::winner()
{
  SingleSevSeg::display(0);
  for(int j=0; j<4; j++)
  {
    for(int i=0; i<NUM_PINS-1; i++)
    {
      if(displayType == COMMON_CATHODE)
      {
        digitalWrite(pinLayout[i], 1);
      } else {
        digitalWrite(pinLayout[i], 0);
      }
      delay(100);
      if(displayType == COMMON_CATHODE)
      {
        digitalWrite(pinLayout[i], 0);
      } else {
        digitalWrite(pinLayout[i], 1);
      }
    }
  }
}

void SingleSevSeg::display(int digit)
{
  const bool* segMap = SEGMENT_MAP[digit];

  for(int i=0; i<NUM_PINS; i++)
  {
    if(displayType == COMMON_CATHODE)
    {
      digitalWrite(pinLayout[i], segMap[i]);
    } else {
      digitalWrite(pinLayout[i], !segMap[i]);
    }
  }
}
