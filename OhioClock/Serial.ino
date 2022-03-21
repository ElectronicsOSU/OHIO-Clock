//char command[8];
//byte position = 0;
//void checkSerial()
//{
////  Serial.println(Serial.available());
//  while(Serial.available()>0)
//  {
//    char c = Serial.read();
//    if(c=='\n' || position > 7)
//    {
//      parse();
//      position = 0;
//      for(byte i = 0; i < 8; i++)
//      {
//        command[i] = 0;
//      }
//    }
//    else
//    {
//      command[position] = c;
//      position++;
//    }
//    Serial.print(F("C: "));
//    Serial.print(c);
//    Serial.print(F("\t"));
//    Serial.println(command);
//  }
//}
//void parse()
//{
//  if(controlled)
//  {
//    if(command[0]=='s')//start
//    {
//      start();
//      Serial.println("s");
//    }
//    else if(command[0]=='g')//give control
//    {
//      controlled = false;
//      Serial.println("s");
//    }
//    else if(command[0]=='h')//set hours
//    {
//      if(!running)
//      {
////        countDownHours = command.substring(1).toInt();
//        Serial.println(countDownHours);
//      }
//      else
//      {
//        Serial.println("f");
//      }
//    }
//    else if(command[0]=='m')//set minutes
//    {
//      if(!running)
//      {
////        countDownMinutes = command.substring(1).toInt();
//        Serial.println(countDownMinutes);
//      }
//      else
//      {
//        Serial.println("f");
//      }
//    }
//    else if(command[0]=='s')//start the timer
//    {
//      running = true;
//      Serial.println("s");
//    }
//    else if(command[0]=='p')//pause the time
//    {
//      pause();
//      Serial.println("s");
//    }
//    else
//    {
//      Serial.println("f");
//    }
//  }
//    else if(command[0]=='c')//take control
//  {
//    controlled = true;
//    Serial.println("s");
//  }
//  else
//  {
//    Serial.println("f");
//  }
//}
