int motionInput[1] = {CONTROLLINO_A0};
bool motionInputState[1] = { false };

void initMotionInputs(){
  
  for (int i=0;i<1;i++){
    pinMode(motionInput[i], INPUT);
  }
}
void doMotionLogic(){
  if (motionInputState[0] == true) settings.lightOn = true; // if motion in sensor A0 turn lights on

}
void readMotionSensors(){
  for (int i=0;i<1;i++){
    bool newstate = false;
    int pinstate = digitalRead(motionInput[i]);
    if (pinstate == LOW) // motion sensors are NC
    { 
      newstate = true;
      //Serial.println("t");
    } 
    //Serial.println("f");
      
    if (motionInputState[i] != newstate)
    {
      motionInputState[i] = newstate;
      if (newstate)
        logMsg(5,F("Motion detected"));
      Serial.print("Input state change A");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println( newstate );
      doMotionLogic();
    }
  }
}
