const byte LEDPIN = 2; 
 
void setup() {
  pinMode(LEDPIN, OUTPUT);
  int r = 50; //change to adjust brightness with pwm
  analogWrite(LEDPIN, r); //outputs pulse width
}
 
 
void loop() {
  } 

