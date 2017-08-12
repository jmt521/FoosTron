#include "Arduino.h"
#include "SevSegController.h"

SevSegController::SevSegController(bool _displayType, int _pinLayout[][NUM_PINS])
{
  displayType = _displayType;
  pinLayout = _pinLayout;
  num_displays = sizeof(_pinLayout);

  for(int i=0; i<num_displays; i++)
  {
    for(int j=0; j<NUM_PINS; j++)
    
    {
      pinMode(pinLayout[i][j], OUTPUT);
    }
  }
}

void SevSegController::animateAll()
{
  SevSegController::displayAll(21);
  for(int j=0; j<2; j++)
  {
    for(int i=0; i<NUM_PINS-2; i++)
    {
      int out = (displayType == COMMON_CATHODE)? 1 : 0;
      for(int k=0; k<num_displays; k++)
      {
        digitalWrite(pinLayout[k][i], out);
      }
      delay(75);
      out = (displayType == COMMON_CATHODE)? 0 : 1;
      for(int k=0; k<num_displays; k++)
      {
        digitalWrite(pinLayout[k][i], out);
      }
    }
  }
}

void SevSegController::animate(int dispNum)
{
  SevSegController::display(21, dispNum);
  for(int j=0; j<4; j++)
  {
    for(int i=0; i<NUM_PINS-2; i++)
    {
      if(displayType == COMMON_CATHODE)
      {
        digitalWrite(pinLayout[dispNum][i], 1);
      } else {
        digitalWrite(pinLayout[dispNum][i], 0);
      }
      delay(75);
      if(displayType == COMMON_CATHODE)
      {
        digitalWrite(pinLayout[dispNum][i], 0);
      } else {
        digitalWrite(pinLayout[dispNum][i], 1);
      }
    }
  }
}

void SevSegController::displayAll(int digit)
{
  const bool* segMap = SEGMENT_MAP[digit];

  for(int i=0; i<NUM_PINS; i++)
  {
    for(int j=0; j<num_displays; j++)
    {
      if(displayType == COMMON_CATHODE)
      {
        digitalWrite(pinLayout[j][i], segMap[i]);
      } else {
        digitalWrite(pinLayout[j][i], !segMap[i]);
      }
    }
  }
}

void SevSegController::display(int digit, int dispNum)
{
  const bool* segMap = SEGMENT_MAP[digit];

  for(int i=0; i<NUM_PINS; i++)
  {
    if(displayType == COMMON_CATHODE)
    {
      digitalWrite(pinLayout[dispNum][i], segMap[i]);
    } else {
      digitalWrite(pinLayout[dispNum][i], !segMap[i]);
    }
  }
}
