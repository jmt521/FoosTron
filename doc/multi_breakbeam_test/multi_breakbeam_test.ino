const byte ledpin = 13;
const byte sensor1pin = 2;
const byte sensor2pin = 3;

//variables to read sensor status
volatile byte sensor1count = 0;
volatile byte sensor2count = 0;
 
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledpin, OUTPUT);      
  // initialize the sensor pin as an input:
  pinMode(sensor1pin, INPUT_PULLUP);       //set as input with pullup resistor
//  pinMode(sensor2pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensor1pin), count1, FALLING); //go to count 1 function when sensor1 goes from HIGH to LOW (beam broken)
//  attachInterrupt(digitalPinToInterrupt(sensor2pin), count2, FALLING);
  
  Serial.begin(9600);
}
 
void loop(){
  Serial.print("Count 1 is ");
  Serial.print(sensor1count,DEC);
  Serial.print("    ");
  Serial.print("Count 2 is ");
  Serial.print(sensor2count,DEC);
  Serial.println(" ");
}

void count1(){
  sensor1count++;
}

void count2(){
  sensor2count++;
}

