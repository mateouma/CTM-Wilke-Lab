// ============================
// DOOR OUTPUTS
// ============================

int outputD1 = 32;
int outputD2 = 33;
int outputD3 = 34;
int outputD4 = 35;
int outputD5 = 41;

// ============================
// BARRIER OUTPUTS
// ============================

int barrierR = 9;
int barrierRpwm = 14; // control speed of right barrier
int barrierL = 8;
int barrierLpwm = 17; // control speed of left barrier

// ============================
// SENSORS
// ============================

int PIR_Start = 22;
int PIR_Midstem = 23;
int PIR_Endstem = 24;
int PIR_LeftPostVertex = 25;
int PIR_RightPostVertex = 26;
int PIR_LeftPostBarrier = 27;
int PIR_RightPostBarrier = 28;
int PIR_LeftStartBox = 29;
int PIR_RightStartBox = 30;

// ============================
// TRIAL EVENTS
// ============================

bool Midstem_TA = true;
bool Endstem_TA = true;
bool LPV_TA = true;
bool RPV_TA = true;
bool LPB_TA = true;
bool RPB_TA = true;
bool LSB_TA = true;
bool RSB_TA = true;

// ============================
// MAIN
// ============================

void setup() {
  // connect to serial monitor
  Serial.begin(9600);

  delay(3000);
  
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
  pinMode(PIR_RightPostBarrier, INPUT);
  pinMode(PIR_LeftPostVertex, INPUT);
  pinMode(PIR_LeftPostBarrier, INPUT);
  pinMode(PIR_RightStartBox, INPUT);
  pinMode(PIR_LeftStartBox, INPUT);

  // turn on internal pull-up resistors
  digitalWrite(PIR_Start, HIGH);
  digitalWrite(PIR_Midstem, HIGH);
  digitalWrite(PIR_RightPostVertex, HIGH);
  digitalWrite(PIR_RightPostBarrier, HIGH);
  digitalWrite(PIR_LeftPostVertex, HIGH);
  digitalWrite(PIR_LeftPostBarrier, HIGH);
  digitalWrite(PIR_RightStartBox, HIGH);
  digitalWrite(PIR_LeftStartBox, HIGH);

  delay(3000);

  // open all doors
  digitalWrite(outputD1, HIGH);
  digitalWrite(outputD2, HIGH);
  digitalWrite(outputD3, HIGH);
  digitalWrite(outputD4, HIGH);
  digitalWrite(outputD5, HIGH);
  
  Serial.print("Setup complete\n");
  delay(3000);
}

void loop() {
  CheckPIRStart();
  CheckPIRRightStartBox();
  CheckPIRLeftStartBox();
  CheckPIRMidstem();
  CheckPIREndstem();
  CheckPIRRightPostVertex();
  CheckPIRRightPostBarrier();
  CheckPIRLeftPostVertex();
  CheckPIRLeftPostBarrier();
}

// ============================
// SENSOR FUNCTIONS
// ============================

void CheckPIRStart() {
  if(digitalRead(PIR_Start)) {
    Serial.print("PIR_Start state: ");
    Serial.print(digitalRead(PIR_Start)); 
    Serial.print("\n");
    delay(1500);

    // open doors
    digitalWrite(outputD1, HIGH);
    digitalWrite(outputD2, HIGH);
    digitalWrite(outputD3, HIGH);
    digitalWrite(outputD4, HIGH);
    digitalWrite(outputD5, HIGH);

    Midstem_TA = true;
    Endstem_TA = true;
    LPV_TA = true;
    RPV_TA = true;
    LPB_TA = true;
    RPB_TA = true;
    LSB_TA = true;
    RSB_TA = true;

    delay(3000);
  }
}

void CheckPIRMidstem() {
  if(digitalRead(PIR_Midstem) && Midstem_TA) {
    Serial.print("PIR_Midstem state: ");
    Serial.print(digitalRead(PIR_Midstem));
    Serial.print("\n");
    delay(1500);
    digitalWrite(outputD1, LOW);
    Midstem_TA = false; // PIR sensor cannot do anything until *loc*_TA is true again
  }
}

void CheckPIREndstem() {
  if(digitalRead(PIR_Endstem) && Endstem_TA) {
    Serial.print("PIR_Endstem state: ");
    Serial.print(digitalRead(PIR_Endstem));
    Serial.print("\n");
    Endstem_TA = false; 
    delay(1500);
  }
}

void CheckPIRLeftPostVertex() {
  if(digitalRead(PIR_LeftPostVertex) && LPV_TA) {
    Serial.print("PIR_LPV state: ");
    Serial.print(digitalRead(PIR_LeftPostVertex));
    Serial.print("\n");
    LPV_TA = false;
    delay(1500);
    digitalWrite(outputD2, LOW);
  }
}

void CheckPIRRightPostVertex() {
  if(digitalRead(PIR_RightPostVertex) && RPV_TA) {
    Serial.print("PIR_RPV state: ");
    Serial.print(digitalRead(PIR_RightPostVertex));
    Serial.print("\n");
    RPV_TA = false;
    delay(1500);
    digitalWrite(outputD3, LOW);
  }
}

void CheckPIRLeftPostBarrier() {
  if(digitalRead(PIR_LeftPostBarrier) && LPB_TA) {
    Serial.print("PIR_LPB detection");
    Serial.print("\n");
    LPB_TA = false;
    delay(1500);
  }
}

void CheckPIRRightPostBarrier() {
  if(digitalRead(PIR_RightPostBarrier) && RPB_TA) {
    Serial.print("PIR_RPB state: ");
    Serial.print(digitalRead(PIR_RightPostBarrier));
    Serial.print("\n");
    RPB_TA = false;
    delay(1500);
  }
}

void CheckPIRLeftStartBox() {
  if(digitalRead(PIR_LeftStartBox) && LSB_TA) {
    Serial.print("PIR_LSB state: ");
    Serial.print(digitalRead(PIR_LeftStartBox));
    Serial.print("\n");
    LSB_TA = false;
    delay(1500);
    digitalWrite(outputD4, LOW);
  }
}

void CheckPIRRightStartBox() {
  if(digitalRead(PIR_RightStartBox) && RSB_TA) {
    Serial.print("PIR_RSB state: ");
    Serial.print(digitalRead(PIR_RightStartBox));
    Serial.print("\n");
    RSB_TA = false;
    delay(1500);
    digitalWrite(outputD5, LOW);
  }
}

// ============================
// TEST FUNCTIONS
// ============================

void playWithDoors() {
  // opens and closes all doors sequentially
  Serial.print("playing with doors \n");

  // opens D1, waits 3 seconds, closes D1
  digitalWrite(outputD1, HIGH);
  delay(3000);
  digitalWrite(outputD1, LOW);

  digitalWrite(outputD5, HIGH);
  delay(3000);
  digitalWrite(outputD5, LOW);

  digitalWrite(outputD2, HIGH);
  delay(3000);
  digitalWrite(outputD2, LOW);

  digitalWrite(outputD3, HIGH);
  delay(3000);
  digitalWrite(outputD3, LOW);

  digitalWrite(outputD4, HIGH);
  delay(3000);
  digitalWrite(outputD4, LOW);
}

void playWithBarrier() {
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
