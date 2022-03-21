const boolean SEGBIG[7] = {0,0,1,0,0,1,1};
const boolean SEG[10][7] //n, segon
{
  {//0
    1, 1, 1, 1, 1, 1, 0
  },{//1
    0, 0, 0, 1, 1, 0, 0
  },{//2
    0, 1, 1, 0, 1, 1, 1
  },{//3
    0, 0, 1, 1, 1, 1, 1
  },{//4
    1, 0, 0, 1, 1, 0, 1
  },{//5
    1, 0, 1, 1, 0, 1, 1
  },{//6
    1, 1, 1, 1, 0, 1, 1
  },{//7
    0, 0, 0, 1, 1, 1, 0
  },{//8
    1, 1, 1, 1, 1, 1, 1
  },{//9
    1, 0, 0, 1, 1, 1, 1
  }
};
//0, 82, 196, 278, 360
boolean colonOn = false;
int animationStep = 0;
int animationStep2 = 0;
int lastHue = 0;
byte hue = 0;
byte flashColon[] = {169,170,173,174,185,186,189,190};

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void writeDigit(int offset, byte n, CRGB color)
{
  int count = offset;
  for(int i = 0; i < 7; i++)
  {
    if(SEGBIG[i])
    {
      if(SEG[n][i])
      {
        for(int k = 0; k < SEGWIDTH; k++)
        {
          leds[count] = color;
          count++;
        }
      }
      else
      {
        for(int k = 0; k < SEGWIDTH; k++)
        {
          leds[count] = 0;
          count++;
        }
      }
    }
    else
    {
      if(SEG[n][i])
      {
        for(int k = 0; k < SEGHEIGHT; k++)
        {
          leds[count] = color;
          count++;
        }
      }
      else
      {
        for(int k = 0; k < SEGHEIGHT; k++)
        {
          leds[count] = 0;
          count++;
        }
      }
    }
    if(i==0 || i==3)
    {
      count++;
    }
  }
}
boolean getBit(byte b, byte n)
{
  return (b >> n) & 1;
}
void writeRawSegment(int offset, byte segments, CRGB color)
{
  CRGB off = CRGB(0,0,0);
  int count = offset;
  for(int k = 0; k < SEGHEIGHT; k++)
  {
    leds[count] = getBit(segments, 0) ? color : off;
    count++;
  }
  count++;
  for(int k = 0; k < SEGHEIGHT; k++)
  {
    leds[count] = getBit(segments, 1) ? color : off;
    count++;
  }
  for(int k = 0; k < SEGWIDTH; k++)
  {
    leds[count] = getBit(segments, 2) ? color : off;
    count++;
  }
  for(int k = 0; k < SEGHEIGHT; k++)
  {
    leds[count] = getBit(segments, 3) ? color : off;
    count++;
  }
  count++;
  for(int k = 0; k < SEGHEIGHT; k++)
  {
    leds[count] = getBit(segments, 4) ? color : off;
    count++;
  }
  for(int k = 0; k < SEGWIDTH; k++)
  {
    leds[count] = getBit(segments, 5) ? color : off;
    count++;
  }
  for(int k = 0; k < SEGWIDTH; k++)
  {
    leds[count] = getBit(segments, 6) ? color : off;
    count++;
  }
}
void writeMinutes(byte m, CRGB c)
{
  writeDigit(196, m/10%10, c);
  writeDigit(278, m%10, c);
  FastLED.show();
}
void writeHours(byte m, CRGB c)
{
  writeDigit(0, m/10%10, c);
  writeDigit(82, m%10, c);
  FastLED.show();
}
void writeColon(boolean on, CRGB c)
{
  for(int i = 164; i<196; i++)
  {
    leds[i] = c;
  }
  if(!on)
  {
    for(int i = 0; i<8; i++)
    {
      leds[flashColon[i]] = 0;
    }
  }
  FastLED.show();
}
void disOhio()
{
  writeRawSegment(0,   0b000111111, CRGB(0,255,0));
  writeRawSegment(82,  0b001011011, CRGB(0,255,0));
  writeRawSegment(196, 0b000000011, CRGB(0,255,0));
  writeRawSegment(278, 0b000111111, CRGB(0,255,0));
  writeColon(false, CRGB(0,0,0));
  FastLED.show();
}
byte secondsToHour(unsigned long time)
{//           sec  /min  /hour
  byte hour = time /60   /60;
  return hour;
}
byte secondsToMinutes(unsigned long time)
{//           sec  /min
  byte min = time /60%60;
  return min;
}
void setTop()
{
  setTop(CHSV(hue,255,255));
}
void setTop(CHSV c)
{
  for(int i = 360; i < NUM_LEDS; i++)
  {
    leds[i] = c;
  }
}
void runHue()
{
  if(millis()-lastHue>1000)
  {
    lastHue = millis();
    hue++;
  }
}
void updateLEDs() 
{ 
  unsigned long time = millis();
  if(!running)
  {
    //disOhio();
    waiting();
    writeFast = true;
  }
  else
  {
    unsigned long timeRemaining = remainingTime();
    if(millis()-startAnimationTime<countUpTime && timeRemaining>=(3600-countUpTime/1000))//run fun animation but only if more than one hour remaining
    {
      writeFast = true;
      double percDone = ((double)(millis()-startAnimationTime))/countUpTime;
//        Serial.println(percDone);
      unsigned long max = (unsigned long)(countDownHours)*60*60+(unsigned long)(countDownMinutes)*60;
      writeHours(secondsToHour(max*percDone), CHSV(hue, 255,255));
      writeMinutes(secondsToMinutes(max*percDone), CHSV(hue++, 255,255));
      setTop();
    }
    else
    {
      runHue();
      if(timeRemaining>0)
      {
        byte hours = secondsToHour(timeRemaining);
        if(hours == 0)
        {
          byte minutes = secondsToMinutes(timeRemaining);
          if(minutes == 0)
          {
            writeHours(timeRemaining%60, CHSV(hue, 255,255));
            writeMinutes(millis()%100, CHSV(hue,  255,255));
            writeFast = true;
          }
          else
          {
            writeHours(secondsToMinutes(timeRemaining), CHSV(hue, 255,255));
            writeMinutes(timeRemaining%60, CHSV(hue,  255,255));
            writeFast = false;
          }
        }
        else
        {
          writeHours(secondsToHour(timeRemaining), CHSV(hue, 255,255));
          writeMinutes(secondsToMinutes(timeRemaining), CHSV(hue,  255,255));
          writeFast = false;
        }
        colonOn = timeRemaining%2;
        writeColon(colonOn, CHSV(hue, 255,255));
        setTop();
      }
      else
      {
        byte spot = (byte)((int)(millis()/3000.0*255)%255);
//        Serial.println(spot);
        if(spot<150)
        {
          FastLED.setBrightness(255);
        }
        else if(spot<160)
        {
          FastLED.setBrightness(255-(spot-160));
        }
        else
        {
          FastLED.setBrightness(0);
        }
        setTop();
        writeHours(secondsToHour(0), CHSV(hue, 255,255));
        writeMinutes(secondsToMinutes(0), CHSV(hue,  255,255));
        writeColon(false, CHSV(hue, 255,255));
        writeFast = true;
      }
    }
  }
  FastLED.show();
}
void setAll(byte r, byte g, byte b)
{
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(r,g,b);
  }
  FastLED.show();
}
void waiting()
{
  if(loopCount%2!=0)
  {
    return;
  }
  leds[(animationStep+5)%(NUM_LEDS-TOP_SIZE)] = CHSV(hue, 255, 255);
  leds[(animationStep+4)%(NUM_LEDS-TOP_SIZE)] = CHSV(hue, 255, 255*(4.0/5.0));
  leds[(animationStep+3)%(NUM_LEDS-TOP_SIZE)] = CHSV(hue, 255, 255*(3.0/5.0));
  leds[(animationStep+2)%(NUM_LEDS-TOP_SIZE)] = CHSV(hue, 255, 255*(2.0/5.0));
  leds[(animationStep+1)%(NUM_LEDS-TOP_SIZE)] = CHSV(hue++, 255, 255*(1.0/5.0));
  leds[animationStep] = 0;
  animationStep++;
  if(animationStep>=(NUM_LEDS-TOP_SIZE))
  {
    animationStep = 0;
  }
//  byte topPosition = animationStep2;
//  if(topPosition > TOP_SIZE)
//  {
//    topPosition = TOP_SIZE*2 - topPosition;
//  }
//  Serial.println(topPosition);
//  for(int i = 0; i < TOP_SIZE; i++)
//  {
//    if(abs(i-topPosition)<2)
//    {
//      leds[(NUM_LEDS-TOP_SIZE)+i] = CHSV(hue, 255, 255);
//    }
//    else if(abs(i-(TOP_SIZE-topPosition))<2)
//    {
//      leds[(NUM_LEDS-TOP_SIZE)+i] = CHSV(hue, 255, 255);
//    }
//    else
//    {
//      leds[(NUM_LEDS-TOP_SIZE)+i] = 0;
//    }
//  }
//  animationStep2++;
//  if(animationStep2>(TOP_SIZE*2))
//  {
//    animationStep2 = 0;
//  }
  for(int i = 0; i < TOP_SIZE; i++)
  {
    leds[(NUM_LEDS-TOP_SIZE)+i] = CHSV(hue, 255, 255);
  }
//  Serial.println(animationStep);
}
