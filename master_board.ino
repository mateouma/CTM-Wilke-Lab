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

  playWithBarrier();
  
  delay(5000);

  // open doors (down)
  digitalWrite(outputD1, HIGH);
  digitalWrite(outputD2, HIGH);
  digitalWrite(outputD3, HIGH);
  digitalWrite(outputD4, HIGH);
  digitalWrite(outputD5, HIGH);

  Serial.print("Setup complete\n");
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
  }
}

void CheckPIRMidstem() {
  if(digitalRead(PIR_Midstem)) {
    Serial.print("PIR_Midstem state: ");
    Serial.print(digitalRead(PIR_Midstem));
    Serial.print("\n");
    delay(1500);
  }
}

void CheckPIREndstem() {
  if(digitalRead(PIR_Endstem)) {
    Serial.print("PIR_Endstem state: ");
    Serial.print(digitalRead(PIR_Endstem));
    Serial.print("\n");
    delay(1500);
  }
}

void CheckPIRLeftPostVertex() {
  if(digitalRead(PIR_LeftPostVertex)) {
    Serial.print("PIR_LPV state: ");
    Serial.print(digitalRead(PIR_LeftPostVertex));
    Serial.print("\n");
    delay(1500);
  }
}

void CheckPIRRightPostVertex() {
  if(digitalRead(PIR_RightPostVertex)) {
    Serial.print("PIR_RPV state: ");
    Serial.print(digitalRead(PIR_RightPostVertex));
    Serial.print("\n");
    delay(1500);
  }
}

void CheckPIRLeftPostBarrier() {
  if(digitalRead(PIR_LeftPostBarrier)) {
    Serial.print("PIR_LPB state: ");
    Serial.print(digitalRead(PIR_LeftPostBarrier));
    Serial.print("\n");
    delay(1500);
  }
}

void CheckPIRRightPostBarrier() {
  if(digitalRead(PIR_RightPostBarrier)) {
    Serial.print("PIR_RPB state: ");
    Serial.print(digitalRead(PIR_RightPostBarrier));
    Serial.print("\n");
    delay(1500);
  }
}

void CheckPIRLeftStartBox() {
  if(digitalRead(PIR_LeftStartBox)) {
    Serial.print("PIR_LSB state: ");
    Serial.print(digitalRead(PIR_LeftStartBox));
    Serial.print("\n");
    delay(1500);
  }
}

void CheckPIRRightStartBox() {
  if(digitalRead(PIR_RightStartBox)) {
    Serial.print("PIR_RSB state: ");
    Serial.print(digitalRead(PIR_RightStartBox));
    Serial.print("\n");
    delay(1500);
  }
}

// ============================
// TEST FUNCTIONS
// ============================

void playWithDoors() {
  // opens and closes all doors sequentially
  Serial.print("playing with doors \n");

  digitalWrite(outputD1, HIGH);
  delay(5000);
  digitalWrite(outputD1, LOW);

  digitalWrite(outputD5, HIGH);
  delay(5000);
  digitalWrite(outputD5, LOW);

  digitalWrite(outputD2, HIGH);
  delay(5000);
  digitalWrite(outputD2, LOW);

  digitalWrite(outputD3, HIGH);
  delay(5000);
  digitalWrite(outputD3, LOW);

  digitalWrite(outputD4, HIGH);
  delay(5000);
  digitalWrite(outputD4, LOW);
}

void playWithBarrier() {
  /*
  setting barrier to LOW makes it go up
  setting barrier to HIGH makes it go down
   */
  digitalWrite(barrierR, LOW); 
  for (int i = 0; i < 255; i++) {
    analogWrite(barrierRpwm, i);
    delay(5);
  }
  delay(3500);
  analogWrite(barrierRpwm, 0);

  digitalWrite(barrierL, LOW);
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
