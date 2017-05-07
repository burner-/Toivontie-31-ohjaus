
int lightRelays[6] = {CONTROLLINO_R10,CONTROLLINO_R11,CONTROLLINO_R12,CONTROLLINO_R13,CONTROLLINO_R14,CONTROLLINO_R15};
int lightAnalogs[10] = {CONTROLLINO_D1,CONTROLLINO_D2,CONTROLLINO_D3,CONTROLLINO_D4,CONTROLLINO_D5,CONTROLLINO_D6,CONTROLLINO_D7,CONTROLLINO_D8,CONTROLLINO_D9,CONTROLLINO_D10};

void initLights(){
  for (int i=0;i<sizeof(lightAnalogs);i++){
   
    pinMode(lightAnalogs[i], OUTPUT);
  }
  for (int i=0;i<sizeof(lightRelays);i++){
    pinMode(lightRelays[i], OUTPUT);
   
  }
}
void lightsOn(){
  for (int i=0;i<sizeof(lightRelays);i++){
    digitalWrite(lightRelays[i], HIGH);
  }
}

void lightsOff(){
  for (int i=0;i<sizeof(lightRelays);i++){
    digitalWrite(lightRelays[i], LOW);
  }
}

void lightsDim(int dimValue){
  for (int i=0;i<sizeof(lightAnalogs);i++){
    analogWrite(lightAnalogs[i], dimValue * 2.5);
  }
}

void handleLights(){

  if(settings.lightDim >= 0 && settings.lightDim <= 100)
    lightsDim(settings.lightDim);
  
  if (settings.lightOn) lightsOn();
  else lightsOff();
}

