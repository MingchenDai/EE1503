#include "Timer.h"

volatile int state = HIGH;
volatile int flag = LOW;
int count = 0;

int buttonState = HIGH;
int lastButtonState = HIGH;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, state);
  pinMode(PUSH2, INPUT_PULLUP);

  SetTimer(isrTimer, 20);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (flag)
  {
    digitalWrite(GREEN_LED, state);
    count++;
    Serial.println(count);
    flag = LOW;
  }
}

void isrTimer(void)
{
  Push2_Dect();
}

void Push2_Dect()
{
  buttonState = digitalRead(PUSH2);
  if (buttonState != lastButtonState)
  {
    if (buttonState == LOW)
    {
      state = !state;
      flag = HIGH;
    }
    lastButtonState = buttonState;
  }
}
