#include "Arduino.h"

#ifndef LedController_h
#define LedController_h

class LedController
{
  public:
    LedController(int pins[]);
    void set(int level, int index);
    void setAll(int level);
    void fade(int start, int end, int index);
    void fadeAll(int start, int end);
    void flash(int index, int duration, int rStart, int rEnd);
  private:
    int (*pins);
    int led_count;
  
};

#endif
