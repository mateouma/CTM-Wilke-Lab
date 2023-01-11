// ============================
// MOTOR TO RELAY PINS
// ============================

int D1Pin = 40; //LEDState changed to D1Pin
int D2Pin = 41;
int D3Pin = 42;
int D4Pin = 43;
int D5Pin = 44;

// ============================
// DOOR INPUTS FROM MASTER
// ============================

int inputD1 = 36;
int inputD2 = 37;
int inputD3 = 34;
int inputD4 = 35;
int inputD5 = 31;

void setup() {
  // designate pins
  pinMode(D1Pin, OUTPUT);
  pinMode(D2Pin, OUTPUT);
  pinMode(D3Pin, OUTPUT);
  pinMode(D4Pin, OUTPUT);
  pinMode(D5Pin, OUTPUT);

  pinMode(inputD1, INPUT);
  pinMode(inputD2, INPUT);
  pinMode(inputD3, INPUT);
  pinMode(inputD4, INPUT);
  pinMode(inputD5, INPUT);

  // turn on internal pull-up resistors
  digitalWrite(inputD1, HIGH);
  digitalWrite(inputD2, HIGH);
  digitalWrite(inputD3, HIGH);
  digitalWrite(inputD4, HIGH);
  digitalWrite(inputD5, HIGH);
  
  delay(2000);
  
  // open doors
  digitalWrite(D1Pin, HIGH);
  digitalWrite(D2Pin, HIGH);
  digitalWrite(D3Pin, HIGH);
  digitalWrite(D4Pin, HIGH);
  digitalWrite(D5Pin, HIGH);

  delay(5000);
  
  // close doors
//  digitalWrite(D1Pin, LOW);
//  digitalWrite(D2Pin, LOW);
//  digitalWrite(D3Pin, LOW);
//  digitalWrite(D4Pin, LOW);
//  digitalWrite(D5Pin, LOW);
  
  delay(5000);
}

void loop() {
  readD1Input();
  readD2Input();
  readD3Input();
  readD4Input();
  readD5Input();
}

void readD1Input() {
  if(digitalRead(inputD1))
    digitalWrite(D1Pin, HIGH);
  else
    digitalWrite(D1Pin, LOW);
}

void readD2Input() {
  if(digitalRead(inputD2))
    digitalWrite(D2Pin, HIGH);
  else
    digitalWrite(D2Pin, LOW);
}

void readD3Input() {
  if(digitalRead(inputD3))
    digitalWrite(D3Pin, HIGH);
  else
    digitalWrite(D3Pin, LOW);
}

void readD4Input() {
  if(digitalRead(inputD4))
    digitalWrite(D4Pin, HIGH);
  else
    digitalWrite(D4Pin, LOW);
}

void readD5Input() {
  if(digitalRead(inputD5))
    digitalWrite(D5Pin, HIGH);
  else
    digitalWrite(D5Pin, LOW);
}
 
