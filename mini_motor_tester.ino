//----------------------------------------------
//      CODE TO CONTROL WILKE MAZE DOORS 
//----------------------------------------------
//                   GOAL
// - open/close different doors in the maze 
//----------------------------------------------

//----------------------------------------------
//              DEVELOPMENT LOG
//----------------------------------------------
// v01 by Thomas Elston; telston@nurhopsi.org
//----------------------------------------------
// v02 by Gary Wilke; coach.wilke@gmail.com
//----------------------------------------------
// mini tester by Mateo Umaguing; mateou@g.ucla.edu
  
//             DOOR SPEED / AMOUNT
// to feather the door
int NumPulses = 1;   // number of pulses
int PulseDuration = 20; // 10ms

// *** NOTE: it's possible that the solenoids will click
// when activated (some do, some don't). If they click, 
// just set NumPulses = 1 and elongate PulseDuration
// so that one long pulse opens/closes the doors. 


//           USE MIDSTEM DOOR OR NOT
// you might not want to open/close the midstem door
// during the initial directional training. Here is a
// variable that lets you turn it off and of
boolean USEMIDSTEM = false; // can be set to false 


// FORMAT:
// PinName               = pinNum // Connection
//----------------------------------------------
//                TTL-INPUTS
//----------------------------------------------
// start gate AND right vertex
// int sig_openD1 = xx; // master pin xx
// int sig_openD1 = xx; // master pin xx

int sig_openD1andD2 = 36; // master pin 32
int sig_CloseVertexOpenRight = 37; // master pin 33

// start gate AND left vertex
int sig_openD1andD3 = 34; // master pin 34
int sig_CloseVertexOpenLeft = 35; // master pin 35

// start gate AND both vertex doors (for free choices)
int sig_openVertex = 10; // master pin 10
int sig_closeVertex = 11; // master pin 11

// right startbox
int sig_openD4 = 30; // master pin 38
int sig_closeD4 = 31; // master pin 39

// left start box
int sig_openD5 = 28; // master pin 40
int sig_closeD5 = 29; // master pin 41

int ttl_inputs[] = {
  sig_openD1andD2,
  sig_CloseVertexOpenRight,
  sig_openD1andD3,
  sig_CloseVertexOpenLeft,
  sig_openVertex,
  sig_closeVertex,
  sig_openD4,
  sig_closeD4,
  sig_openD5,
  sig_closeD5
};

//----------------------------------------------
//                BUTTON-INPUTS
//----------------------------------------------
// start gate / midstem
int button_openD1 = 2;  // button 7
int button_closeD1 = 3;  // button 8

// right vertex
int button_openD2 = 4;  // button 9
int button_closeD2 = 5;  // button 10

// left vertex
int button_openD3 = 6; // button 11
int button_closeD3 = 7; // button 12

// both vertex doors (for free choices)
int button_openVertex = 8; // button 13
int button_closeVertex = 9; // button 14

// right start box
int button_openD4 = 12; // button 15
int button_closeD4 = 13; // button 16

// left start box
int button_openD5 = 22; // button 17
int button_closeD5 = 23; // button 18

int button_inputs[] = {
  button_openD1,
  button_closeD1,
  button_openD2,
  button_closeD2,
  button_openD3,
  button_closeD3,
  button_openVertex,
  button_closeVertex,
  button_openD4,
  button_closeD4,
  button_openD5,
  button_closeD5
};

//----------------------------------------------
//      OUTPUT TO THE RELAYS / SOLENOIDS
//----------------------------------------------
// these drive the doors up/down
int OpenD1 = 40; // Relay 1
int CloseD1 = 41; // Relay 1
int OpenD2 = 42; // Relay 2
int CloseD2 = 43; // Relay 2
int OpenD3 = 44; // Relay 3
int CloseD3 = 45; // Relay 3
int OpenD4 = 46; // Relay 4
int CloseD4 = 47; // Relay 4
int OpenD5 = 48; // Relay 5
int CloseD5 = 49; // Relay 5

int relay_outputs[] = {
  OpenD1,
  CloseD1,
  OpenD2,
  CloseD2,
  OpenD3,
  CloseD3,
  OpenD4,
  CloseD4,
  OpenD5,
  CloseD5
};

//----------------------------------------------

void setup() {

  //Initiate Serial communication.
  Serial.begin(9600);

  // initialize TTL input pins
  for (int i = 0; i < 10; i++) {
    pinMode(ttl_inputs[i], INPUT);
  }
  /*
  pinMode(sig_openD1andD2, INPUT); 
  pinMode(sig_CloseVertexOpenRight, INPUT);
  pinMode(sig_openD1andD3, INPUT);  
  pinMode(sig_CloseVertexOpenLeft, INPUT);  
  pinMode(sig_openVertex, INPUT);
  pinMode(sig_closeVertex, INPUT);  

  pinMode(sig_openD4, INPUT);  
  pinMode(sig_closeD4, INPUT);  
  pinMode(sig_openD5, INPUT);  
  pinMode(sig_closeD5, INPUT);
  */

  // initialize button pins
  for (int i = 0; i < 12; i++) {
    pinMode(button_inputs[i], INPUT_PULLUP);
  }
  /*
  pinMode(button_openD1,INPUT_PULLUP);
  pinMode(button_closeD1,INPUT_PULLUP);  
  pinMode(button_openD2,INPUT_PULLUP);  
  pinMode(button_closeD2,INPUT_PULLUP);  
  pinMode(button_openD3,INPUT_PULLUP);  
  pinMode(button_closeD3,INPUT_PULLUP);  
  pinMode(button_openVertex,INPUT_PULLUP);// not sure I need this
  pinMode(button_closeVertex,INPUT_PULLUP); // not sure I need this
  pinMode(button_openD4,INPUT_PULLUP);  
  pinMode(button_closeD4,INPUT_PULLUP);  
  pinMode(button_openD5,INPUT_PULLUP);  
  pinMode(button_closeD5,INPUT_PULLUP); 
  */

  // initialize relay output pins
  for (int i = 0; i < 10; i++) {
    pinMode(relay_outputs[i], OUTPUT);
  }
  /*
  pinMode(OpenD1,OUTPUT);
  pinMode(CloseD1,OUTPUT);
  pinMode(OpenD2,OUTPUT);
  pinMode(CloseD2,OUTPUT);
  pinMode(OpenD3,OUTPUT);
  pinMode(CloseD3,OUTPUT);
  pinMode(OpenD4,OUTPUT);
  pinMode(CloseD4,OUTPUT);
  pinMode(OpenD5,OUTPUT);
  pinMode(CloseD5,OUTPUT);
  */

  close_all_doors();

  Serial.print("Setup complete -- doors closed (up)\n");

}

void loop() {

  // check if buttons are pressed
  button_open_D1();
  button_open_D2();
  button_open_D3();
  button_open_D4();
}

//---------------------------------------------
//---------------------------------------------
// functions for opening/closing doors

void close_all_doors() {
  for (int i = 0; i < NumPulses; i++) {
    digitalWrite(CloseD1, LOW);
    digitalWrite(CloseD2, LOW);
    digitalWrite(CloseD3, LOW);
    digitalWrite(CloseD4, LOW);
    digitalWrite(CloseD5, LOW);
    delay(PulseDuration); // is this needed?
    digitalWrite(CloseD1, LOW);
    digitalWrite(CloseD2, LOW);
    digitalWrite(CloseD3, LOW);
    digitalWrite(CloseD4, LOW);
    digitalWrite(CloseD5, LOW);
    delay(20);
  }
}

// general function for feathering pulses for opening/closing doors
void feather_pulses(int pin, int value) {
  for (int i = 0; i < NumPulses; i++) {
    digitalWrite(pin, value);
    delay(PulseDuration);
  }
}

void manual_open_D4() {
  for (int i = 0; i < NumPulses; i++) {
    digitalWrite(OpenD4, HIGH);
    delay(PulseDuration);
    digitalWrite(OpenD4, HIGH);
    delay(20);
  }
}

//---------------------------------------------
//---------------------------------------------
// start of button functions
// - drive each door with a button

void button_open_D1() {
  if(!digitalRead(button_openD1)) {
    // open_gate_D1();
    Serial.print("button d1 pressed\n");
  }
}

void button_open_D2() {
  if(!digitalRead(button_openD2)) {
    // open_gate_D2();
    Serial.print("button d2 pressed\n");
  }
}

void button_open_D3() {
  if(!digitalRead(button_openD3)) {
    // open_gate_D3();
    Serial.print("button d3 pressed\n");
  }
}

void button_open_D4() {
  if(!digitalRead(button_openD4)) {
    manual_open_D4();
    Serial.print("button d4 pressed\n");
  }
}
