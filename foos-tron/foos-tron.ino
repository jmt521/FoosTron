#include "SingleSevSeg.h"

int disp1_pins[8] = {22,23,24,25,26,27,28,29};
int disp2_pins[8] = {30,31,32,33,34,35,36,37};
SingleSevSeg disp(COMMON_CATHODE, disp1_pins);
SingleSevSeg disp2(COMMON_CATHODE, disp2_pins);

int goal1_pin = 2;
int goal2_pin = 3;

int btn_pin = 18;

int player1_score = 0;
int player2_score = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  pinMode(goal1_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(goal1_pin), p1_goal, FALLING);

  pinMode(goal2_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(goal2_pin), p2_goal, FALLING);

  pinMode(btn_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(btn_pin), new_game, RISING);
}

void loop() {
  if(player1_score >= 10)
  {
    p1_win();
  }else if(player2_score >= 10)
  {
    p2_win();
  }
  // put your main code here, to run repeatedly:
  disp.display(player1_score);
  disp2.display(player2_score);
}

void p1_win()
{
  disp.winner();
  new_game();
}

void p2_win()
{
  disp2.winner();
  new_game();
}

void p1_goal()
{
  player1_score++;
}

void p2_goal()
{
  player2_score++;
}

void new_game()
{
  Serial.println("Reset");
  player1_score = 0;
  player2_score = 0;
}

