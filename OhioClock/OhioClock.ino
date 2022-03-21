#include <Wire.h>
#include "rgb_lcd.h"
#include "RTClib.h"
#include <EEPROM.h>
#include <FastLED.h>

#define JOYBUTTON 2
#define JOYX A1
#define JOYY A0

#define NUM_LEDS 395
#define TOP_SIZE 35
#define DATA_PIN 7

#define SEGHEIGHT 11
#define SEGWIDTH 12

#define xsensi 300
#define ysensi 100
#define xcenter 512
#define ycenter 512

CRGB leds[NUM_LEDS];
rgb_lcd lcd;
RTC_DS3231 rtc;

long clickedTime = -1;
byte mode;// 0 = main menu, 1 = options, 2 = changeHours
byte loopCount = 0;
int8_t subMode = 0;//signed byte
int8_t currentDirection = -1;//0 = up, 1 = right, 2 = down, 3 = left
byte countDownHours = 24;
byte countDownMinutes = 0;
const short countUpTime = 10000;//take 10 seconds
int startAnimationTime = -countUpTime;
boolean controlled = false;
unsigned long targetTime = 0;
boolean running = false;
const char loadingChars[5] = {'|','+','-','+'};

boolean writeFast = true;

void setup() {
  //init
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(F("OHI/O Clock"));
  lcd.setCursor(0, 1);
  lcd.print(F("Max Fojtik 2020"));
  LEDS.addLeds<WS2812,DATA_PIN,GRB>(leds,NUM_LEDS);
  LEDS.setBrightness(255);
  pinMode(JOYBUTTON, INPUT_PULLUP);
  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(4, INPUT_PULLUP);
  Serial.begin(9600);
  if(!rtc.begin()) 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("Can't find RTC!"));
    lcd.setCursor(0,1);
    lcd.print(F("Can't run"));
    while (1);
  }
  LEDS.setBrightness(67);
  for(int i = 0; i < 2; i++)
  {
    lcd.setRGB(255,0,0);
    setAll(255,0,0);
    delay(333);
    lcd.setRGB(0,255,0);
    setAll(0,255,0);
    delay(333);
    lcd.setRGB(0,0,255);
    setAll(0,0,255);
    delay(333);
  }
  lcd.setRGB(255,255,255);
  setAll(255,255,255);
  delay(1000);
  setAll(0,0,0);
  LEDS.setBrightness(255);
  
  mode = 0;
  DateTime now = rtc.now();
  unsigned long unixTimeNow = now.unixtime();
  unsigned long lastTarget = EEPROM_readlong(0);
  if(lastTarget>unixTimeNow)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("WE LOST POWER!"));
    lcd.setCursor(0,1);
    lcd.print(F("CLICK TO LOAD"));
    for(int i = 0; i < 5000 && digitalRead(JOYBUTTON) && !controlled; i++)
    {
//      checkSerial();
      delay(1);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("CLICK TO LOAD"));
    lcd.setCursor(0,1);
    lcd.print(F("HOLD TO IGNORE"));
    while(digitalRead(JOYBUTTON) && !controlled)//wait for click
    {
//      checkSerial();
      delay(1);
    }
    unsigned long startClick = millis();
    delay(100);
    while(!digitalRead(JOYBUTTON) && millis()-startClick<5000 && !controlled)//wait to let go
    {
//      checkSerial();
      delay(1);
    }
    if(millis()-startClick<5000 && !controlled)
    {
      startWithTime(lastTarget);
      startAnimationTime = -countUpTime;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("Loaded..."));
      delay(500);
    }
  }
  if (rtc.lostPower()) 
  {
    lcd.print(F("RTC lost power!"));
    lcd.setCursor(0,1);
    lcd.print(F("Cant' Recover"));
    delay(1000);
  }
}
void loop() 
{
//  Serial.print(analogRead(JOYX));
//  Serial.print("\t");
//  Serial.print(analogRead(JOYY));
//  Serial.print("\t");
//  Serial.println(currentDirection);
  int joyX = analogRead(JOYX);
  int joyY = analogRead(JOYY);
  boolean joyButton = digitalRead(JOYBUTTON);
  if(running)
  {
//    Serial.println(remainingTime());
  }
  if(clickedTime==-1 && !joyButton)
  {
    clickedTime = millis();
  }
  else if((millis()-clickedTime)<500 && clickedTime!=-1 && joyButton)
  {
    clicked();
  }
  else if((millis()-clickedTime)>500 && clickedTime!=-1)
  {
    held();
  }
  if(joyButton)
  {
    clickedTime = -1;
  }
  if(currentDirection == -1 && joyX>(xcenter+xsensi))
  {
    currentDirection = 1;
    right();
  }
  else if(currentDirection == -1 && joyX<(xcenter-xsensi))
  {
    currentDirection = 3;
    left();
  }
  else if(currentDirection == -1 && joyY>(ycenter+ysensi))
  {
    currentDirection = 2;
    down();
  }
  else if(currentDirection == -1 && joyY<(ycenter-ysensi))
  {
    currentDirection = 0;
    up();
  }
  if(abs(joyX-xcenter)<xsensi && abs(joyY-ycenter)<ysensi)
  {
    currentDirection = -1;
  }
  updateLEDs();
  if(mode==0 && !running && !digitalRead(4))//start the clock if the top button is clicked on D4 and we are in mode 0
  {
    start();
  }
//  checkSerial();
//  Serial.println(writeFast);
  if(!writeFast)
  {
    updateLCD();
    delay(200);
  }
  else
  {
    loopCount++;
    if(loopCount%17==0)
    {
//      Serial.println("LCD");
      updateLCD();
    }
  }
}
