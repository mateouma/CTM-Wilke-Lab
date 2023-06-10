char _trialType[] = "3v1B"; 

// amount of time to dispense rewards
int _HRTime = 3000;
int _LRTime = 1000;

// barrier heights
int _bRH = 10;
int _bLH = 5;

// choosing which side is the HR
char _HRside = "R";
char _leftReward[] = "LR";
char _rightReward[] = "HR";

// choosing higher probability
float _prob = 0.8;

bool PIRStartPrimed = true; //set trial available to start
bool PIRStartActivated = false;
bool PIRMidstemPrimed = false;
bool PIRMidstemActivated = false;
bool PIREndstemPrimed = false;
bool PIREndstemActivated = false;
bool PIRLeftPostVertexPrimed = false;
bool PIRLeftPostVertexActivated = false;
bool PIRRightPostVertexPrimed = false;
bool PIRRightPostVertexActivated = false;
bool PIRLeftPreBarrierPrimed = false;
bool PIRLeftPreBarrierActivated = false;
bool PIRRightPreBarrierPrimed = false;
bool PIRRightPreBarrierActivated = false;
bool PIRLeftStartBoxPrimed = false;
bool PIRLeftStartBoxActivated = false;
bool PIRRightStartBoxPrimed = false;
bool PIRRightStartBoxActivated = false;

unsigned long startTime;
unsigned long currentTime;

// =============================
// PINS
// edit these variables if the pins change
// =============================

// door outputs
int outputD1 = 32;
int outputD2 = 33;
int outputD3 = 34;
int outputD4 = 35;
int outputD5 = 41;

// barrier outputs
int barrierR = 9;
int barrierRpwm = 14;
int barrierL = 8;
int barrierLpwm = 17; 

// sensors
int PIR_Start = 22;
int PIR_Midstem = 23;
int PIR_Endstem = 24;
int PIR_LeftPostVertex = 25;
int PIR_RightPostVertex = 26;
int PIR_LeftPreBarrier = 27;
int PIR_RightPreBarrier = 28;
int PIR_LeftStartBox = 29;
int PIR_RightStartBox = 30;

// reward
int pump1Output = 5;
int pump2Output = 2;

void setup() {
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
  
  delay(1000);
  Serial.print("Setup complete\n");
  startTimer();
}

void loop() {
  checkSensors();
}

void resetBarriers() {
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

void configureBarriers() {
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

void startTimer() {
  startTime = millis();
  Serial.print("Start time: ");
  Serial.print(startTime);
  Serial.print("\n");
}

void ActivatePIRStart() {
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

void ActivatePIRMidstem() {
  if (digitalRead(PIR_Midstem) && PIRMidstemPrimed && !PIRMidstemActivated) { // Check for activated midstem sensor AND that midstem sensor is primed AND that is is not already activated
    currentTime = millis();
    Serial.print("PIR Midstem Activation Time: ");
    Serial.print(currentTime);
    Serial.print("\n");
    
    PIRMidstemPrimed = false; // Un-prime PIRMidstem
    PIRMidstemActivated = true; 
    
    PIRMidstemCloseD1();
  }
}

void ActivatePIREndstem() {
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

void ActivatePIRLeftPostVertex() {
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

void ActivatePIRRightPostVertex() {
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

void ActivatePIRLeftPreBarrier() {
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

void ActivatePIRRightPreBarrier() {
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

void ActivatePIRLeftStartBox() {
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

void ActivatePIRRightStartBox() {
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

void checkSensors() {
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

void resetFlags() {
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


void activatePump(char *rew, char *pump, long prob) {
  // sends a pulse of a given duration to the reward board to activate pump
  
  int pumpOP;
  int durationHP; // duration with higher probability
  int durationLP; // duration with lower probability
  int chosenDuration;
  
  // HR or LR pump?
  if (rew == "HR") {
    durationHP = _HRTime;
    durationLP = _LRTime;
  }

  else if (rew == "LR"){
    durationHP = _LRTime;
    durationLP = _HRTime;
  }
  // pick reward with prob% chance
  long probPicker = random(1);
  Serial.println(pump);
  if (probPicker <= prob) {
    chosenDuration = durationHP;
    Serial.print(rew);
    Serial.print("; ");
    Serial.println("HP");
  } else {
    chosenDuration = durationLP;
    Serial.print(rew);
    Serial.print(" ; ");
    Serial.println("LP");
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

void PIRStartOpenD1D2D3() {
  digitalWrite(outputD1, HIGH); // Open D1
  digitalWrite(outputD2, HIGH); // Open D2
  digitalWrite(outputD3, HIGH); // Open D3
  
  delay(100);
  PIRMidstemPrimed = true; // Prime Midstem sensor
}

void PIRMidstemCloseD1() {
  delay(100);
  PIREndstemPrimed = true; // Prime Endstem
  
  // Closes the Midstem door after a 2.5s delay
  delay(2500);
  digitalWrite(outputD1, LOW); // Close D1
}

void PIRLPBCloseD2D3D5OpenD4() {
  delay(1000);
  digitalWrite(outputD2, LOW); // Close D2
  digitalWrite(outputD3, LOW); // Close D3
  digitalWrite(outputD5, LOW);
  digitalWrite(outputD4, HIGH); // Open D4
  //Serial.print("D2 and D3 Closed\n"); 

  activatePump(_leftReward, 'L', _prob);

  delay(100);
  PIRLeftStartBoxPrimed = true; // Prime LSB sensor  
}

void PIRRPBCloseD3D2D4OpenD5() {
  delay(1000);
  digitalWrite(outputD2, LOW);
  digitalWrite(outputD3, LOW); // Close D3
  digitalWrite(outputD4, LOW);
  //Serial.print("D3 Closed\n"); 

  digitalWrite(outputD5, HIGH); // Open D5
  //Serial.print("D5 Open\n");  
  activatePump(_rightReward, 'R', _prob);

  delay(100);
  PIRRightStartBoxPrimed = true;
}

void PIRLSBCloseD4() {
  delay(2500);
  digitalWrite(outputD4, LOW); // Close D4
  //Serial.print("D4 Closed\n");

  delay(3000);
  resetFlags();
}

void PIRRSBCloseD5() { 
  delay(2500);
  digitalWrite(outputD5, LOW); // Close D5
  //Serial.print("D5 Closed\n");

  delay(3000);
  resetFlags();
}

void playWithBarriers() {
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