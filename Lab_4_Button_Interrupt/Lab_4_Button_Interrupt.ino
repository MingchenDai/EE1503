volatile int state = HIGH;
volatile int flag = LOW;
int count = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, state);
  pinMode(PUSH2, INPUT_PULLUP);

  attachInterrupt(PUSH2, blink, FALLING);
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

void blink()
{
  state = !state;
  flag = HIGH;
}
