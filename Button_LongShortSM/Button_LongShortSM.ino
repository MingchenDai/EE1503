#include "Timer.h"

// Define long push period
#define COUNTER_THRESHOLD 50

// Define states of state machine
#define IDLE 0
#define SHORT 1
#define LONG 2

// Intialize state of state machine and counter
int state = 0;
int timerCounter = 0;

// Intialize state of button and led
volatile int buttonState = HIGH;
volatile int lastButtonState = HIGH;
volatile int redLedState = HIGH;
volatile int greenLedState = HIGH;

void setup()
{
  // Set leds to output modes
  pinMode(GREEN_LED, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
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

// Detect long clicks
bool longClickDect()
{
  timerCounter++;
  if (timerCounter == COUNTER_THRESHOLD)
  {
    timerCounter = 0;
    return true;
  }
  else
  {
    return false;
  }
}

// Actions for short click
void shortRelease()
{
  redLedState = !redLedState;
  digitalWrite(RED_LED, redLedState);
}

// Actions for long click
void longClick()
{
  greenLedState = !greenLedState;
  digitalWrite(GREEN_LED, greenLedState);
}

void Button_SM()
{
  // Check statue of PUSH: 1 for Activated and 2 for Inactivated
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
  case IDLE:
    timerCounter = 0;
    if (buttonDect == 1)
    {
      state = SHORT;
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
