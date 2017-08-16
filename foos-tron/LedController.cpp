#include "Arduino.h"
#include "LedController.h"

LedController::LedController(int _pins[])
{
  pins = _pins;
  led_count = sizeof(_pins);

}

void LedController::setAll(int r)
{
  for(int i=0; i<led_count; i++)
  {
    analogWrite(pins[i], r);
  }
}

void LedController::set(int r, int player)
{
  analogWrite(pins[player], r);
}

void LedController::fadeAll(int rStart, int rEnd)
{
  if(rStart < rEnd)
  {
    for(int i=rStart; i<=rEnd; i++)
    {
      for(int j=0; j<led_count; j++)
      {
        analogWrite(pins[j], i);
      }
      delay(15);
    }
  }
  else
  {
    for(int i=rStart; i>=rEnd; i--)
    {
      for(int j=0; j<led_count; j++)
      {
        analogWrite(pins[j], i);
      }
      delay(20);
    }
  }
}

void LedController::fade(int rStart, int rEnd, int player)
{
  if(rStart < rEnd)
  {
    for(int i=rStart; i<=rEnd; i++)
    {
      analogWrite(pins[player], i);
      delay(20);
    }
  }
  else
  {
    for(int i=rStart; i>=rEnd; i--)
    {
      analogWrite(pins[player], i);
      delay(20);
    }
  }
}


void LedController::flash(int index, int duration, int rStart, int rEnd)
{
  for(int j=0; j<duration; j++)
  {
    for(int i=rStart; i<rEnd; i++)
    {
      analogWrite(pins[index], i);
      delayMicroseconds(2000);
    }
    for(int i=rEnd; i>rStart; i--)
    {
      analogWrite(pins[index], i);
      delayMicroseconds(2000);
    }
  }
}
