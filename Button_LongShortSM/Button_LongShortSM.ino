#include "Timer.h"
#define COUNTER_THRESHOLD 50
#define IDLE 0
#define SHORT 1
#define LONG 2
int state = 0;
int timerCounter = 0;
volatile int buttonState=HIGH;
volatile int lastButtonState=HIGH;
volatile int redLedState=HIGH;
volatile int greenLedState=HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(PUSH1,INPUT_PULLUP);
  pinMode(RED_LED,OUTPUT);
  SetTimer(isrTimer,19);
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
  Serial.print(state);
  Serial.print(buttonState);
  Serial.println(lastButtonState);
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
