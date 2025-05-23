#include "Timer.h"

// Define long push period as 3s
#define COUNTER_THRESHOLD 150

// Define status of each button
#define IDLE 0
#define SHORT 1
#define LONG 2

// Define status of system
#define LOCKED 0
#define UNLOCKED 1
#define RELEASED 2

// Intialize state of each button
volatile int buttonState1 = IDLE;
volatile int buttonState2 = IDLE;

// Intialize status of each button
volatile int buttonStatus1 = HIGH;
volatile int buttonStatus2 = HIGH;
volatile int lastButtonStatus1 = HIGH;
volatile int lastButtonStatus2 = HIGH;

// Define conter for each button and system
volatile int buttonCounter1 = 0;
volatile int buttonCounter2 = 0;
volatile int systemCounter = 0;
volatile int doubleCounter = 0;

// Intialize status of LED
volatile int redLedStatus = HIGH;
volatile int greenLedStatus = LOW;

// Intialize status of system
volatile int systemStatus = LOCKED;
volatile bool isDoubleClick = false;
volatile bool isBothPressed = false;

// Claim function prototypes
bool longPressDect1();
void longPressAction1();
void longPressAction2();
void shortPressAction1();
void shortPressAction2();
void setLedStatus();
void longSystemAction();
void isrTimer();

// Setup the program
void setup()
{
    // Set the pin modes for the buttons and LEDs
    pinMode(PUSH1, INPUT_PULLUP); // Button 1
    pinMode(PUSH2, INPUT_PULLUP); // Button 2
    pinMode(RED_LED, OUTPUT);     // Red LED
    pinMode(GREEN_LED, OUTPUT);   // Green LED

    // Initialize the LEDs to demo LOCKED state
    digitalWrite(RED_LED, redLedStatus);
    digitalWrite(GREEN_LED, greenLedStatus);

    // Initialize the timer
    SetTimer(isrTimer, 20);
}

void setLedStatus()
{
    switch (systemStatus)
    {
    case LOCKED:
        redLedStatus = HIGH;
        greenLedStatus = LOW;
        break;
    case UNLOCKED:
        redLedStatus = LOW;
        greenLedStatus = LOW;
        break;
    case RELEASED:
        redLedStatus = LOW;
        greenLedStatus = HIGH;
        break;
    }
    digitalWrite(RED_LED, redLedStatus);
    digitalWrite(GREEN_LED, greenLedStatus);
}

bool longPressDect1()
{
    buttonCounter1++;
    if ((buttonStatus1 == LOW && buttonStatus2 == LOW) || isBothPressed == true)
    {
        buttonCounter1 = 0;
        isBothPressed = true;
        return false;
    }
    if (buttonCounter1 == COUNTER_THRESHOLD * 2)
    {
        buttonCounter1 = 0;
        return true;
    }
    return false;
}

void longPressAction1()
{
    if (systemStatus == LOCKED)
    {
        systemStatus = UNLOCKED;
    }
}

void shortPressAction1()
{
    // if (systemStatus == UNLOCKED && !(lastButtonStatus2 == LOW && buttonStatus2 == LOW))
    // {
    //     systemStatus = LOCKED;
    // }
}

void shortPressAction2()
{
    switch (systemStatus)
    {
    case UNLOCKED:
        systemStatus = RELEASED;
        break;
    case RELEASED:
        systemStatus = LOCKED;
        break;
    }
}

void longSystemAction()
{
    if (systemStatus == UNLOCKED)
    {
        systemCounter++;
        if (buttonStatus1 == LOW)
        {
            systemCounter = 0;
        }
        if (systemCounter == COUNTER_THRESHOLD)
        {
            if (systemStatus == UNLOCKED && !(lastButtonStatus1 == LOW && buttonStatus1 == LOW))
            {
                systemStatus = LOCKED;
            }
            systemCounter = 0;
        }
    }
    else
    {
        systemCounter = 0;
    }
}

bool doubleClickDect()
{
    if (isDoubleClick && doubleCounter <= 15)
    {

        isDoubleClick = !isDoubleClick;
        doubleCounter = 0;
        return true;
    }
    doubleCounter = 0;
    isDoubleClick = !isDoubleClick;
    return false;
}

void loop()
{
}

void isrTimer()
{
    unsigned char buttonDect1 = 0;
    unsigned char buttonDect2 = 0;
    buttonStatus1 = digitalRead(PUSH1);
    buttonStatus2 = digitalRead(PUSH2);

    if ((lastButtonStatus1 == HIGH) && (buttonStatus1 == LOW))
        buttonDect1 = 1;
    else if ((lastButtonStatus1 == LOW) && (buttonStatus1 == HIGH))
        buttonDect1 = 2;
    if ((lastButtonStatus2 == HIGH) && (buttonStatus2 == LOW))
        buttonDect2 = 1;
    else if ((lastButtonStatus2 == LOW) && (buttonStatus2 == HIGH))
        buttonDect2 = 2;

    switch (buttonState1)
    {
    case IDLE:
        buttonCounter1 = 0;
        if (buttonDect1 == 1)
            buttonState1 = SHORT;
        break;
    case SHORT:
        if (buttonDect1 == 2)
        {
            shortPressAction1();
            buttonState1 = IDLE;
            //            if(doubleClickDect()){
            //                systemStatus=LOCKED;
            //            }
        }
        else if (longPressDect1())
        {
            longPressAction1();
            buttonState1 = LONG;
        }
        break;
    case LONG:
        if (buttonDect1 == 2)
            buttonState1 = IDLE;
        break;
    default:
        buttonState1 = IDLE;
        break;
    }

    switch (buttonState2)
    {
    case IDLE:
        buttonCounter2 = 0;
        if (buttonDect2 == 1)
            buttonState2 = SHORT;
        break;
    case SHORT:
        if (buttonDect2 == 2)
        {
            shortPressAction2();
            buttonState2 = IDLE;
        }
        break;
    default:
        buttonState2 = IDLE;
    }
    longSystemAction();
    setLedStatus();
    if (buttonStatus1 == HIGH && buttonStatus2 == HIGH)
    {
        isBothPressed = false;
    }
    lastButtonStatus1 = buttonStatus1;
    lastButtonStatus2 = buttonStatus2;

    //    if(isDoubleClick){
    //        doubleCounter++;
    //    }
}
