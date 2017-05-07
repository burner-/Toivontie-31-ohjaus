#define DEBUG_ESP_HTTP_SERVER
#include <EthernetWebServer.h> //https://github.com/burner-/EthernetWebServer
EthernetWebServer server(80);  // create a server at port 80

#define ORIGIN "*"

// convert a single hex digit character to its integer value (from https://code.google.com/p/avr-netino/)
unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}


String urldecode(String input) // (based on https://code.google.com/p/avr-netino/)
{
   char c;
   String ret = "";
   
   for(byte t=0;t<input.length();t++)
   {
     c = input[t];
     if (c == '+') c = ' ';
         if (c == '%') {


         t++;
         c = input[t];
         t++;
         c = (h2int(c) << 4) | h2int(input[t]);
     }
    
     ret.concat(c);
   }
   return ret;
  
}


int boolToInt(boolean bo)
{
  if (bo) return 1;
  else return 0;
}

void getArgumentValue(const char parmname[], boolean *target)
{
  
  if (server.hasArg(parmname))
  {
    
    String argVal = server.arg(parmname);
    if (argVal == "1" || argVal == "true")
    {
      DBG_OUTPUT_PORT.print(parmname);
      DBG_OUTPUT_PORT.println(" true");
      *target = true;
    }
    else 
    {
      DBG_OUTPUT_PORT.print(parmname);
      DBG_OUTPUT_PORT.println(" false");
      *target = false;
    }
  }
}
void getArgumentValue(const char parmname[], String *target)
{
  
  if (server.hasArg(parmname))
  {
    
    String argVal = server.arg(parmname);
    *target = urldecode(argVal);
  }
}


void getArgumentValue(const char parmname[], float *target)
{
  if (server.hasArg(parmname))
  {
    String argVal = server.arg(parmname);
    *target = argVal.toFloat();
  }
}

void getArgumentValue(const char parmname[], unsigned long *target)
{
  if (server.hasArg(parmname))
  {
    String argVal = server.arg(parmname);
    *target = argVal.toFloat();
  }
}
void getArgumentValue(const char parmname[], double *target)
{
  if (server.hasArg(parmname))
  {
    String argVal = server.arg(parmname);
    *target = argVal.toFloat();
  }
}

void getArgumentValue(const char parmname[], int *target)
{
  if (server.hasArg(parmname))
  {
    String argVal = server.arg(parmname);
    *target = argVal.toInt();
  }
}

bool setAddressConfigValue(const char parmname[], byte target[])
{
  if (server.hasArg(parmname))
  {
    String argVal = server.arg(parmname);
    byte addr[8];
    if (argVal.length() == 16)
    {
      argVal.toUpperCase();
      getAddressBytes(argVal, addr);
      copyByteArray(addr, target, 8);
      return true;
    }
    else
      DBG_OUTPUT_PORT.println(F("ignoring bogus sensor address"));
  }
  return false;
}



void jsonAddValue (String &string, String valuename, String value, bool newline)
{
  string += "\"" + valuename  + "\" : \"" + value + "\"" ;
  if (newline) string += ",\n";
}

void jsonAddValue (String &string, String valuename, float value, bool newline)
{
  string += "\"" + valuename  + "\" : \"" + (String) value + "\"" ;
  if (newline) string += ",\n";
}

void jsonAddValue (String &string, String valuename, double value, bool newline)
{
  string += "\"" + valuename  + "\" : \"" + (String) value + "\"" ;
  if (newline) string += ",\n";
}

void jsonAddValue (String &string, String valuename, int value, bool newline)
{
  string += "\"" + valuename  + "\" : \"" + (String) value + "\"" ;
  if (newline) string += ",\n";
}

void jsonAddValue (String &string, String valuename, unsigned long value, bool newline)
{
  string += "\"" + valuename  + "\" : \"" + (String) value + "\"" ;
  if (newline) string += ",\n";
}

void jsonAddValue (String &string, String valuename, bool value, bool newline)
{
  string += "\"" + valuename  + "\" : ";
  if (value)
    string += "true"; 
  else 
    string += "false";
  if (newline) string += ",\n";
}


int sendWebContent(String str)
{
  server.sendContent(str);
  return str.length();
}


void set_settings()
{
 // Get new values from client   
  getArgumentValue("lightOn", &settings.lightOn);
  getArgumentValue("DefaultAutoOff", &settings.DefaultAutoOff);
  getArgumentValue("lightDim", &settings.lightDim);
  bool save = false;
  getArgumentValue("save", &save);
  if(save)
    Serial.println("Saving settings");
  

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    if (server.argName(i) == "save") 
      saveConfig();
  }
  // send current values to client
  int exceptedSize = 1000;
  int sentSize = 0;
  server.setContentLength(exceptedSize);
  server.sendHeader("Cache-Control", F("no-transform,public,max-age=6,s-maxage=6"));
  server.sendHeader(F("Access-Control-Allow-Origin"), "*");
  server.send(200, "text/plain", "{");

  String values = "";
  jsonAddValue(values, F("lightOn"), settings.lightOn,true);
  jsonAddValue(values, F("lightDim"), settings.lightDim,true);
  jsonAddValue(values, F("DefaultAutoOff"), settings.DefaultAutoOff,false);
  
  sentSize += sendWebContent(values);
  values = "}";
  
  
  while(exceptedSize>sentSize){
    values += "\n";
    sentSize++;
  }
  server.sendContent(values);
}



void InitWebserver()
{
  server.on("/settings", set_settings);
  //server.on("/metrics", handle_prometheus);
  
  server.begin();
  DBG_OUTPUT_PORT.println(F("Webserver started"));
}





