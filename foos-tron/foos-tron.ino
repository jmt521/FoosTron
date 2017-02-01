#include "SingleSevSeg.h"

int disp1_pins[8] = {22,23,25,25,26,27,28,29};
int disp2_pins[8] = {30,31,32,33,34,35,36,37};
SingleSevSeg disp(COMMON_CATHODE, disp1_pins);
SingleSevSeg disp2(COMMON_CATHODE, disp2_pins);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  disp.display(15);
}
