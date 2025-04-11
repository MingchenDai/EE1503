#include "Timer.h"

const int COUNTER_T1s=50;  // 50*20ms=1s
const int COUNTER_T8s=400; // 400*20ms

volatile unsigned char clock1s_flag=0;
volatile unsigned char clock8s_flag=0;

unsigned int clock1s=0; // Counter for 1s timer
unsigned int clock8s=0; // Counter for 8s timer

byte redLedState=HIGH;   // Status of RED_LED
byte greenLedState=HIGH; // Status of GREEN_LED

void setup(){
  pinMode(RED_LED,OUTPUT);    // Set RED_LED to OUTPUT
  digitalWrite(RED_LED,HIGH); // Activate RED_LED

  pinMode(GREEN_LED,OUTPUT);    // Set GREEN_LED to OUTPUT
  digitalWrite(GREEN_LED,HIGH); // Activate GREEN_LED

  SetTimer(isrTimer,20); //Set hardware timer to 20ms
}

void loop(){
  if(clock1s_flag==1){   // Check if reach 1s
    clock1s_flag=0;      // Reset 1s flag
    softTimer_1s_Func(); // 1s flag action
  }
  if(clock8s_flag==1){   // Check if reach 8s
    clock8s_flag=0;      // Reset 8s flag
    softTimer_8s_Func(); // 8s flag action
  }
}

void isrTimer(void){
  if(++clock1s>=COUNTER_T1s){ // 1s Timer counts
    clock1s_flag=1;           // Flag activated
    clock1s=0;                // Reset counter
  }
  if(++clock8s>=COUNTER_T8s){ // 1s Timer counts
    clock8s_flag=1;           // Flag activated
    clock8s=0;                // Reset counter
  }
}

void softTimer_1s_Func(){
  redLedState=!redLedState;
  digitalWrite(RED_LED,redLedState);
}

void softTimer_8s_Func(){
  greenLedState=!greenLedState;
  digitalWrite(GREEN_LED,greenLedState);
}
