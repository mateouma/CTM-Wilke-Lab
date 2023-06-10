/*
 CTM_base.cpp - Library for continuous T-maze base functions
 Created by Mateo Umagung, May 31, 2023.
 Authorized for Wilke Lab use only.
 */

#include "Arduino.h"
#include "CTM_base.h"

CTM_base::CTM_base() {  

}

void CTM_base::configureParams(int HRTime, int LRTime, int barrierRHeight, int barrierLHeight, float prob, char HRside) {
  _HRTime = HRTime;
  _LRTime = LRTime;
  _bRH = barrierRHeight;
  _bLH = barrierLHeight;
  _HRside = HRside;
  _prob = prob;
  
  if (HRside == 'R') {
    _rightReward = "H";
    _leftReward = "L";
  } else if (HRside == 'L') {
    _rightReward = 'L';
    _leftReward = 'R';
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
  
  resetBarriers();
  configureBarriers();

  digitalWrite(outputD2, HIGH); // Open D2
  digitalWrite(outputD3, HIGH); // Open D3
  
  delay(500);
  Serial.print("Setup complete\n");
  startTimer();
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
  // right barrier
  switch (_bRH) {
    case 0:
      // do nothing
      break;
    case 5:
      digitalWrite(barrierR, LOW);
      for (int i = 0; i < 200; i++) {
        analogWrite(barrierRpwm, i);
        delay(8);
      }
      delay(300);
      analogWrite(barrierRpwm, 0);
      break;
    case 10:
      digitalWrite(barrierR, LOW); // go up
      for (int i = 0; i < 255; i++) {
        analogWrite(barrierRpwm, i);
        delay(8);
      }

      delay(1500);
      analogWrite(barrierRpwm, 0);
      break;
  }

  switch (_bLH) {
    case 0:
      // do nothing
      break;
    case 5:
      digitalWrite(barrierL, LOW);
      for (int i = 0; i < 200; i++) {
        analogWrite(barrierLpwm, i);
        delay(8);
      }
      delay(300);
      analogWrite(barrierLpwm, 0);
      break;
    case 10:
      digitalWrite(barrierL, LOW); // go up
      for (int i = 0; i < 255; i++) {
        analogWrite(barrierLpwm, i);
        delay(8);
      }

      delay(1500);
      analogWrite(barrierLpwm, 0);
      break;
  }
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
    PIRRightPreBarrierPrimed = false;
  }
}

void CTM_base::ActivatePIRRightPostVertex() {
  if (digitalRead(PIR_RightPostVertex) && PIRRightPostVertexPrimed && !PIRRightPostVertexActivated) {
    currentTime = millis();
    Serial.print("PIR RPV Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
  
    PIRRightPostVertexActivated = true;
  
    PIRLeftPreBarrierPrimed = false;
    PIRRightPreBarrierPrimed = true;
  }
}

void CTM_base::ActivatePIRLeftPreBarrier() {
  if (digitalRead(PIR_LeftPreBarrier) && PIRLeftPreBarrierPrimed && !PIRLeftPreBarrierActivated) {
    currentTime = millis();
    Serial.print("PIR LPB Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    PIRLeftPreBarrierPrimed = false; // Un-prime PIRLPB
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
    
    PIRRightPreBarrierPrimed = false; // Un-prime PIRRPB
    PIRLeftPreBarrierPrimed = false;
    
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
  
  Serial.print("Flags Reset\n");
}


void CTM_base::activatePump(char rew, char pump, long prob) {
  // sends a pulse of a given duration to the reward board to activate pump
  
  int pumpOP;
  int durationHP; // duration with higher probability
  int durationLP; // duration with lower probability
  int chosenDuration;
  
  // HR or LR pump?
  if (rew == 'H') {
    durationHP = _HRTime;
    durationLP = _LRTime;
  }

  else if (rew == 'L'){
    durationHP = _LRTime;
    durationLP = _HRTime;
  }
  // pick reward with prob% chance
  long probPicker = random(1);
  Serial.println(pump);
  if (probPicker <= prob) {
    chosenDuration = durationHP;
  } else {
    chosenDuration = durationLP;
  }
  // choose pump
  if (pump == 'R')
    pumpOP = pump1Output;
  
  else if (pump == 'L') 
    pumpOP = pump2Output;

  digitalWrite(pumpOP, HIGH);
  delay(chosenDuration);
  digitalWrite(pumpOP, LOW);
  delay(20);
}

void CTM_base::PIRStartOpenD1D2D3() {
  digitalWrite(outputD1, HIGH); // Open D1
  //digitalWrite(outputD2, HIGH); // Open D2
  //digitalWrite(outputD3, HIGH); // Open D3
  
  delay(100);
  PIRMidstemPrimed = true; // Prime Midstem sensor
}

void CTM_base::PIRMidstemCloseD1() {
  delay(100);
  PIREndstemPrimed = true; // Prime Endstem
  
  // Closes the Midstem door after a 2.5s delay
  delay(2500);
  digitalWrite(outputD1, LOW); // Close D1
}

void CTM_base::PIRLPBCloseD2D3D5OpenD4() {
  delay(1000);
  digitalWrite(outputD2, LOW); // Close D2
  digitalWrite(outputD3, LOW); // Close D3
  digitalWrite(outputD5, LOW); // Close D5
  digitalWrite(outputD1, LOW); // Close D1

  digitalWrite(outputD4, HIGH); // Open D4
  //Serial.print("D2 and D3 Closed\n"); 

  activatePump(_leftReward, 'L', _prob);

  delay(100);
  PIRLeftStartBoxPrimed = true; // Prime LSB sensor  
}

void CTM_base::PIRRPBCloseD3D2D4OpenD5() {
  delay(1000);
  digitalWrite(outputD2, LOW); // Close D2
  digitalWrite(outputD3, LOW); // Close D3
  digitalWrite(outputD4, LOW); // Close D4
  digitalWrite(outputD1, LOW); // Close D1

  digitalWrite(outputD5, HIGH); // Open D5
  activatePump(_rightReward, 'R', _prob);

  delay(100);
  PIRRightStartBoxPrimed = true;
}

void CTM_base::PIRLSBCloseD4() {
  delay(3500);
  digitalWrite(outputD4, LOW); // Close D4
  //Serial.print("D4 Closed\n");

  delay(8000);
  resetFlags();
}

void CTM_base::PIRRSBCloseD5() { 
  delay(3500);
  digitalWrite(outputD5, LOW); // Close D5
  //Serial.print("D5 Closed\n");

  delay(8000);
  resetFlags();
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