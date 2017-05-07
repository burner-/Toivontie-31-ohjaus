
int lightRelays[6] = {CONTROLLINO_R10,CONTROLLINO_R11,CONTROLLINO_R12,CONTROLLINO_R13,CONTROLLINO_R14,CONTROLLINO_R15};
int lightAnalogs[6] = {CONTROLLINO_A0,CONTROLLINO_A1,CONTROLLINO_A2,CONTROLLINO_A3,CONTROLLINO_A4,CONTROLLINO_A5};

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

void lightsDim(byte dimValue){
  for (int i=0;i<6;i++){
    analogWrite(lightAnalogs[i], dimValue);
  }
}

void handleLights(){

  if(settings.lightDim < 255)
    lightsDim(settings.lightDim);
  
  if (settings.lightOn) lightsOn();
  else lightsOff();
}

