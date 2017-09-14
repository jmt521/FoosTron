#include "SevSegController.h"
#include "LedController.h"
#include "FoosConfig.h"
#include <ArduinoJson.h>

//CONTROLLERS
SevSegController scoreboard = SevSegController(COMMON_CATHODE, DISP_PINS);
LedController leds = LedController(LED_PINS);
LedController btnLeds = LedController(BTN_LED_PINS);

//GAME STATE GLOBALS
int winner = -1;
int scores[2] = {0, 0};
int resetCount = 0;
boolean goals_allowed = false;
int num_balls = 1;
boolean is_tiebreak = false;
boolean show_tiebreak = false;
int tiebreak_total = -1;
int tiebreak_diff = -1;

// DEBOUNCE VARS
unsigned long lastDebounceTime[BTN_COUNT];
int buttonState[BTN_COUNT];
int lastButtonState[BTN_COUNT];
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

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
  leds.fadeAll(0,LED_BRIGHTNESS_DIM);

  //Wait for start buttons
  int btns[] = {4,5};
  wait_for_press(btns,BTN_LED_PINS);

  //Select number of balls and start the game
  set_num_balls();
  new_game();
}

void loop() {
  //Check for a winner
  if(winner != -1)
  {
    win();
  }

  //Check for tiebreak
  if(show_tiebreak)
  {
    tiebreak();
    show_tiebreak = false;
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
        set_num_balls();
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
   send_event("goal");
   
    if(!isWin)
    {
      //Light up the opposite goal
      int goal = (player == 0) ? 1 : 0;
      leds.flash(goal, 3, LED_BRIGHTNESS_PLAY, LED_BRIGHTNESS_MAX); 
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

void set_num_balls()
{
  bool cont = false;
  int blinkCount = 2000;
  int displayOn = true;
  while(!cont)
  {
    if(displayOn)
    {
      scoreboard.displayAll(num_balls);
      btnLeds.setAll(255);
    }
    else
    {
      scoreboard.displayAll(21);
      btnLeds.setAll(0);
    }
    
    //Check for a single button press
    for(int i=0; i<BTN_COUNT-2; i++) 
    {
      if(debounce(i)) 
      {
        switch(i) {
          case 0:
          case 2:
            num_balls++;
            break;
          case 1:
          case 3:
            num_balls--;
            break;
        }
      }
    }

    //Keep number within defined limits
    num_balls = fmax(num_balls, BALLS_MIN);
    num_balls = fmin(num_balls, BALLS_MAX);

    //Check for enter button
    for(int i=4; i<6; i++) 
    {
      if(debounce(i)) 
      {
        cont = true;
        leds.setAll(LED_BRIGHTNESS_DIM);
        btnLeds.setAll(0);
        scoreboard.displayAll(21);
      }
    }

    //Manage blink timing
    blinkCount--;
    if(blinkCount == 0)
    {
      if(displayOn)
      {
        displayOn = false;
        blinkCount = 1000;
      }
      else
      {
        displayOn = true;
        blinkCount = 2000;
      }
    }
  }
}

void wait_for_press(const int btnNums[], const int ledPins[])
{
  btnLeds.setAll(255);
    
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

  btnLeds.setAll(0);
}

boolean goal_adjust(int player, int amt)
{
  //Determine other player
  int other = (player == 0)? 1 : 0;

  //Adjust the score
  scores[player] += amt;
  
  //Check for acceptable score limits
  scores[player] = fmax(scores[player], SCORE_MIN);
  scores[player] = fmin(scores[player], SCORE_MAX);

  //Check for multi-ball scoring
  if(MULTIBALL && num_balls > 1)
  {
    if(is_tiebreak)
    {
      if(scores[player] + scores[other] == tiebreak_total)
      {
        if(scores[player] > scores[other])
        {
          winner = player;
          return true;
        }
        else if(scores[other] > scores[player])
        {
          winner = other;
          return true;
        }
      }
    }
    if((scores[player] + scores[other]) % num_balls == 0)
    {
      if(scores[player] >= SCORE_TO_WIN && scores[player] > scores[other])
      {
        //Last to score wins if they are ahead
        winner = player;
        return true;
      }
      else if(scores[other] >= SCORE_TO_WIN && (scores[other] - scores[player]) >= num_balls)
      {
        //If other is ahead by more than the number of balls, they win
        winner = other;
        return true;
      }
      else if((scores[player] >= SCORE_TO_WIN || scores[other] >= SCORE_TO_WIN) 
                  && abs(scores[player]-scores[other]) < num_balls)
      {
        //If score is within number of balls, go to tiebreaker
        //Show the number of balls for tiebreaker, 1 more than score difference
        stop_goals();
        is_tiebreak = true;
        show_tiebreak = true;
        tiebreak_diff = abs(scores[player]-scores[other]) + 1;
        tiebreak_total = scores[player] + scores[other] + tiebreak_diff;
      }
    }
    return false;
  }
  //Normal scoring
  else
  {
    //Check for win condition
    if(scores[player] >= SCORE_TO_WIN) {
      winner = player;
      return true;
    }
    return false; 
  }
}

void tiebreak()
{
  //Show tiebreak number
  for(int i=0; i<3; i++)
  {
    leds.setAll(LED_BRIGHTNESS_MAX);
    scoreboard.displayAll(tiebreak_diff);
    delay(500);
    leds.setAll(LED_BRIGHTNESS_PLAY);
    scoreboard.displayAll(21);
    delay(500);
  }
  scoreboard.displayAll(tiebreak_diff);
  //Wait for start buttons
  wait_for_press(START_BTNS, BTN_LED_PINS);
  allow_goals();
}

void win()
{
  //Remove interrupts from sensors so more goals can't be scored
  stop_goals();

  send_event("win");
  
  //determine the loser
  int loser = 0;
  if(winner == 0)
  {
    loser = 1;
  }
  
  //Animate LEDs and scoreboard
  leds.set(LED_BRIGHTNESS_MAX, winner);
  scoreboard.animate(winner);

  //Update the boards with final scores
  scoreboard.display(scores[0], 0);
  scoreboard.display(scores[1], 1);

  //Animate LEDs
  leds.fade(LED_BRIGHTNESS_PLAY, 0, loser);
  delay(3000);
  leds.fade(LED_BRIGHTNESS_MAX, 0, winner);
  leds.fadeAll(0,LED_BRIGHTNESS_DIM);

  //Wait for start buttons
  const int btns[] = {4,5};
  wait_for_press(btns,BTN_LED_PINS);
  
  new_game();
}

void new_game()
{
  //Remove interrupts from sensors so more goals can't be scored
  stop_goals();
  scoreboard.displayAll(21);

  //Animate
  leds.setAll(LED_BRIGHTNESS_MAX);
  delay(500);
  leds.fadeAll(LED_BRIGHTNESS_MAX,LED_BRIGHTNESS_PLAY);
  scoreboard.animateAll();
  scoreboard.displayAll(0);

  //Reset scores and turn LEDs on
  scores[0] = 0;
  scores[1] = 0;
  winner = -1;
  is_tiebreak = false;
  show_tiebreak = false;
  tiebreak_total = -1;
  tiebreak_diff = -1;

  //Update clients
  send_event("start");
  
  //Reattach interrupts
  allow_goals();
}

void allow_goals()
{
  goals_allowed = true;
}

void stop_goals()
{
  goals_allowed = false;
}

void send_event(String type)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["event"] = type;

  JsonArray& score_msg = root.createNestedArray("score");
  score_msg.add(scores[0]);
  score_msg.add(scores[1]);
  
  root.printTo(Serial);
  Serial.flush ();
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

  if ((millis() - lastDebounceTime[btnNum]) > BTN_DEBOUNCE_DELAY) {
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

