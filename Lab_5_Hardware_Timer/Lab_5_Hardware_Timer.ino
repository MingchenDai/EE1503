#include "Timer.h"

volatile unsigned int count = 0;

void setup()
{
  Serial.begin(9600);

  // set ISR timer isrTimer
  SetTimer(isrTimer, 20);
}

void loop()
{
  Serial.println(count);
  delay(1);
}

void isrTimer(void)
{
  count++;
}
