void GPS_Check()
{
   while (GPS.available() > 0)
   if (gps.encode(GPS.read()))
   displayInfo();

   if (millis() > 5000 && gps.charsProcessed() < 10)
  {
   //  Serial.println(F("No GPS detected: check wiring."));
    // while(true);
  }
  else
  {
  }
}


void displayInfo()
{
  if(gps.location.isValid())
    {
      lat1=gps.location.lat();
      lng1=gps.location.lng();
    }
   else
    {
    }
}

void GSM_Call()
{
   Serial.println("at");
   delay(300);
   Serial.println("at");
   delay(300);
   Serial.println("atdPhone number;");
   delay(300);
}

void GSM_MSG_MODE_ACTIVE()
{
  //------------GSM RECIVE MODE-----------------//
   Serial.println("AT+CMGF=1"); // set SMS mode to text
   delay(300);
   Serial.println("AT+CMGF=1"); // set SMS mode to text
   delay(300);
   Serial.println("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt
   delay(300);
   //----------------------------------------//
}
void GSM_SENT(int x)
{
   Serial.println("AT");
   delay(300);
   Serial.println("AT");
   delay(300);
   if(x==2)
   {
      Serial.println("AT+CMGS=\"Phone number\"");
   }
   else if(x==1)
   {
      Serial.println("AT+CMGS=\"Phone number\"");
   }
   else if(x==3)
   {
      Serial.println("AT+CMGS=\"Phone number\"");
   }
   delay(300);

 //---------------------------------------//
   Serial.print(F("PLEASE HELP ME\n"));
   Serial.print(F(" http://www.google.com/maps/place/"));
   Serial.print(lat1, 6);
   Serial.print(F(","));
   Serial.print(lng1, 6);
//---------------------------------------//
   delay(300);
   Serial.write(0x1A);
   delay(300);
}
