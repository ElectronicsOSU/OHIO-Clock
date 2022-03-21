void start()
{
  DateTime now = rtc.now();
  startWithTime(now.unixtime()+(unsigned long)(countDownHours)*60*60+(unsigned long)(countDownMinutes)*60);
  EEPROM_writelong(0, targetTime);
}
void startWithTime(unsigned long tt)
{
  targetTime = tt;
  startAnimationTime = millis();
  running = true;
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = 0;
  }
}
unsigned long remainingTime()
{
  DateTime now = rtc.now();
  unsigned long unixTimeNow = now.unixtime();
  if(unixTimeNow>targetTime)
  {
    return 0;
  }
  return targetTime-unixTimeNow;
}
void pause()
{
  running = false;
  if(remainingTime()>0)
  {
    DateTime now = rtc.now();
    unsigned long unixTimeNow = now.unixtime();
    unsigned long timeLeft = targetTime-unixTimeNow;
    countDownHours = (timeLeft/60/60);
    countDownMinutes = (timeLeft/60)%60;
    subMode = 0;
  }
}

//everything below copied from arduino form https://forum.arduino.cc/index.php?topic=507200.0
// read double word from EEPROM, give starting address
unsigned long EEPROM_readlong(int address)
{
   //use word read function for reading upper part
   unsigned long dword = EEPROM_readint(address);
   //shift read word up
   dword = dword << 16;
   // read lower word from EEPROM and OR it into double word
   dword = dword | EEPROM_readint(address+2);
   return dword;
}

//write word to EEPROM
void EEPROM_writeint(int address, int value)
{
   EEPROM.write(address,highByte(value));
   EEPROM.write(address+1 ,lowByte(value));
}
 
 //write long integer into EEPROM
 void EEPROM_writelong(int address, unsigned long value)
{
   //truncate upper part and write lower part into EEPROM
   EEPROM_writeint(address+2, word(value));
   //shift upper part down
   value = value >> 16;
   //truncate and write
   EEPROM_writeint(address, word(value));
}

unsigned int EEPROM_readint(int address)
{
   unsigned int word = word(EEPROM.read(address), EEPROM.read(address+1));
   return word;
}
