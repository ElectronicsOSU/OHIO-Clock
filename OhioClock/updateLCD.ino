unsigned long loadingCharsSpot = 0;
void updateLCD()
{
  if(controlled)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    if(running)
    {
      lcd.print(F("Running "));
      lcd.print(loadingChars[loadingCharsSpot++%4]);
    }
    else
    {
      lcd.print(F("Not Running"));
    }
    lcd.setCursor(0, 1);
    lcd.print(F("Under Control"));
  }
  else if(mode == 0)//Startup
  {
    lcd.clear();
    lcd.setCursor(0,0);
    if(running)
    {
      lcd.print(F("Running "));
      lcd.print(loadingChars[loadingCharsSpot++%4]);
    }
    else
    {
      lcd.print(F("Ready to Start"));
    }
    lcd.setCursor(0, 1);
    lcd.print(F("Hold for Menu"));
  }
  else if(mode == 1)//
  {
    lcd.clear();
    lcd.setCursor(0,0);
    if(subMode == -1)
    {
      if(running)
      {
        lcd.print(F("<-   Pause    ->"));
        lcd.setCursor(0, 1);
        lcd.print("Hold to Pause");
      }
      else
      {
        lcd.print(F("<-Change hours->"));
        lcd.setCursor(0, 1);
        lcd.print(countDownHours);
        lcd.print(F(" hours"));
      }
    }
    else if(subMode == -2)
    {
      if(running)
      {
        lcd.print(F("  Time Left   ->"));
        lcd.setCursor(0, 1);
        DateTime now = rtc.now();
        unsigned long unixTimeNow = now.unixtime();
        lcd.print(targetTime-unixTimeNow);
      }
      else
      {
        lcd.print(F("  Change  mins->"));
        lcd.setCursor(0, 1);
        lcd.print(countDownMinutes);
        lcd.print(F(" minutes"));
      }
    }
    else if(subMode == 0)
    {
      if(running)
      {
        lcd.print(F("<-    Back      "));
        lcd.setCursor(0, 1);
      }
      else
      {
        lcd.print(F("<-    Back      "));
        lcd.setCursor(0, 1);
      }
    }
  }
  else if(mode==2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(countDownHours);
    lcd.print(F(" hours"));
  }
  else if(mode==3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(countDownMinutes);
    lcd.print(F(" minutes"));
  }
}
