#include "Arduino.h"

#ifndef SingleSevSeg_h
#define SingleSevSeg_h

#define COMMON_CATHODE	0
#define COMMON_ANODE	1

class SingleSevSeg
{
  public:
    SingleSevSeg(bool displayType, int* pinLayout);
    void display(int digit);
    void winner();

  protected:
    bool displayType;
    int* pinLayout;

  private:
    const bool* getSegMap(int digit);

    const int NUM_PINS = 8;
    const int NUM_DIGITS = 38;
  

    const bool SEGMENT_MAP[40][8]
    {
      //Digits 1-9
      {1,1,1,1,1,1,0,0}, // zero
      {0,1,1,0,0,0,0,0}, // one
      {1,1,0,1,1,0,1,0}, // two
      {1,1,1,1,0,0,1,0}, // three
      {0,1,1,0,0,1,1,0}, // four
      {1,0,1,1,0,1,1,0}, // five
      {1,0,1,1,1,1,1,0}, // six
      {1,1,1,0,0,0,0,0}, // seven
      {1,1,1,1,1,1,1,0}, // eight
      {1,1,1,1,0,1,1,0}, // nine

      // //Digits 10-19, dot adds 10
      {1,1,1,1,1,1,0,1}, // ten
      {0,1,1,0,0,0,0,1}, // eleven
      {1,1,0,1,1,0,1,1}, // twelve
      {1,1,1,1,0,0,1,1}, // thirteen
      {0,1,1,0,0,1,1,1}, // fourteen
      {1,0,1,1,0,1,1,1}, // fifteen
      {1,0,1,1,1,1,1,1}, // sixteen
      {1,1,1,0,0,0,0,1}, // seventeen
      {1,1,1,1,1,1,1,1}, // eighteen
      {1,1,1,1,0,1,1,1}, // nineteen
      {0,1,1,1,0,0,0,0}, // J (20)  - We want to show a J but ran out of numbers 
      {0,1,1,1,0,0,0,0}, // off (21)
    };
};

#endif
