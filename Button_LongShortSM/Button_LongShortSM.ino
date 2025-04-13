#include "Timer.h"

volatile int state = 0;
int timerCounter = 0;
const int COUNTER_THRESHOLD=50;
const int IDLE=0;
const int SHORT=1;
const int LONG=2;
int buttonState=HIGH;
int lastButtonState=HIGH;
int redLedState=HIGH;
int greenLedState=HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(PUSH2,INPUT_PULLUP);
  SetTimer(isrTimer,20);
}

void loop(){
  delay(1);
}

void isrTimer(void){
  Button_SM();
}

bool longClickDect(){
  timerCounter++;
  if(timerCounter == COUNTER_THRESHOLD){
    timerCounter = 0;
    return true;
  }
  else{
    return false;
  }
}

void shortRelease(){
  redLedState =! redLedState;
  digitalWrite(RED_LED, redLedState);
}

void longClick(){
  greenLedState =! greenLedState;
  digitalWrite(GREEN_LED,greenLedState);
}

void Button_SM(){
  unsigned char buttonDect = 0; // Check statue of PUSH: 1 for Activated and 2 for Inactivated
  buttonState = digitalRead(PUSH1);

  if((lastButtonState == HIGH)&&(buttonState == LOW)){
    buttonDect = 1;
  }

  if((lastButtonState == LOW)&&(buttonState == HIGH)){
    buttonDect = 2;
  }
  switch(state){
    case IDLE:
      timerCounter = 0;
      if(buttonDect == 1){
        state = SHORT;
      }
      break;
   case SHORT:
    if(buttonDect == 2){
      shortRelease();
      state = IDLE;
    }
    if(longClickDect()){
      longClick();
      state = LONG;
    }
    break;
   case LONG:
    timerCounter = 0;
    if(buttonDect == 2){
      state = IDLE;
    }
    break;
   default:
    state = IDLE;
    break;
  }
  lastButtonState = buttonState;
}
