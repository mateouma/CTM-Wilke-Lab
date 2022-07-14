// to feather the door
int NumPulses = 1;
int PulseDuration = 20;

// button inputs
int button7 = 2;
int button8 = 3;
int button9 = 4; // top yellow button
int button10 = 5;
int button11 = 6; // bottom yellow button
int button12 = 7;
int button13 = 8;
int button14 = 9;
int button15 = 12;
int button16 = 13; // right white button
int button17 = 14;
int button18 = 15;

int buttons[] = {
  button7,
  button8,
  button9,
  button10,
  button11,
  button12,
  button13,
  button14,
  button15,
  button16,
  button17,
  button18
};

// drive doors
// these are outputs from the motor arduino to the relays
int pin40 = 40; // open D1
int pin41 = 41; // open D2
int pin42 = 42; // open D3
int pin43 = 43; // open D4
int pin44 = 44; // open D5
int pin45 = 45;
int pin46 = 46;
int pin47 = 47;
int pin48 = 48;
int pin49 = 49;
int pin50 = 50;
int pin51 = 51;
int pin52 = 52;
int pin53 = 53;

int door_pins[] = {
  pin40,
  pin41,
  pin42,
  pin43,
  pin44,
  pin45,
  pin46,
  pin47,
  pin48,
  pin49,
  pin50,
  pin51,
  pin52,
  pin53
};

void setup() {
  Serial.begin(9600);

  // initialize button pins
  // INPUT_PULLUP means button presses register as LOW
  //pinMode(button7, INPUT_PULLUP);
  //pinMode(button8, INPUT_PULLUP);
  //pinMode(button9, INPUT_PULLUP);
  //pinMode(button10, INPUT_PULLUP);
  //pinMode(button11, INPUT_PULLUP);
  //pinMode(button12, INPUT_PULLUP);
  //pinMode(button13, INPUT_PULLUP);
  //pinMode(button14, INPUT_PULLUP);
  //pinMode(button15, INPUT_PULLUP);
  //pinMode(button16, INPUT_PULLUP);
  //pinMode(button17, INPUT_PULLUP);
  //pinMode(button18, INPUT_PULLUP);
  
  /*for (int i = 0; i < 12; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }*/

  pinMode(pin40, OUTPUT);
  pinMode(pin41, OUTPUT);
  pinMode(pin42, OUTPUT);
  pinMode(pin44, OUTPUT);
  pinMode(pin43, OUTPUT);
  pinMode(pin45, OUTPUT);
  pinMode(pin46, OUTPUT);
  pinMode(pin47, OUTPUT);
  pinMode(pin48, OUTPUT);
  pinMode(pin49, OUTPUT);
  pinMode(pin50, OUTPUT);
  pinMode(pin51, OUTPUT);
  pinMode(pin52, OUTPUT);
  pinMode(pin53, OUTPUT);

  // initialize door pins
  /*for (int i = 0; i < 14; i++) {
    pinMode(door_pins[i], OUTPUT);
  }*/

  Serial.print("Setup complete\n");
  delay(2000);
  Serial.print("beginning tests\n");
  open_all_doors();
  delay(5000);
  
}

void loop() {
  //button7_func();
  //button8_func();
  //button9_func();
  //button10_func();
  //button11_func();
  //button12_func();
  //button13_func();
  //button14_func();
  //button15_func();
  //button16_func();
  //button17_func();
  //button18_func();
}

//-------------------------------------------
// SIGNAL SEND FUNCTION (OPEN/CLOSE DOORS)
//-------------------------------------------

void feather_pulses(int pin, int value) {
  Serial.print("Pin ");
  Serial.print(pin);
  Serial.print(", Value ");
  Serial.print(value);
  Serial.print("\n");
  for (int i = 0; i < NumPulses; i++) {
    digitalWrite(pin, value);
    delay(PulseDuration);
    digitalWrite(pin, value);
    delay(20);
  }
}

void open_all_doors(){
  feather_pulses(pin40, HIGH);
  feather_pulses(pin41, HIGH);
  feather_pulses(pin42, HIGH);
  feather_pulses(pin43, HIGH);
  feather_pulses(pin44, HIGH);
}

//-------------------------------------------
// BUTTON FUNCTIONS
//-------------------------------------------
/*void button7_func() {
  if(!digitalRead(button7)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 7 pressed\n");
    delay(500);
    digitalWrite(button7, HIGH);
  }
}*/

/*void button8_func() {
  if(!digitalRead(button8)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 8 pressed\n");
    delay(500);
    digitalWrite(button8, HIGH);
  }
}*/

void button9_func() {
  if(!digitalRead(button9)) {
    feather_pulses(pin42, HIGH);
    Serial.print("button 9 pressed\n");
    delay(500);
  }
}

/*void button10_func() {
  if(!digitalRead(button10)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 10 pressed\n");
    delay(500);
    digitalWrite(button10, HIGH);
  }
}*/

void button11_func() {
  if(!digitalRead(button11)) {
    feather_pulses(pin44, HIGH);
    Serial.print("button 11 pressed\n");
    delay(500);
  }
}

/*void button12_func() {
  if(!digitalRead(button12)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 12 pressed\n");
    delay(500);
    digitalWrite(button12, HIGH);
  }
}*/

/*void button13_func() {
  if(!digitalRead(button13)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 13 pressed\n");
    delay(500);
    digitalWrite(button13, HIGH);
  }
}*/

/*void button14_func() {
  if(!digitalRead(button14)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 14 pressed\n");
    delay(500);
    digitalWrite(button14, HIGH);
  }
}*/

/*void button15_func() {
  if(!digitalRead(button15)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 15 pressed\n");
    delay(500);
    digitalWrite(button15, HIGH);
  }
}*/

void button16_func() {
  if(!digitalRead(button16)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 16 pressed\n");
    delay(500);
  }
}

/*void button17_func() {
  if(!digitalRead(button17)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 17 pressed\n");
    delay(500);
    digitalWrite(button17, HIGH);
  }
}*/

/*void button18_func() {
  if(!digitalRead(button18)) {
    feather_pulses(pin41, HIGH);
    Serial.print("button 18 pressed\n");
    delay(500);
    digitalWrite(button18, HIGH);
  }
}*/
