#ifndef FoosConfig_h
#define FoosConfig_h

// ----------- PIN CONFIGURATIONS -------------
// DISPLAY PINS
int DISP_PINS[2][8] = {{15,14,16,26,28,30,32,34},{36,38,40,42,44,46,48,50}};

// GOAL PINS
const int GOAL1_PIN = 18;
const int GOAL2_PIN = 19;

// BUTTON PINS
const int BTN_COUNT = 6;
const int BTN_PINS[BTN_COUNT] = {4,5,9,10,12,2};

//BUTTONS
const int NUMERIC_BTNS[2][2] = {{9,10},{12,2}};
const int START_BTNS[2] = {4,5};
const int RESET_BTNS[2] = {4,5};

//LEDs
const int LED_COUNT = 2;
int LED_PINS[LED_COUNT] = {6,7};

//BUTTON LEDs
const int BTN_LED_COUNT = 2;
int BTN_LED_PINS[BTN_LED_COUNT] = {52,49};

// -------------- GAME OPTIONS -------------------
const int SCORE_MIN = 0;
const int SCORE_MAX = 19;
const int SCORE_TO_WIN = 10;
const bool MULTIBALL = true;
const int BALLS_MIN = 1;
const int BALLS_MAX = 20;

// -------------- TIMING PROPERTIES -------------------
const int RESET_DELAY = 1000;
const int BTN_DEBOUNCE_DELAY = 50;
const int GOAL_DEBOUNCE_DELAY = 50;

// -------------- LIGHT OPTIONS -------------------
const int LED_BRIGHTNESS_DIM = 10;
const int LED_BRIGHTNESS_PLAY = 50;
const int LED_BRIGHTNESS_MAX = 150;

#endif

