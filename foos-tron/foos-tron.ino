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
const int BTN_PINS[BTN_COUNT] = {18,19,11,10};

//OTHER CONSTANTS
const int SCORE_MIN = 0;
const int SCORE_MAX = 19;
const int SCORE_TO_WIN = 10;
const int RESET_DELAY = 1000;

//LEDs
const int LED_COUNT = 2;
const int LED_PINS[LED_COUNT] = {6,7};
const int LED_BRIGHTNESS_PLAY = 25;
const int LED_BRIGHTNESS_MAX = 125;


// GLOBAL OBJECTS
SingleSevSeg scoreboard[2] = {SingleSevSeg(COMMON_CATHODE, DISP1_PINS),SingleSevSeg(COMMON_CATHODE, DISP2_PINS)};
int scores[2] = {0, 0};
int winner = -1;
int resetCount = 0;


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

  //LEDs
  pinMode(LED_PINS[0], OUTPUT);
  pinMode(LED_PINS[1], OUTPUT);

//  animate();
  led_fade(0,LED_BRIGHTNESS_MAX);
  led_fade(LED_BRIGHTNESS_MAX,LED_BRIGHTNESS_PLAY);
}

void loop() {
  //Ensure LEDs are at right level
  for(int j=0; j<LED_COUNT; j++)
  {
    analogWrite(LED_PINS[j], LED_BRIGHTNESS_PLAY);
  }
  
  //Check for the reset button combination
  //If a pair of buttons is held down for a certain number of loops, reset the game
  boolean cont = true;
  for(int i=0; i<BTN_COUNT; i+=2) {
    //If a pair of buttons is held at the same time, increment the reset counter
    if(digitalRead(BTN_PINS[i]) == HIGH && digitalRead(BTN_PINS[i+1]) == HIGH) {
      cont = false;
      resetCount++;
      
      //Wait for the reset delay
      delay(RESET_DELAY/100);

      //Check multiple times for release
      if(resetCount == RESET_DELAY/10) {
        //Reset the game
        animate();
        resetCount = 0;
        new_game();
      }
    }
  }

  //Only continue through the loop if the reset button was not pressed
  if(cont) {
    resetCount = 0;
    //Check for a single button press
    for(int i=0; i<BTN_COUNT; i++) 
    {
      if(debounce(i)) 
      {
        btn_press(i);
      }
    }

    //Update the boards with current scores
    scoreboard[0].display(scores[0]);
    scoreboard[1].display(scores[1]);

    //Check for a winner
    if(winner != -1)
    {
      win();
    }
  }
}

void p1_goal()
{
  goal_adjust(0, 1);
  led_goal(0);
}

void p2_goal()
{
  goal_adjust(1, 1);
  led_goal(1);
}

//Handles all single button presses
//In this case, buttons just adjust scores for the players
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

   //Check for acceptable score limits
   scores[player] = fmax(scores[player], SCORE_MIN);
   scores[player] = fmin(scores[player], SCORE_MAX);

  //Check for win condition
  if(scores[player] >= SCORE_TO_WIN) {
    winner = player;
  }
}

void animate()
{
  scoreboard[0].winner();
  scoreboard[1].winner();
}

void win()
{
  //Make sure there's a winner
  if(winner == -1)
  {
    //We shouldn't be here
    return;
  }
  
  //determine the loser
  int loser = 0;
  if(winner == 0)
  {
    loser = 1;
  }
  //Remove interrupts from sensors so more goals can't be scored
  detachInterrupt(digitalPinToInterrupt(GOAL1_PIN));
  detachInterrupt(digitalPinToInterrupt(GOAL2_PIN));

  //Animate LEDs and scoreboard
  led_player_set(LED_BRIGHTNESS_MAX, winner);
  scoreboard[winner].winner();

  //Update the boards with final scores
  scoreboard[0].display(scores[0]);
  scoreboard[1].display(scores[1]);
  led_player_fade(LED_BRIGHTNESS_PLAY, 0, loser);

  delay(5000);
  
  led_player_fade(LED_BRIGHTNESS_MAX, 0, winner);
  new_game();
}

void new_game()
{
  //Remove interrupts from sensors so more goals can't be scored
  detachInterrupt(digitalPinToInterrupt(GOAL1_PIN));
  detachInterrupt(digitalPinToInterrupt(GOAL2_PIN));

  //Reattach interrupts
  attachInterrupt(digitalPinToInterrupt(GOAL1_PIN), p1_goal, FALLING);
  attachInterrupt(digitalPinToInterrupt(GOAL2_PIN), p2_goal, FALLING);
  
  //Reset scores and turn LEDs on
  scores[0] = 0;
  scores[1] = 0;
  winner = -1;
  led_fade(0,LED_BRIGHTNESS_PLAY);
}

void led_set(int r)
{
  for(int i=0; i<LED_COUNT; i++)
  {
    analogWrite(LED_PINS[i], r);
  }
}

void led_player_set(int r, int player)
{
  analogWrite(LED_PINS[player], r);
}

void led_fade(int rStart, int rEnd)
{
  if(rStart < rEnd)
  {
    for(int i=rStart; i<=rEnd; i++)
    {
      for(int j=0; j<LED_COUNT; j++)
      {
        analogWrite(LED_PINS[j], i);
      }
      delay(15);
    }
  }
  else
  {
    for(int i=rStart; i>=rEnd; i--)
    {
      for(int j=0; j<LED_COUNT; j++)
      {
        analogWrite(LED_PINS[j], i);
      }
      delay(20);
    }
  }
}

void led_player_fade(int rStart, int rEnd, int player)
{
  if(rStart < rEnd)
  {
    for(int i=rStart; i<=rEnd; i++)
    {
      analogWrite(LED_PINS[player], i);
      delay(20);
    }
  }
  else
  {
    for(int i=rStart; i>=rEnd; i--)
    {
      analogWrite(LED_PINS[player], i);
      delay(20);
    }
  }
}

void led_goal(int player)
{
  for(int j=0; j<3; j++)
  {
    for(int i=LED_BRIGHTNESS_PLAY; i<LED_BRIGHTNESS_MAX; i++)
    {
      analogWrite(LED_PINS[player], i);
      delay(2);
    }
    for(int i=LED_BRIGHTNESS_MAX; i>LED_BRIGHTNESS_PLAY; i--)
    {
      analogWrite(LED_PINS[player], i);
      delay(2);
    }
  }
}

bool debounce(int btnNum) 
{
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

