#define LEAP_YEAR(Y) (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

EthernetUDP ntpUdp;

char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(char* address) {
  // set all bytes in the buffer to 0
  //memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ntpUdp.beginPacket(address, 123); //NTP requests are to port 123
  ntpUdp.write(packetBuffer, NTP_PACKET_SIZE);
  ntpUdp.endPacket();
}

void getNTPTime(){
    ntpUdp.begin(123);

    sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(500);
  if (ntpUdp.parsePacket()) {
    // We've received a packet, read the data from it
    ntpUdp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
    // easy ones
    unsigned char aHour = (unsigned char)((epoch  % 86400L) / 3600); // hour (86400 equals secs per day)
    unsigned char aMinute = (unsigned char)((epoch  % 3600) / 60);
    unsigned char aSecond = (unsigned char)(epoch % 60);
    unsigned char aWeekDay = ((epoch + 4) % 7);
    
    //complicated conversions
    int year = -1;
    int month = -1;
    year = 0;
    
    unsigned long days = 0;
    
    epoch /= 86400L; //Convert epoch to day calculation mode
    
    while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch)
    {
      year++;
    }
    year += 1970 - 2000; //f* two digit rtc
    unsigned char aYear = (unsigned char)year;
  
    
    days -= LEAP_YEAR(year) ? 366 : 365; // get how many days at this year
    
    epoch -= days;
    
    days = 0;
    month = 0;
    int monthLength = 0;
    uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0
    for (month=0; month<12; month++) {
        if (month==1) { // february
          if (LEAP_YEAR(year)) {
            monthLength=29;
          } else {
            monthLength=28;
          }
        } else {
          monthLength = monthDays[month];
        }
        
        if (epoch >= monthLength) {
          epoch -= monthLength;
        } else {
            break;
        }
    }
    month += 1;
    unsigned char aDay = (unsigned char)(epoch + 1);
    unsigned char aMonth = (unsigned char)month;
    
    Serial.print(aDay);
    Serial.print(".");
    Serial.print(aMonth);
    Serial.print(".");
    Serial.print(aYear);
    Serial.print(" ");
    Serial.print(aHour);
    Serial.print(":");
    Serial.print(aMinute);
    Serial.print(":");
    Serial.print(aSecond);
    Serial.println(" <");
    char ret = Controllino_SetTimeDate(aDay, aWeekDay,aMonth, aYear, aHour, aMinute, aSecond);
    Serial.println((int)ret);
  }
  else 
    Serial.println("No response from ntp");

}


