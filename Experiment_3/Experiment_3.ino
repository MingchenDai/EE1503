// Include header files
#include "HX711.h"

// Define Units
#define GRAM 0
#define KILOGRAM 1
#define OUNCE 2
#define POUND 3
volatile int unit = GRAM;
const long unitFactors[] = {10000000, 10000, 352740, 22046};
const char *unitLabels[] = {" g", " kg", " oz", " lb"};

// Define variables and constants
volatile int lastButtonState1 = HIGH;
volatile int lastButtonState2 = HIGH;
volatile int buttonState1 = HIGH;
volatile int buttonState2 = HIGH;
volatile bool isInitialized = true;
volatile int counter = 0;

// Initialize HX711
HX711 Scale(7, 6);

// Parameter of linear fitting
// Actually the variable `intercept` is 6 times the real intercept and the variable `slope` is 100 times the real slope.
volatile long intercept = 3400620;
volatile long slope = 43330;
volatile long factor = 259980;
volatile long externalWeight = 0;

void setup()
{
  // Intialize the scale, serial communication, and button pins
  Scale.begin();
  Serial.begin(9600);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  Serial.println("Welcome to the scale program.");
  Serial.println("Press PUSH1 during runtime to switch units bteween gram, kilogram, ounce, and pound.");
  Serial.println("Press PUSH2 during runtime to reset the external weight.");
  Serial.println("Press PUSH1 for 3s to reset the scale.");
  Serial.println();
  Serial.print("Running Automatic Calibration... ");
  intercept = measure_check() * 6;
  Serial.println("Done.");
  Serial.println("Please press PUSH1 to start.");
}

void loop()
{
  // Read the button status
  buttonState1 = digitalRead(PUSH1);
  buttonState2 = digitalRead(PUSH2);

  // Initialize the scale
  if (buttonState1 == LOW && lastButtonState1 == LOW)
    counter++;
  else
    counter = 0;
  if (counter == 30)
  {
    counter = 0;
    isInitialized = false;
    return;
  }
  if (!isInitialized)
  {
    Serial.println("Initializing scale...");
    reset();
  }

  // Read and calculate the weight
  volatile long result = (Scale.HX711_Read() * 6) - intercept;

  // Reset external weight

  if (lastButtonState2 == LOW && buttonState2 == HIGH && buttonState1 == HIGH)
    externalWeight = result;
  result -= externalWeight;

  // Switch the unit
  if (lastButtonState1 == LOW && buttonState1 == HIGH && buttonState2 == HIGH)
    unit = (unit + 1) % 4;
  long result1 = result / 10000 * unitFactors[unit] / factor * 100;
  long result2 = ((result % 10000 / 100) * unitFactors[unit] + ((result / 10000 * unitFactors[unit]) % factor) * 100) / factor;
  long result3 = ((result % 10000 % 100) * unitFactors[unit] + (((result % 10000 / 100) * unitFactors[unit] + ((result / 10000 * unitFactors[unit]) % factor) * 100) % factor) * 100) / (factor * 100);
  result = result1 + result2 + result3;

  // Display the weight
  if (result < 0)
  {
    Serial.print("-");
    result = -result;
  }
  Serial.print(result / 1000);
  Serial.print(".");
  int a = result % 1000;
  if (a < 100)
    Serial.print("0");
  if (a < 10)
    Serial.print("0");
  Serial.print(a);
  Serial.println(unitLabels[unit]);
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
  delay(100);
}

long measure_check()
{
  long weight = 0;
  for (int i = 0; i < 10; i++)
  {
    weight += Scale.HX711_Read();
    delay(100);
  }
  return weight / 10;
}

void confirmPress()
{
  buttonState1 = digitalRead(PUSH1);
  buttonState2 = digitalRead(PUSH2);
  while (!(buttonState1 == HIGH && lastButtonState1 == LOW))
  {
    lastButtonState1 = buttonState1;
    lastButtonState2 = buttonState2;
    buttonState1 = digitalRead(PUSH1);
    buttonState2 = digitalRead(PUSH2);
  }
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
}

void reset()
{
  long weight_0 = 0, weight_50 = 0, weight_100 = 0;
  confirmPress();
  Serial.println("Please place the scale on a stable surface.");
  Serial.println("[1/3] Keep scale clear and press PUSH1 to confirm.");
  confirmPress();
  weight_0 = measure_check();
  Serial.println("[2/3] Place a 50g weight and press PUSH1 to confirm.");
  confirmPress();
  weight_50 = measure_check();
  Serial.println("[3/3] Place a 100g weight and press PUSH1 to confirm.");
  confirmPress();
  weight_100 = measure_check();
  slope = weight_100 - weight_0;
  factor = slope * 6;
  intercept = (weight_0 + weight_50 + weight_100) * 2 - slope * 3;
  Serial.println("Initialization completed, please press PUSH1 to start.");
  confirmPress();
  isInitialized = true;
}