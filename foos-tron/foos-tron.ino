#include "SevSegController.h"

// PIN CONFIGURATIONS
// DISPLAY PINS
int DISP_PINS[2][8] = {{22,23,24,25,26,27,28,29},{30,31,32,33,34,35,36,37}};

// GOAL PINS
const int GOAL1_PIN = 18;
const int GOAL2_PIN = 19;

// BUTTON PINS
const int BTN_COUNT = 6;
const int BTN_PINS[BTN_COUNT] = {44,45,10,11,46,47};
const int BTN_LED_PINS[2] = {48,49};

//OTHER CONSTANTS
const int SCORE_MIN = 0;
const int SCORE_MAX = 19;
const int SCORE_TO_WIN = 10;
const int RESET_DELAY = 1000;

//LEDs
const int LED_COUNT = 2;
const int LED_PINS[LED_COUNT] = {6,7};
const int LED_BRIGHTNESS_DIM = 10;
const int LED_BRIGHTNESS_PLAY = 50;
const int LED_BRIGHTNESS_MAX = 150;


// GLOBAL OBJECTS
SevSegController scoreboard = SevSegController(COMMON_CATHODE, DISP_PINS);
int winner = -1;
int scores[2] = {0, 0};
int resetCount = 0;
boolean goals_allowed = false;

// DEBOUNCE VARS
const unsigned long DEBOUNCE_DELAY = 50;
unsigned long lastDebounceTime[BTN_COUNT];
int buttonState[BTN_COUNT];
int lastButtonState[BTN_COUNT];
 
void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600); 

  //Goal sensors
  pinMode(GOAL1_PIN, INPUT_PULLUP);
  pinMode(GOAL2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GOAL1_PIN), p1_goal, FALLING);
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

  //Startup animation
  scoreboard.displayAll(20);
  led_fade(0,LED_BRIGHTNESS_DIM);

  //Wait for start buttons
  const int btns[] = {4,5};
  wait_for_press(btns,BTN_LED_PINS);

  new_game();
}

void loop() {
  //Check for a winner
  if(winner != -1)
  {
    win();
  }
  
  //Ensure LEDs are at right level
  for(int j=0; j<LED_COUNT; j++)
  {
    analogWrite(LED_PINS[j], LED_BRIGHTNESS_PLAY);
  }

  //Update the boards with current scores
  scoreboard.display(scores[0], 0);
  scoreboard.display(scores[1], 1);
  
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
  }
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
 if(goals_allowed)
 {
   boolean isWin = goal_adjust(player, 1);
  
    if(!isWin)
    {
      //Light up the opposite goal
      int goal = (player == 0) ? 1 : 0;
      led_goal(goal); 
    }
 }
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

void wait_for_press(const int btnNums[], const int ledPins[])
{
  for(int i=0; i<2; i++) 
  {
    analogWrite(ledPins[i], 255);
  }
    
  boolean cont = false;
  while(!cont)
  {
    for(int i=0; i<2; i++) 
    {
      if(debounce(btnNums[i])) 
      {
        cont = true;
      }
    }
  }

  for(int i=0; i<2; i++) 
  {
    analogWrite(ledPins[i], 0);
  }
}

boolean goal_adjust(int player, int amt)
{
   scores[player] += amt;

   //Check for acceptable score limits
   scores[player] = fmax(scores[player], SCORE_MIN);
   scores[player] = fmin(scores[player], SCORE_MAX);

  //Check for win condition
  if(scores[player] >= SCORE_TO_WIN) {
    winner = player;
    return true;
  }
  return false;
}

void win()
{
  //Remove interrupts from sensors so more goals can't be scored
  stop_goals();
  
  //determine the loser
  int loser = 0;
  if(winner == 0)
  {
    loser = 1;
  }
  
  //Animate LEDs and scoreboard
  led_player_set(LED_BRIGHTNESS_MAX, winner);
  scoreboard.animate(winner);

  //Update the boards with final scores
  scoreboard.display(scores[0], 0);
  scoreboard.display(scores[1], 1);

  //Animate LEDs
  led_player_fade(LED_BRIGHTNESS_PLAY, 0, loser);
  delay(3000);
  led_player_fade(LED_BRIGHTNESS_MAX, 0, winner);
  led_fade(0,LED_BRIGHTNESS_DIM);

  //Wait for start buttons
  const int btns[] = {4,5};
  wait_for_press(btns,BTN_LED_PINS);
  
  new_game();
}

void new_game()
{
  //Remove interrupts from sensors so more goals can't be scored
  stop_goals();

  //Animate
  led_set(LED_BRIGHTNESS_MAX);
  delay(500);
  led_fade(LED_BRIGHTNESS_MAX,LED_BRIGHTNESS_PLAY);
  scoreboard.animateAll();
  scoreboard.displayAll(0);

  //Reset scores and turn LEDs on
  scores[0] = 0;
  scores[1] = 0;
  winner = -1;

  //Reattach interrupts
  allow_goals();
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

void allow_goals()
{
  goals_allowed = true;
}

void stop_goals()
{
  goals_allowed = false;
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

