void clicked()
{
  if(mode==0 && !running)
  {
    //start();
  }
  else if(mode== 1)
  {
    if(subMode==0)
    {
      mode = 0;
    }
  }
  else if(mode == 2)
  {
    mode = 1;
  }
  else if(mode == 3)
  {
    mode = 1;
  }
  if(writeFast)
  {
    delay(80);//just to make sure to have debounce
  }
}
void held()
{
  if(mode==0)
  {
    mode = 1;
  }
  else if(mode == 1)
  {
    if(!running)
    {
      if(subMode == 0)
      {
        //mode = 0;
      }
      else if(subMode == -1)
      {
        mode = 2;
      }
      else if(subMode == -2)
      {
        mode = 3;
      }
    }
    else
    {
      if(subMode == 0)
      {
        //mode = 0;
      }
      else if(subMode == -1)
      {
        pause();
      }
      else if(subMode == -2)
      {
        subMode = 0;
        mode = 0;
      }
    }
  }
}
void up()
{
  if(mode == 2)
  {
    countDownHours++;
  }
  else if(mode == 3)
  {
    countDownMinutes++;
    if(countDownMinutes>59)
    {
      countDownMinutes = 0;
    }
  }
}
void down()
{
  if(mode == 2)
  {
    countDownHours = max(0, countDownHours-1);
  }
  else if(mode == 3)
  {
    countDownMinutes--;
    if(countDownMinutes<0)
    {
      countDownMinutes = 59;
    }
  }
}
void left()
{
  if(mode==1 && subMode>-2)
  {
    subMode--;
  }
}
void right()
{
  if(mode==1 && subMode<0)
  {
    subMode++;
  }
}
