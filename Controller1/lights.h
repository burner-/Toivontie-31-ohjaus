
int lightRelays[6] = {CONTROLLINO_R10,CONTROLLINO_R11,CONTROLLINO_R12,CONTROLLINO_R13,CONTROLLINO_R14,CONTROLLINO_R15};

void initLights(){
  for (int i=0;i<6;i++){
    pinMode(lightRelays[i], OUTPUT);
  }
}
void lightsOn(){
  for (int i=0;i<6;i++){
    digitalWrite(lightRelays[i], HIGH);
  }
}

void lightsOff(){
  for (int i=0;i<6;i++){
    digitalWrite(lightRelays[i], LOW);
  }
}

void handleLights(){
  if (settings.lightOn) lightsOn();
  else lightsOff(); 
}

