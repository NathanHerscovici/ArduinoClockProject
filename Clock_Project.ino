#include <LiquidCrystal.h>
#include <String.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define contrast 9
#define brightness 10

int hours = 0;
int seconds = 0;
int minutes = 0;
unsigned long pastTime = 0;
int counter = 0;
int aState;
int aLastState;
int state = 0;
int buttonState;
int previousButtonState;
bool isButtonPressed;
bool setMode;
void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  Serial.println("test");
  lcd.begin(16, 2);
  pinMode(contrast, OUTPUT);
  pinMode(brightness, OUTPUT);
  digitalWrite(contrast, LOW);
  analogWrite(brightness, 255);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  aLastState = digitalRead(A0);
  digitalWrite(contrast, LOW);
  analogWrite(brightness, 255);
  lcd.setCursor(0, 0);
  //lcd.blink();
  buttonState = digitalRead(A1);
  previousButtonState = buttonState;
  state = 3;
  setMode = false;

  lcd.print("00:00:00");
  pastTime = millis();

}

void loop() {
  // put your main code here, to run repeatedly:

  aState = digitalRead(A1);
  buttonState = digitalRead(A2);

  if (!setMode)
  {
    if (millis() - pastTime >= 1000)
    {
      pastTime = millis();
      if (seconds != 59)
      {
        seconds++;
      }
      else
      {
        if (minutes != 59)
        {
          seconds = 0;
          minutes++;
        }
        else
        {
          if (hours != 23)
          {
            seconds = 0;
            minutes = 0;
            hours++;
          }
          else
          {
            seconds = 0;
            minutes = 0;
            hours = 0;
          }
        }
      }
    }
  }

  if (previousButtonState == 1 && buttonState == 0)
  {
    isButtonPressed = buttonPress;
  }
  else
    isButtonPressed = false;

  if (isButtonPressed == true)
    setMode = true;

  if (setMode == true)
    setClocks();

  previousButtonState = buttonState;
  aLastState = aState;

  lcd.setCursor(0, 0);
  lcd.print(hours / 10);
  lcd.print(hours % 10);
  lcd.setCursor(3, 0);
  lcd.print(minutes / 10);
  lcd.print(minutes % 10);
  lcd.setCursor(6, 0);
  lcd.print(seconds / 10);
  lcd.print(seconds % 10);
  lcd.setCursor(2, 0);
  lcd.print(":");
  lcd.setCursor(5, 0);
  lcd.print(":");

}

bool buttonPress()
{
  delay(300);
  if (digitalRead(A2) == 0)
  {
    return true;
  }
  else
    return false;
}

void setClocks()
{

  //lcd.setCursor(0, 1);
  //lcd.print("in");
  lcd.blink();
  bool isPressed = buttonPress();
  int oldHours = hours;
  int oldMinutes = minutes;
  int oldSeconds = seconds;

  while ((digitalRead(A2) != 0) && !isPressed)
  {

    aState = digitalRead(A1);
    if (aState != aLastState) {
      if (digitalRead(A0) != aState)
      {
        if ((hours / 2) != 23)
          hours++;
        else
          hours = 0;
      }
      else {
        if (hours != 0)
          hours--;
        else
          hours = 47;
      }
    }

    if (oldHours != hours)
    {
      lcd.setCursor(0, 0);
      lcd.print((hours / 2) / 10);
      lcd.print((hours / 2) % 10);
    }
    oldHours = hours;
    aLastState = aState;
  }

  isPressed = false;
  delay(300);

  while ((digitalRead(A2) != 0) && !isPressed)
  {

    aState = digitalRead(A1);
    if (aState != aLastState) {
      if (digitalRead(A0) != aState)
      {
        if ((minutes / 2) != 59) {
          minutes++;
          Serial.println("non wrap add");
        }
        else {
          minutes = 0;
          Serial.println("wrap add");
        }
      }
      else {
        if (minutes != 0) {
          minutes--;
          Serial.println("non wrap subtract");
        }
        else {
          minutes = 119;
          Serial.println("wrap subtract");
        }
      }
    }

    if (oldMinutes != minutes)
    {
      lcd.setCursor(3, 0);
      lcd.print((minutes / 2) / 10);
      lcd.print((minutes / 2) % 10);
    }
    oldMinutes = minutes;
    aLastState = aState;

  }

  isPressed = false;
  delay(300);

  while ((digitalRead(A2) != 0) && !isPressed)
  {
    aState = digitalRead(A1);
    if (aState != aLastState) {
      if (digitalRead(A0) != aState)
      {
        if ((seconds / 2) != 59)
          seconds++;
        else
          hours = 0;
      }
      else {
        if (seconds != 0)
          seconds--;
        else
          seconds = 119;
      }

      if (oldSeconds != seconds)
      {
        lcd.setCursor(6, 0);
        lcd.print((seconds / 2) / 10);
        lcd.print((seconds / 2) % 10);
      }
      oldSeconds = seconds;
      aLastState = aState;
    }
  }

  hours = hours/2;
  minutes = minutes/2;
  seconds = seconds/2;
  setMode = false;
  lcd.noBlink();
  return;

}
