/*

*/
#include <JC_Button.h>          // https://github.com/JChristensen/JC_Button

// pins
#define RSW   4
#define YSW   2
#define GSW   9
#define BSW   5
#define RLED  3
#define YLED  10
#define GLED  6
#define BLED  7
#define SP    A0
#define SLEEP_DLY 5000

// buttons
Button RBtn(RSW);
Button YBtn(YSW);
Button GBtn(GSW);
Button BBtn(BSW);

unsigned long lastActivity;

void setup() {

  // initialize serial debug port
  Serial.begin(9600);

  // initialize the button objects
  RBtn.begin();
  YBtn.begin();
  GBtn.begin();
  BBtn.begin();

  // initialize output pins
  pinMode(RLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  pinMode(SP, OUTPUT);

  // store start time
  lastActivity = millis();
}

void led_on(int led_num) {
  switch (led_num) {
    case 0:
      digitalWrite(RLED, HIGH);
      break;
    case 1:
      digitalWrite(YLED, HIGH);
      break;
    case 2:
      digitalWrite(GLED, HIGH);
      break;
    case 3:
      digitalWrite(BLED, HIGH);
      break;
  }
}

void led_off(int led_num) {
  switch (led_num) {
    case 0:
      digitalWrite(RLED, LOW);
      break;
    case 1:
      digitalWrite(YLED, LOW);
      break;
    case 2:
      digitalWrite(GLED, LOW);
      break;
    case 3:
      digitalWrite(BLED, LOW);
      break;
  }
}

void sleepmode()
{
  long time1 = millis();
  int led = 0;
  int i;
  bool incled;

  // turn off all leds
  for (i = 0; i < 4; i++) led_off(i);

  while (true)
  {
    // Read all buttons every loop
    RBtn.read();
    YBtn.read();
    GBtn.read();
    BBtn.read();

    if (millis() > time1 + 3000)
    {
      Serial.println(led);
      // turn last led off
      if (led == 0)
      {
        led_off(3);
      }
      else
      {
        led_off(led - 1);
      }

      // turn led on
      led_on(led);

      // start over again
      time1 = millis();
      incled = true;
    }

    // Here be dragons, led values seem wrong
    if ((RBtn.wasPressed() && led == 1) ||
        (YBtn.wasPressed() && led == 2) ||
        (GBtn.wasPressed() && led == 3) ||
        (BBtn.wasPressed() && led == 0))
    {
      lastActivity = millis();
      tone(A0, 523, 200);
      delay(200);
      tone(A0, 784, 200);
      delay(200);
      tone(A0, 1046, 200);
      delay(200);
      tone(A0, 523, 200);
      delay(200);
      tone(A0, 784, 200);
      delay(200);
      tone(A0, 1046, 200);
      delay(200);
      

      // turn off all leds and return
      for (i = 0; i < 4; i++) led_off(i);
      return;
    }

    if ((RBtn.wasPressed() && led != 0) ||
        (YBtn.wasPressed() && led != 1) ||
        (GBtn.wasPressed() && led != 2) ||
        (BBtn.wasPressed() && led != 3))
    {
      lastActivity = millis();
      tone(A0, 1046, 200);
      delay(200);

      // turn off all leds and return
      for (i = 0; i < 4; i++) led_off(i);
      return;
    }
    //next led
    if (incled)
    {
      if (led >= 3) led = 0;
      else led++;
      incled = false;
    }
  }
}


// the loop function runs over and over again forever
void loop() {
  // C tone(A0, 523, 200);
  // F tone(A0, 698, 200);
  // G tone(A0, 784, 200);
  // C tone(A0, 1046, 200);

  static bool Rsta;
  static bool Ysta;
  static bool Gsta;
  static bool Bsta;


  // Read all buttons every loop
  RBtn.read();
  YBtn.read();
  GBtn.read();
  BBtn.read();

  if (RBtn.wasPressed())
  {
    Rsta = !Rsta;
    Rsta ? led_on(0) : led_off(0);
    Rsta ? tone(A0, 523, 100) : tone(A0, 523, 100);
    lastActivity = millis();
  }
  if (YBtn.wasPressed())
  {
    Ysta = !Ysta;
    Ysta ? led_on(1) : led_off(1);
    Ysta ? tone(A0, 698, 100) : tone(A0, 698, 100);
    lastActivity = millis();
  }
  if (GBtn.wasPressed())
  {
    Gsta = !Gsta;
    Gsta ? led_on(2) : led_off(2);
    Gsta ? tone(A0, 784, 100) : tone(A0, 784, 100);
    lastActivity = millis();
  }
  if (BBtn.wasPressed())
  {
    Bsta = !Bsta;
    Bsta ? led_on(3) : led_off(3);
    Bsta ? tone(A0, 1046, 100) : tone(A0, 1046, 100);
    lastActivity = millis();
  }

  // check if we are sleeping
  if (millis() > lastActivity + SLEEP_DLY)
  {
    sleepmode();
    Rsta = false;
    Ysta = false;
    Gsta = false;
    Bsta = false;
  }
}
