#include "Timer.h"

// Define long push period
#define COUNTER_THRESHOLD 150

// Define states of state machine
#define LOCK 0
#define UNLOCK 1
#define RELEASE 2

// Intialize state of state machine and counter
int state = 0;
int timerCounter1 = 0;
int timerCounter2 = 0;
bool autoLockFlag = true;

// Intialize state of button and led
volatile int buttonState = HIGH;
volatile int lastButtonState = HIGH;
volatile int redLedState = HIGH;
volatile int greenLedState = LOW;

void setup()
{
  // Set leds to output modes
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  SetTimer(isrTimer, 19);
}

void loop()
{
  delay(1);
}

void isrTimer(void)
{
  Button_SM();
}

bool push1LongClickDect()
{
  timerCounter1++;
  if (timerCounter1 == COUNTER_THRESHOLD)
  {
    timerCounter1 = 0;
    return true;
  }
  else
  {
    return false;
  }
}

void shortRelease()
{
  if (redLedState == LOW)
  {
    greenLedState = !greenLedState;
    digitalWrite(GREEN_LED, greenLedState);
  }
}

void longClick()
{
  redLedState = !redLedState;
  digitalWrite(RED_LED, redLedState);
}

void Button_SM()
{
  // Check statue of PUSH1: 1 for Activated and 2 for Inactivated
  unsigned char buttonDect = 0; 
  buttonState = digitalRead(PUSH1);
  if ((lastButtonState == HIGH) && (buttonState == LOW))
  {
    buttonDect = 1;
  }

  if ((lastButtonState == LOW) && (buttonState == HIGH))
  {
    buttonDect = 2;
  }
  
  switch (state)
  {
  case LOCK:
    timerCounter = 0;
    if (longClickDect())
    {
      longClick();
      state = LONG;
    }
    break;
  case SHORT:
    if (buttonDect == 2)
    {
      shortRelease();
      state = IDLE;
    }
    if (longClickDect())
    {
      longClick();
      state = LONG;
    }
    break;
  case LONG:
    timerCounter = 0;
    if (buttonDect == 2)
    {
      state = IDLE;
    }
    break;
  default:
    state = IDLE;
    break;
  }
  lastButtonState = buttonState;
}
