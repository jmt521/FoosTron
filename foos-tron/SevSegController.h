#include "Arduino.h"

#ifndef SevSegController_h
#define SevSegController_h

#define COMMON_CATHODE	0
#define COMMON_ANODE	1

class SevSegController
{
  public:
    SevSegController(bool displayType, int pinLayout[][8]);
    void displayAll(int digit);
    void display(int digit, int dispNum);
    void animateAll();
    void animate(int dispNum);

  protected:
    bool displayType;
    int (*pinLayout)[8];

  private:
    const bool* getSegMap(int digit);
    const static int NUM_PINS = 8;
    const int NUM_DIGITS = 38;
    int num_displays;
  

    const bool SEGMENT_MAP[22][8]
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

      //Other
      {0,1,1,1,0,0,0,0}, // [20] - J
      {0,0,0,0,0,0,0,0}, // [21] - off
    };
};

#endif
