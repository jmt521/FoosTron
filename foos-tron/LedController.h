#include "Arduino.h"

#ifndef LedController_h
#define LedController_h

class LedController
{
  public:
    LedController(int pinLayout[][3]);
    void set(int level, int group);
    void setAll(int level);
    void fade(int start, int end, int group);
    void fadeAll(int start, int end);
  
};

#endif
