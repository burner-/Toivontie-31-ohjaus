void printRTCTime()
{
 unsigned char aDay =0;
 unsigned char aWeekDay =0;
 unsigned char aMonth =0;
 unsigned char aYear=0;
 unsigned char aHour=0;
 unsigned char aMinute=0;
 unsigned char aSecond = 0;

  Controllino_ReadTimeDate(&aDay, &aWeekDay, &aMonth, &aYear,  &aHour, &aMinute, &aSecond);
  
  Serial.print(aDay);
  Serial.print(".");
  Serial.print(aMonth);
  Serial.print(".");
  Serial.print(aYear + 2000);
  Serial.print(" ");
  Serial.print(aHour);
  Serial.print(":");
  Serial.print(aMinute);
  Serial.print(":");
  Serial.println(aSecond);
  
}

