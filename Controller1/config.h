#include <EEPROM.h>

typedef struct
{
  char BOFmark = 'A';
  bool lightOn = false;
  unsigned long DefaultAutoOff = 28800; // 8h in seconds
  char EOFmark = 'E';
}  settingsType;

settingsType settings;


template <class T> int EEPROM_writeGeneric(int ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.update(ee++, *p++);
  return i;
}

template <class T> int EEPROM_readGeneric(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}

bool saveConfig() {
  
  DBG_OUTPUT_PORT.print(F("Writing config to memory "));
  settings.BOFmark = 'A';
  settings.EOFmark = 'E';
  int count = EEPROM_writeGeneric(1, settings);
  DBG_OUTPUT_PORT.print(count);
  DBG_OUTPUT_PORT.println(F("Bytes writed"));
}
bool loadConfig() {
  int count = EEPROM_readGeneric(1, settings);
  DBG_OUTPUT_PORT.print(F("Bytes read: "));
  DBG_OUTPUT_PORT.println(count);
  if (settings.BOFmark == 'A' && settings.EOFmark == 'E')
  {
    DBG_OUTPUT_PORT.println(F("Configuration found"));
  }
  else 
  {
    DBG_OUTPUT_PORT.print(F("Cannot find configuration!"));
    return false;
  }
}
