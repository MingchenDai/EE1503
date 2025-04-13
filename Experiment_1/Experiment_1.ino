#include "Timer.h"

int mode=HIGH;     // Running mode of LED, HIGH for a short period and LOW for a long period
int counter=0;  // Counter for the timer;
int state=HIGH; // State of the LED, HIGH or LOW
int buttonState=HIGH;
int lastButtonState=HIGH;
void setup() {
  pinMode(GREEN_LED,OUTPUT);   // Set GREEN LED to output mode
  pinMode(PUSH2,INPUT_PULLUP); // Set a pull-up resistence for PUSH2 button
  SetTimer(isrTimer,20);      // Set isrTimer
}

void loop() {
  delay(1);
}

void isrTimer(void){
  buttonState=digitalRead(PUSH2);
  if((buttonState!=lastButtonState)&&(buttonState==LOW)){
    mode=!mode;
    counter=0;
    state=HIGH;
  }
  lastButtonState=buttonState;
  if(((counter==5)&&(mode==HIGH))||((counter==100)&&(mode==LOW))){
    counter=0;
    state=!state;
  }
  digitalWrite(GREEN_LED,state);
  counter++;
}
