/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.

  Hardware Required:
  * LaunchPad with an LED

  This example code is in the public domain.
*/

// most launchpads have a red LED
#define LED RED_LED

// see pins_energia.h for more LED definitions
// #define LED GREEN_LED

// the setup routine runs once when you press reset:
void setup()
{
  // initialize the digital pin as an output.
  pinMode(LED, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop()
{
  digitalWrite(LED, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(2000);             // wait for 2 second
  digitalWrite(LED, LOW);  // turn the LED off by making the voltage LOW
  delay(2000);             // wait for 2 second
  digitalWrite(LED, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for 0.1 second
  digitalWrite(LED, LOW);  // turn the LED off by making the voltage LOW
  delay(100);              // wait for 0.1 second
}
