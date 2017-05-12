#include "SingleSevSeg.h"

// PIN CONFIGURATIONS
// DISPLAY PINS
int DISP1_PINS[8] = {22,23,24,25,26,27,28,29};
int DISP2_PINS[8] = {30,31,32,33,34,35,36,37};

// GOAL PINS
const int GOAL1_PIN = 2;
const int GOAL2_PIN = 3;

// BUTTON PINS
const int BTN_COUNT = 4;
const int BTN_PINS[BTN_COUNT] = {18,19,20,21};


// GLOBAL OBJECTS
SingleSevSeg scoreboard[2] = {SingleSevSeg(COMMON_CATHODE, DISP1_PINS),SingleSevSeg(COMMON_CATHODE, DISP2_PINS)};
int scores[2] = {0, 0};
int winner = -1;


// DEBOUNCE VARS
const unsigned long DEBOUNCE_DELAY = 50;
unsigned long lastDebounceTime[BTN_COUNT];
int buttonState[BTN_COUNT];
int lastButtonState[BTN_COUNT];
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  pinMode(GOAL1_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GOAL1_PIN), p1_goal, FALLING);

  pinMode(GOAL2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GOAL2_PIN), p2_goal, FALLING);

  //Buttons and debounce
  for(int i=0; i<BTN_COUNT; i++) {
    pinMode(BTN_PINS[i], INPUT);
    lastDebounceTime[i] = 0;
    lastButtonState[i] = LOW;
  }

  scoreboard[0].winner();
  scoreboard[1].winner();
}

void loop() {
  for(int i=0; i<BTN_COUNT; i++) {
    if(debounce(i)) {
      btn_press(i);
    }
  }

  scoreboard[0].display(scores[0]);
  scoreboard[1].display(scores[1]);

  if(winner != -1)
  {
    scoreboard[winner].winner();
    new_game();
  }
}

void win(int player)
{
  winner = player;
}
void p1_goal()
{
  goal(0);
}

void p2_goal()
{
  goal(1);
}

void goal(int player)
{
  scores[player]++;
  if(scores[player] >= 10)
  {
    win(player);
  }
}

void btn_press(int btnNum)
{
  switch(btnNum) {
    case 0:
      goal_adjust(0, 1);
      break;
    case 1:
      goal_adjust(0, -1);
      break;
    case 2: 
      goal_adjust(1, 1);
      break;
    case 3:
      goal_adjust(1, -1);
      break;
  }
}

void goal_adjust(int player, int amt)
{
  scores[player] += amt;
}

void new_game()
{
  Serial.println("Reset");
  scores[0] = 0;
  scores[1] = 0;
  winner = -1;
}

bool debounce(int btnNum) {

  // read the state of the switch into a local variable:
  int reading = digitalRead(BTN_PINS[btnNum]);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState[btnNum]) {
    // reset the debouncing timer
    lastDebounceTime[btnNum] = millis();
  }

  if ((millis() - lastDebounceTime[btnNum]) > DEBOUNCE_DELAY) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState[btnNum]) {
      buttonState[btnNum] = reading;
  
      // only toggle the LED if the new button state is HIGH
      if (buttonState[btnNum] == HIGH) {
        lastButtonState[btnNum] = reading;
        return true;
      }
    }
  }
  lastButtonState[btnNum] = reading;
  return false;
}

