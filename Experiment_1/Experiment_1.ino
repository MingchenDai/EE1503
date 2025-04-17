#include "Timer.h"

int mode = HIGH;            // Running mode of GREEN_LED, HIGH for a short period and LOW for a long period
int counter = 0;            // Counter for the timer;
int state = HIGH;           // State of GREEN_LED, HIGH or LOW
int buttonState = HIGH;     // State of the button
int lastButtonState = HIGH; // Last state of the button
void setup()
{
  pinMode(GREEN_LED, OUTPUT);   // Set GREEN_LED to output mode
  pinMode(PUSH2, INPUT_PULLUP); // Set a pull-up resistence for PUSH2 button
  SetTimer(isrTimer, 19);       // Set isrTimer
}

void loop()
{
  delay(1);
}

void isrTimer(void)
{
  buttonState = digitalRead(PUSH2); // Read the state of PUSH2
  if ((buttonState != lastButtonState) && (buttonState == LOW))
  {               // Judge of the button is pressed
    mode = !mode; // Switch the running mode of GREEN_LED
    counter = 0;  // Reset the counter
    state = HIGH; // Reset state of the GREEN LED
  }
  lastButtonState = buttonState; // Store cutrrent button state
  if (((counter == 5) && (mode == HIGH)) || ((counter == 100) && (mode == LOW)))
  {                 // Judge if the state of the LED requires a change
    counter = 0;    // Reset the counter
    state = !state; // Switch the state of GREEN_LED
  }
  digitalWrite(GREEN_LED, state); // Write the state of GREEN_LED
  counter++;
}
