/*
 CTM_base.cpp - Library for continuous T-maze base functions
 Created by Mateo Umagung, May 31, 2023.
 Authorized for Wilke Lab use only.
 */

#include "Arduino.h"
#include "CTM_base.h"

CTM_base::CTM_base() {  

}

void CTM_base::configureParams(int HRTime, int LRTime,
                         int barrierRHeight, int barrierLHeight,
                         float prob_HR, float prob_LR,
                         int HRside, int ITI, int delayTime,
                         int nForceTrials, int ftSide) {
  // HIGH REWARD = 1
  // LOW REWARD = 0

  _HRTime = HRTime;
  _LRTime = LRTime;
  _bRH = barrierRHeight;
  _bLH = barrierLHeight;
  _HRside = HRside;
  _prob_HR = prob_HR;
  _prob_LR = prob_LR;
  _ITI = ITI;
  _delayTime = delayTime;
  _nForceTrials = nForceTrials;
  _ftSide = ftSide;

  // HRside: R is 1, L is 0  
  if (HRside == 1) {
    _rightReward = 1; // higher reward
    _leftReward = 0; // lower reward
  } else if (HRside == 0) {
    _rightReward = 0; // lower reward
    _leftReward = 1; // higher reward
  }
}

void CTM_base::begin() {
  Serial.begin(9600);
  
  delay(1500);
  
  // designate pins
  pinMode(outputD1, OUTPUT);
  pinMode(outputD2, OUTPUT);
  pinMode(outputD3, OUTPUT);
  pinMode(outputD4, OUTPUT);
  pinMode(outputD5, OUTPUT);
  
  pinMode(barrierR, OUTPUT);
  pinMode(barrierRpwm, OUTPUT);
  
  pinMode(barrierL, OUTPUT);
  pinMode(barrierLpwm, OUTPUT);

  pinMode(pump1Output, OUTPUT);
  pinMode(pump2Output, OUTPUT);

  pinMode(PIR_Start, INPUT);
  pinMode(PIR_Midstem, INPUT);
  pinMode(PIR_RightPostVertex, INPUT);
  pinMode(PIR_RightPreBarrier, INPUT);
  pinMode(PIR_LeftPostVertex, INPUT);
  pinMode(PIR_LeftPreBarrier, INPUT);
  pinMode(PIR_RightStartBox, INPUT);
  pinMode(PIR_LeftStartBox, INPUT);

  // turn on internal pull-up resistors
  digitalWrite(PIR_Start, HIGH);
  digitalWrite(PIR_Midstem, HIGH);
  digitalWrite(PIR_RightPostVertex, HIGH);
  digitalWrite(PIR_RightPreBarrier, HIGH);
  digitalWrite(PIR_LeftPostVertex, HIGH);
  digitalWrite(PIR_LeftPreBarrier, HIGH);
  digitalWrite(PIR_RightStartBox, HIGH);
  digitalWrite(PIR_LeftStartBox, HIGH);

  // Setup input pins from the buttons
  pinMode(startDoorButton, INPUT_PULLUP);
  pinMode(backRightDoorButton, INPUT_PULLUP);
  pinMode(backLeftDoorButton, INPUT_PULLUP);
  pinMode(frontRightDoorButton, INPUT_PULLUP);
  pinMode(frontLeftDoorButton, INPUT_PULLUP);

  pinMode(rightPumpButton, INPUT_PULLUP);
  pinMode(leftPumpButton, INPUT_PULLUP);

  pinMode(rightBarrierButton, INPUT_PULLUP);
  pinMode(leftBarrierButton, INPUT_PULLUP);

  
  resetBarriers();
  configureBarriers();

  digitalWrite(outputD2, HIGH); // Open D2
  isD2Open = true;
  digitalWrite(outputD3, HIGH); // Open D3
  isD3Open = true;

  delay(500);

  if (_nForceTrials > 0)
    ftActive = true;

  Serial.print("Setup complete\n");
  startTimer();
}

void CTM_base::printConfigParams() {
  if (_HRside == 1) 
    Serial.println("High Reward Side: Right");
  else if(_HRside == 0)
    Serial.println("High Reward Side: Left");
  else 
    Serial.println("oops");

  Serial.print("Left Barrier Height: ");
  Serial.print(_bLH);
  Serial.print("\n");

  Serial.print("Right Barrier Height: ");
  Serial.print(_bRH);
  Serial.print(" cm\n");

  Serial.print("High Reward Time: ");
  Serial.print(_HRTime);
  Serial.print(" ms \n");

  Serial.print("Low Reward Time: ");
  Serial.print(_LRTime);
  Serial.print(" ms \n");

  Serial.print("HR Reward Probability on HR Side: ");
  Serial.print(_prob_HR);
  Serial.print("\n");

  Serial.print("HR Reward Probability on LR Side: ");
  Serial.print(_prob_LR);
  Serial.print("\n");
}

void CTM_base::resetBarriers() {
  digitalWrite(barrierR, HIGH); 
  for (int i = 0; i < 255; i++) {
    analogWrite(barrierRpwm, i);
    delay(8);
  }
  delay(2000);
  analogWrite(barrierRpwm, 0);

  digitalWrite(barrierL, HIGH); 
  for (int i = 0; i < 255; i++) {
    analogWrite(barrierLpwm, i);
    delay(8);
  }
  delay(2000);
  analogWrite(barrierLpwm, 0);
}

void CTM_base::configureBarriers() {
  // Right barrier
  _converted_bRH = 143 * _bRH + 10.2; // Relationship between time to raise and height for the right side
  if (_converted_bRH < 0){
    _converted_bRH = 0;
    }
  if(_converted_bRH > 2296){ // Max height that right barrier could go (16 cm)
    _converted_bRH = 2296;
  }

  if(_converted_bRH != 0){
    digitalWrite(barrierR, LOW);
    analogWrite(barrierRpwm, 255);
    delay(_converted_bRH); 
    analogWrite(barrierRpwm, 0);
  }


  // Left Barrier
  _converted_bLH = 122 * _bLH - 7.78; // Relationship between time to raise and height for the left side
  if (_converted_bLH < 0){
    _converted_bLH = 0;
  }
  if(_converted_bLH > 1943){  // Max height that left barrier could go (16 cm)
    _converted_bLH = 1943;
  }

  if(_converted_bLH != 0){
    
    digitalWrite(barrierL, LOW);
    analogWrite(barrierLpwm, 255);
    delay(_converted_bLH); 
    analogWrite(barrierLpwm, 0);
  }



  // Testing code
  // for(int testTime = 100; testTime < 2800; testTime){
  //   Serial.print("Testing: ");
  //   Serial.print(testTime);
  //   Serial.print(" ms\n");

  //   digitalWrite(barrierL, LOW);
  //   analogWrite(barrierLpwm, 255);
  //   delay(testTime);
    
  //   analogWrite(barrierLpwm, 0);
  //   delay(8000); // Wait for measurement
  //   digitalWrite(barrierL, HIGH);
  //   analogWrite(barrierLpwm, 255);
  //   delay(3000); 
  //   analogWrite(barrierLpwm, 0);
  //   delay(3000); 
  //   testTime = testTime + 100;

  // }
}

void CTM_base::startTimer() {
  startTime = millis();
  Serial.print("Start time: ");
  Serial.print(startTime);
  Serial.print("\n");
}

void CTM_base::ActivatePIRStart() {
  if (digitalRead(PIR_Start) && PIRStartPrimed && !PIRStartActivated) { // Check for activated sensor AND that trial is available AND PIR is not already activated
    currentTime = millis();

    if (!ftActive) {
      Serial.print("Current Trial: ");
      Serial.print(currTrial);
      Serial.print("\n");
    } else {
      Serial.print("Force Trial: ");
      Serial.print(currFt);
      Serial.print("\n");
    }
    
    Serial.print("PIR Start Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    PIRStartPrimed = false; // Set trial active to FALSE so this can't get called again until the trial resets
    PIRStartActivated = true; // Set PIR Active to TRUE
    
    PIRStartOpenD1D2D3(); // Call function to open doors 1, 2, and 3
  }
}

void CTM_base::ActivatePIRMidstem() {
  if (digitalRead(PIR_Midstem) && PIRMidstemPrimed && !PIRMidstemActivated) { // Check for activated midstem sensor AND that midstem sensor is primed AND that is is not already activated
    currentTime = millis();
    Serial.print("PIR Midstem Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    PIRMidstemPrimed = false; // Un-prime PIRMidstem
    PIRMidstemActivated = true; 

    PIREndstemPrimed = true;
    
    //PIRMidstemCloseD1();
  }
}

void CTM_base::ActivatePIREndstem() {
  if (digitalRead(PIR_Endstem) && PIREndstemPrimed && !PIREndstemActivated) {
    currentTime = millis();
    Serial.print("PIR Endstem Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    PIREndstemPrimed = false;
    PIREndstemActivated = true;
  
    PIRLeftPostVertexPrimed = true; // Prime post LPV sensor
    PIRRightPostVertexPrimed = true; // Prime post-RPV sensor
  }
}

void CTM_base::ActivatePIRLeftPostVertex() {
  if (digitalRead(PIR_LeftPostVertex) && PIRLeftPostVertexPrimed && !PIRLeftPostVertexActivated) {
    currentTime = millis();
    Serial.print("PIR LPV Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
   
    PIRLeftPostVertexActivated = true;
  
    PIRLeftPreBarrierPrimed = true;
    // PIRRightPreBarrierPrimed = false;
  }
}

void CTM_base::ActivatePIRRightPostVertex() {
  if (digitalRead(PIR_RightPostVertex) && PIRRightPostVertexPrimed && !PIRRightPostVertexActivated) {
    currentTime = millis();
    Serial.print("PIR RPV Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
  
    PIRRightPostVertexActivated = true;
  
    // PIRLeftPreBarrierPrimed = false;
    PIRRightPreBarrierPrimed = true;
  }
}

void CTM_base::ActivatePIRLeftPreBarrier() {
  if (digitalRead(PIR_LeftPreBarrier) && PIRLeftPreBarrierPrimed && !PIRLeftPreBarrierActivated) {
    currentTime = millis();
    Serial.print("PIR LPB Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    // PIRLeftPreBarrierPrimed = false; // Un-prime PIRLPB
    // PIRRightPreBarrierPrimed = false;
    PIRLeftPreBarrierActivated = true;
    
    PIRLPBCloseD2D3D5OpenD4();
  }
}

void CTM_base::ActivatePIRRightPreBarrier() {
  if (digitalRead(PIR_RightPreBarrier) && PIRRightPreBarrierPrimed && !PIRRightPreBarrierActivated) {
    currentTime = millis();
    Serial.print("PIR RPB Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    // PIRRightPreBarrierPrimed = false; // Un-prime PIRRPB
    // PIRLeftPreBarrierPrimed = false;
    
    PIRRightPreBarrierActivated = true;
    
    PIRRPBCloseD3D2D4OpenD5();
  }
}

void CTM_base::ActivatePIRLeftStartBox() {
  if (digitalRead(PIR_LeftStartBox) && PIRLeftStartBoxPrimed && !PIRLeftStartBoxActivated) {
    currentTime = millis();
    Serial.print("PIR LSB Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    PIRLeftStartBoxPrimed = false;
    PIRLeftStartBoxActivated = true;
    
    PIRLSBCloseD4(); // Close D4 function
  }
}

void CTM_base::ActivatePIRRightStartBox() {
  if (digitalRead(PIR_RightStartBox) && PIRRightStartBoxPrimed && !PIRRightStartBoxActivated) {
    currentTime = millis();
    Serial.print("PIR RSB Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    PIRRightStartBoxPrimed = false;
    PIRRightStartBoxActivated = true;
    
    PIRRSBCloseD5(); // Close D5 function
  }
}

void CTM_base::checkSensors() {
  ActivatePIRStart();
  ActivatePIRMidstem();
  ActivatePIREndstem();
  ActivatePIRLeftPostVertex();
  ActivatePIRRightPostVertex();
  ActivatePIRLeftPreBarrier();
  ActivatePIRRightPreBarrier();
  ActivatePIRLeftStartBox();
  ActivatePIRRightStartBox();
}

void CTM_base::resetFlags() {
  PIRStartPrimed = true; //set trial available to start
  PIRStartActivated = false;
  PIRMidstemPrimed = false;
  PIRMidstemActivated = false;
  PIREndstemPrimed = false;
  PIREndstemActivated = false;
  PIRLeftPostVertexPrimed = false;
  PIRLeftPostVertexActivated = false;
  PIRRightPostVertexPrimed = false;
  PIRRightPostVertexActivated = false;
  PIRLeftPreBarrierPrimed = false;
  PIRLeftPreBarrierActivated = false;
  PIRRightPreBarrierPrimed = false;
  PIRRightPreBarrierActivated = false;
  PIRLeftStartBoxPrimed = false;
  PIRLeftStartBoxActivated = false;
  PIRRightStartBoxPrimed = false;
  PIRRightStartBoxActivated = false;

  Serial.println("Flags Reset");

  if (currFt <= _nForceTrials) {
    currFt++;
    if (currFt > _nForceTrials) {
      ftActive = false;
    }
  } else {
    Serial.print("Trial Completed: ");
    Serial.println(currTrial);
    currTrial++;
  }

  if (ftActive) {
    enactForceTrials();
  }
}

void CTM_base::activatePump(int rew, int pump, float prob_HR, float prob_LR) {
  // sends a pulse of a given duration to the reward board to activate pump
  
  int pumpOP;
  int durationHP; // duration with higher probability
  int durationLP; // duration with lower probability
  int chosenDuration;
  int prob;
  durationHP = _HRTime;
  durationLP = _LRTime;

  /* choose the probability of the duration according to which type of reward is being dispensed */
  if (rew == 1) {
    // durationHP = _HRTime;
    // durationLP = _LRTime;
    prob = prob_HR * 100;
    Serial.println("HR Side Was Chosen");
  }

  else if (rew == 0){
    // durationHP = _LRTime;
    // durationLP = _HRTime;
    prob = prob_LR * 100;
    Serial.println("LR Side Was Chosen");
  }

  // pick reward with prob% chance
  long probPicker = random(0, 101);
  probPicker = float(probPicker);

  if (probPicker <= prob) {
    chosenDuration = durationHP;
    // Serial.println("Higher Probability");
  } else {
    chosenDuration = durationLP;
    // Serial.println("Lower Probability");
  }




  if(chosenDuration == durationHP && durationHP>=durationLP){
    Serial.print("Large Reward: ");
    Serial.print(chosenDuration);
    Serial.print(" ms\n");
  }
  else if(chosenDuration == durationLP && durationLP>=durationHP){
    Serial.print("Large Reward: ");
    Serial.print(chosenDuration);
    Serial.print(" ms\n");
  }

  else if(chosenDuration == durationHP && durationHP<=durationLP){
    Serial.print("Small Reward: ");
    Serial.print(chosenDuration);
    Serial.print(" ms\n");
  }
  else if(chosenDuration == durationLP && durationLP<=durationHP){
    Serial.print("Small Reward: ");
    Serial.print(chosenDuration);
    Serial.print(" ms\n");
  }
  
  // Serial.print("Chosen duration: ");
  // Serial.print(chosenDuration);
  // Serial.print("\n");

  // choose pump
  if (pump == 1)
    pumpOP = pump1Output;
  
  else if (pump == 0) 
    pumpOP = pump2Output;

  digitalWrite(pumpOP, HIGH);
  delay(chosenDuration);
  digitalWrite(pumpOP, LOW);
  delay(20);
}

void CTM_base::testPump() {
  // for(int i = 0; i<=10000; i+=500){
  //   Serial.print("Testing Pump 1: ");
  //   Serial.print(i);
  //   Serial.print(" ms\n");
  //   digitalWrite(pump1Output, HIGH);
  //   delay(i);
  //   digitalWrite(pump1Output, LOW);
  //   delay(30000);
  // }
  // for(int i = 0; i<=10000; i+=500){
  //   Serial.print("Testing Pump 2: ");
  //   Serial.print(i);
  //   Serial.print(" ms\n");
  //   digitalWrite(pump2Output, HIGH);
  //   delay(i);
  //   digitalWrite(pump2Output, LOW);
  //   delay(30000);
  // }
  Serial.print("Testing Pump 2: ");
    Serial.print("5000");
    Serial.print(" ms\n");
    digitalWrite(pump2Output, HIGH);
    delay(5000);
    digitalWrite(pump2Output, LOW);
    delay(30000);

    Serial.print("Testing Pump 2: ");
    Serial.print("8000");
    Serial.print(" ms\n");
    digitalWrite(pump2Output, HIGH);
    delay(8000);
    digitalWrite(pump2Output, LOW);
    delay(30000);

    Serial.print("Testing Pump 2: ");
    Serial.print("10000");
    Serial.print(" ms\n");
    digitalWrite(pump2Output, HIGH);
    delay(10000);
    digitalWrite(pump2Output, LOW);
    delay(30000);
}

void CTM_base::PIRStartOpenD1D2D3() {
  digitalWrite(outputD1, HIGH); // Open D1
  isD1Open = true;
  //digitalWrite(outputD2, HIGH); // Open D2
  //digitalWrite(outputD3, HIGH); // Open D3


  
  delay(100);
  PIRMidstemPrimed = true; // Prime Midstem sensor
}

void CTM_base::PIRMidstemCloseD1() {
  delay(100);
  PIREndstemPrimed = true; // Prime Endstem
  
  // Closes the Midstem door after a specified delay
  delay(_delayTime);
  digitalWrite(outputD1, LOW); // Close D1
  isD1Open = false;
}

void CTM_base::PIRLPBCloseD2D3D5OpenD4() {
  activatePump(_leftReward, 0, _prob_HR, _prob_LR);
  delay(_delayTime);
  digitalWrite(outputD2, LOW); // Close D2
  digitalWrite(outputD3, LOW); // Close D3
  digitalWrite(outputD5, LOW); // Close D5
  digitalWrite(outputD1, LOW); // Close D1
  isD2Open = false;
  isD3Open = false;
  isD5Open = false;
  isD1Open = false;

  digitalWrite(outputD4, HIGH); // Open D4
  isD4Open = true;
  
  delay(100);
  PIRLeftStartBoxPrimed = true; // Prime LSB sensor  
}

void CTM_base::PIRRPBCloseD3D2D4OpenD5() {
  activatePump(_rightReward, 1, _prob_HR, _prob_LR);
  delay(_delayTime);
  digitalWrite(outputD2, LOW); // Close D2
  digitalWrite(outputD3, LOW); // Close D3
  digitalWrite(outputD4, LOW); // Close D4
  digitalWrite(outputD1, LOW); // Close D1
  isD2Open = false;
  isD3Open = false;
  isD4Open = false;
  isD1Open = false;

  digitalWrite(outputD5, HIGH); // Open D5
  isD5Open = true;
  

  delay(100);
  PIRRightStartBoxPrimed = true;
}

void CTM_base::PIRLSBCloseD4() {
  delay(_delayTime);
  digitalWrite(outputD4, LOW); // Close D4
  isD4Open = false;

  digitalWrite(outputD2, HIGH);
  digitalWrite(outputD3, HIGH);
  isD2Open = true;
  isD3Open = true;

  delay(_ITI);
  resetFlags();
}

void CTM_base::PIRRSBCloseD5() { 
  delay(_delayTime);
  digitalWrite(outputD5, LOW); // Close D5
  isD5Open = false;
  
  digitalWrite(outputD2, HIGH);
  digitalWrite(outputD3, HIGH);
  isD2Open = true;
  isD3Open = true;

  delay(_ITI);
  resetFlags();
}



void CTM_base::buttonD1Pressed(){
  Serial.print("START DOOR BUTTON PRESSED");

  // If the door is open, close it.
  if(isD1Open == true){
    Serial.print(" - Closing Door\n");
    digitalWrite(outputD1, LOW);
    isD1Open = false;
    delay(1000);

  }else if(isD1Open == false){
    // Otherwise, open it.
    Serial.print(" - Opening Door\n");
    digitalWrite(outputD1, HIGH);
    isD1Open = true;
    delay(1000);
  }
}

void CTM_base::buttonD2Pressed(){
  Serial.print("BACK LEFT DOOR BUTTON PRESSED");

  // If the door is open, close it.
  if(isD2Open == true){
    Serial.print(" - Closing Door\n");
    digitalWrite(outputD2, LOW);
    isD2Open = false;
    delay(1000);

  }else if(isD2Open == false){
    // Otherwise, open it.
    Serial.print(" - Opening Door\n");
    digitalWrite(outputD2, HIGH);
    isD2Open = true;
    delay(1000);

  }
  // PIRStartPrimed = false;
  // PIRStartActivated = false;
  // PIRMidstemPrimed = false;
  // PIRMidstemActivated = false;
  // PIREndstemPrimed = false;
  // PIREndstemActivated = false;
  // PIRLeftPostVertexPrimed = false;
  // PIRLeftPostVertexActivated = false;
  // PIRRightPostVertexPrimed = false;
  // PIRRightPostVertexActivated = false;
  // PIRLeftPreBarrierPrimed = false;
  // PIRLeftPreBarrierActivated = false;
  // PIRRightPreBarrierPrimed = false;
  // PIRRightPreBarrierActivated = false;
  // PIRLeftStartBoxPrimed = true;
  // PIRLeftStartBoxActivated = false;
  // PIRRightStartBoxPrimed = true;
  // PIRRightStartBoxActivated = false;
  // Serial.print("MANUAL MODE ENGAGED ");
  // Serial.print("SENSORS DISREGARDED FOR CURRENT TRIAL\n");
}

void CTM_base::buttonD3Pressed(){
  Serial.print("BACK RIGHT DOOR BUTTON PRESSED");
  
  // If the door is open, close it.
  if(isD3Open == true){
    Serial.print(" - Closing Door\n");
    digitalWrite(outputD3, LOW);
    isD3Open = false;
    delay(1000);

  }else if(isD3Open == false){
    // Otherwise, open it.
    Serial.print(" - Opening Door\n");
    digitalWrite(outputD3, HIGH);
    isD3Open = true;
    delay(1000);

  }
}

void CTM_base::buttonD4Pressed(){
  Serial.print("FRONT LEFT DOOR BUTTON PRESSED");

  // If the door is open, close it.
  if(isD4Open == true){
    Serial.print(" - Closing Door\n");
    digitalWrite(outputD4, LOW);
    isD4Open = false;
    delay(1000);

  }else if(isD4Open == false){
    // Otherwise, open it.
    Serial.print(" - Opening Door\n");
    digitalWrite(outputD4, HIGH);
    isD4Open = true;
    delay(1000);

  }

}

void CTM_base::buttonD5Pressed(){
  Serial.print("FRONT RIGHT DOOR BUTTON PRESSED");

  // If the door is open, close it.
  if(isD5Open == true){
    Serial.print(" - Closing Door\n");
    digitalWrite(outputD5, LOW);
    isD5Open = false;
    delay(1000);

  }else if(isD5Open == false){
    // Otherwise, open it.
    Serial.print(" - Opening Door\n");
    digitalWrite(outputD5, HIGH);
    isD5Open = true;
    delay(1000);

  }

}



  // // Right barrier
  // _converted_bRH = 143 * _bRH + 10.2; // Relationship between time to raise and height for the right side
  // if (_converted_bRH < 0){
  //   _converted_bRH = 0;
  //   }
  // if(_converted_bRH > 2296){ // Max height that right barrier could go (16 cm)
  //   _converted_bRH = 2296;
  // }

  // if(_converted_bRH != 0){
  //   digitalWrite(barrierR, LOW);
  //   analogWrite(barrierRpwm, 255);
  //   delay(_converted_bRH); 
  //   analogWrite(barrierRpwm, 0);
  // }


  // // Left Barrier
  // _converted_bLH = 122 * _bLH - 7.78; // Relationship between time to raise and height for the left side
  // if (_converted_bLH < 0){
  //   _converted_bLH = 0;
  // }
  // if(_converted_bLH > 1943){  // Max height that left barrier could go (16 cm)
  //   _converted_bLH = 1943;
  // }

void CTM_base::buttonRBarrierPressed(){
  unsigned long beginTime = millis();
  unsigned long timeRan = 0;

  if(_converted_bRH < 2296){
    Serial.print("RAISING RIGHT BARRIER\n");
    digitalWrite(barrierR, LOW);
    analogWrite(barrierRpwm, 255);
    while(digitalRead(rightBarrierButton) == LOW){
    }
    analogWrite(barrierRpwm, 0);
    timeRan = millis() - beginTime;

    if(_converted_bRH + timeRan > 2296){
      _converted_bRH = 2296;
    }else{
      _converted_bRH = _converted_bRH + timeRan;
    }
  }
  else if(_converted_bRH >= 2296){
    Serial.print("LOWERING RIGHT BARRIER\n");
    digitalWrite(barrierR, HIGH);
    analogWrite(barrierRpwm, 255);
    while(digitalRead(rightBarrierButton) == LOW){
    }
    analogWrite(barrierRpwm, 0);
    timeRan = millis() - beginTime;

    if(2296 - timeRan < 0){
      _converted_bRH = 0;
    }else{
      _converted_bRH = 2296 - timeRan;
    }
  }
  // float approxHeight = (_converted_bRH - 10.2) / 143;
  // if(approxHeight > 16){
  //   approxHeight = 16;
  // }if(approxHeight < 0){
  //   approxHeight = 0;
  // }

  // Serial.print("Approximate Height of Right Barrier: ");
  // Serial.print(approxHeight);
  // Serial.print("cm\n");


}

void CTM_base::buttonLBarrierPressed(){
  unsigned long beginTime = millis();
  unsigned long timeRan = 0;

  if(_converted_bLH < 1943){
    Serial.print("RAISING LEFT BARRIER\n");
    digitalWrite(barrierL, LOW);
    analogWrite(barrierLpwm, 255);
    while(digitalRead(leftBarrierButton) == LOW){
    }
    analogWrite(barrierLpwm, 0);
    timeRan = millis() - beginTime;

    if(_converted_bLH + timeRan > 1943){
      _converted_bLH = 1943;
    }else{
      _converted_bLH = _converted_bLH + timeRan;
    }
  }
  else if(_converted_bLH >= 1943){
    Serial.print("LOWERING LEFT BARRIER\n");
    digitalWrite(barrierL, HIGH);
    analogWrite(barrierLpwm, 255);
    while(digitalRead(leftBarrierButton) == LOW){
    }
    analogWrite(barrierLpwm, 0);
    timeRan = millis() - beginTime;

    if(1943 - timeRan < 0){
      _converted_bLH = 0;
    }else{
      _converted_bLH = 1943 - timeRan;
    }
  }

}

void CTM_base::buttonRPumpPressed(){
  Serial.print("BEGIN PUMPING RIGHT SIDE\n");
  int beginTime = millis();

  while(digitalRead(rightPumpButton) == LOW){
    digitalWrite(pump1Output, HIGH);
  }

  digitalWrite(pump1Output, LOW);

  int timeRan = millis() - beginTime;

  Serial.print("Right Pump Ran For: ");
  Serial.print(timeRan);
  Serial.print(" ms\n");


  Serial.print("END PUMPING RIGHT SIDE\n");
}

void CTM_base::buttonLPumpPressed(){
  Serial.print("BEGIN PUMPING LEFT SIDE\n");
  int beginTime = millis();

  while(digitalRead(leftPumpButton) == LOW){
    digitalWrite(pump2Output, HIGH);
  }

  digitalWrite(pump2Output, LOW);

  int timeRan = millis() - beginTime;

  Serial.print("Left Pump Ran For: ");
  Serial.print(timeRan);
  Serial.print(" ms\n");


  Serial.print("END PUMPING LEFT SIDE\n");

}












void CTM_base::checkButtons() {
  startDoorButtonState = digitalRead(startDoorButton);
  backRightDoorButtonState = digitalRead(backRightDoorButton);
  backLeftDoorButtonState = digitalRead(backLeftDoorButton);
  frontRightDoorButtonState = digitalRead(frontRightDoorButton);
  frontLeftDoorButtonState = digitalRead(frontLeftDoorButton);

  rightBarrierButtonState = digitalRead(rightBarrierButton);
  leftBarrierButtonState = digitalRead(leftBarrierButton);

  rightPumpButtonState = digitalRead(rightPumpButton);
  leftPumpButtonState = digitalRead(leftPumpButton);  

  if(startDoorButtonState == LOW){
    buttonD1Pressed();
  }
  if(backLeftDoorButtonState == LOW){
    buttonD2Pressed();
  }
  if(backRightDoorButtonState == LOW){
    buttonD3Pressed();
  }
  if(frontLeftDoorButtonState == LOW){
    buttonD4Pressed();
  }
  if(frontRightDoorButtonState == LOW){
    buttonD5Pressed();
  }

  if(rightBarrierButtonState == LOW){
    buttonRBarrierPressed();
  }
  if(leftBarrierButtonState == LOW){
    buttonLBarrierPressed();
  }

  if(rightPumpButtonState == LOW){
    buttonRPumpPressed();
  }
  if(leftPumpButtonState == LOW){
    buttonLPumpPressed();
  }
}

void CTM_base::testSensors() {
  if(digitalRead(PIR_Start)){
    Serial.print("PIR_Start has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_Midstem)){
    Serial.print("PIR_Midstem has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_Endstem)){
    Serial.print("PIR_Endstem has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_LeftPostVertex)){
    Serial.print("PIR_LeftPostVertex has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_RightPostVertex)){
    Serial.print("PIR_RightPostVertex has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_LeftPreBarrier)){
    Serial.print("PIR_LeftPreBarrier has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_RightPreBarrier)){
    Serial.print("PIR_RightPreBarrier has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_LeftStartBox)){
    Serial.print("PIR_LeftStartBox has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
  if(digitalRead(PIR_RightStartBox)){
    Serial.print("PIR_RightStartBox has been activated at time: ");
    Serial.println(millis());
    delay(100);
  }
}

void CTM_base::playWithBarriers() {
  /*
   setting barrier to LOW makes it go up
   setting barrier to HIGH makes it go down
   */
  digitalWrite(barrierR, LOW); // LOW = go up
  for (int i = 0; i < 255; i++) {
    analogWrite(barrierRpwm, i);
    delay(5);
  }
  delay(3500);
  analogWrite(barrierRpwm, 0);

  digitalWrite(barrierL, LOW); // LOW = go up
  for (int i = 0; i < 255; i++) {
    analogWrite(barrierLpwm, i);
    delay(5);
  }
  delay(3500);
  analogWrite(barrierLpwm, 0);

  digitalWrite(barrierR, HIGH); // HIGH = go down
  for (int i = 0; i < 255; i++) {
    analogWrite(barrierRpwm, i);
    delay(5);
  }
  delay(3500);
  analogWrite(barrierRpwm, 0);

  digitalWrite(barrierL, HIGH); // HIGH = go down
  for (int i = 0; i < 255; i++) {
    analogWrite(barrierLpwm, i);
    delay(5);
  }
  delay(3500);
  analogWrite(barrierLpwm, 0);
}

void CTM_base::enactForceTrials() {
  if (_ftSide == 0) {
    digitalWrite(outputD2, LOW); // close D2
    isD2Open = false;
  } else {
    digitalWrite(outputD3, LOW); // close D3
    isD3Open = false;
  }
}
