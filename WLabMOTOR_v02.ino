//----------------------------------------------
//      CODE TO CONTROL WILKE MAZE DOORS 
//----------------------------------------------
//                   GOAL
// - open/close different doors in the maze 
//----------------------------------------------

//      *********** IMPORTANT DETAIL
// this code is meant to run on an arduino MEGA / MEGA 2560
// In order to successfully upload this code, you will need
// to change the "board" settings in the arduino IDE (i.e. 
// the program you're using to view this code). To do this,
// look at the toolbar at the top of the screen and click:
// Tools > Board > Arduino Mega

//----------------------------------------------
//              DEVELOPMENT LOG
//----------------------------------------------
// v01 by Thomas Elston; telston@nurhopsi.org
//----------------------------------------------
// v02 by Gary Wilke; coach.wilke@gmail.com
//----------------------------------------------
  
//             DOOR SPEED / AMOUNT
// I used a for-loop to "feather" the doors up/down
// in my experience, this was quieter. You will 
// definitely want to optimize time the doors are pulsed.
// My advice: start with a small number and go up. 
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


// let's give the pins sensible names
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
//----------------------------------------------

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
pinMode(sig_openD1andD2,INPUT);
pinMode(sig_CloseVertexOpenRight,INPUT);  
pinMode(sig_openD1andD3,INPUT);  
pinMode(sig_CloseVertexOpenLeft,INPUT);  
pinMode(sig_openVertex,INPUT);
pinMode(sig_closeVertex,INPUT);  

pinMode(sig_openD4,INPUT);  
pinMode(sig_closeD4,INPUT);  
pinMode(sig_openD5,INPUT);  
pinMode(sig_closeD5,INPUT);  

// use internal pullup resistors for the buttons
// means buttons are registerd as LOW digital signals
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

// START WITH ALL DOORS UP 'CLOSED'
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD1, LOW);
      digitalWrite(CloseD2, LOW);
      digitalWrite(CloseD3, LOW);
      digitalWrite(CloseD4, LOW);
      digitalWrite(CloseD5, LOW);
      delay(PulseDuration);// not sure I need this
      digitalWrite(CloseD1, LOW);
      digitalWrite(CloseD2, LOW);
      digitalWrite(CloseD3, LOW);
      digitalWrite(CloseD4, LOW);
      digitalWrite(CloseD5, LOW);
      delay(20);
      } // end of for-loop 

// if you won't use the midstem this session, 
// go ahead and open it up
  if(USEMIDSTEM ==false){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(OpenD1, HIGH);
      delay(PulseDuration);
      digitalWrite(OpenD1, HIGH);
      delay(20);
      } // end of for-loop 
  } // end of if
} // end of setup

void loop() {
  // put your main code here, to run repeatedly:
TTL_OpenMidstemAndRight();
TTL_OpenMidstemAndVertex();

TTL_OpenMidstemAndLeft();
TTL_CloseVertexOpenLeft();
TTL_CloseMidstemAndOpenRight();

TTL_CloseRightStartbox();
TTL_CloseLeftStartbox();

// button-driven door operation
button_OpenMidstem();
button_CloseMidstem();

button_OpenRight();
button_CloseRight();

button_OpenLeft();
button_CloseLeft();

button_OpenRightStartBox();
button_CloseRightStartBox();

button_OpenLeftStartBox();
button_CloseLeftStartBox();


} // end of main loop


void TTL_OpenMidstemAndRight(){
  if(digitalRead(sig_openD1andD2)){
    Serial.print("sig_openD1andD2 signal detected, opening midstem and right\n");
    for(int z = 0; z <NumPulses; z++){
      if(USEMIDSTEM ==true){
      digitalWrite(OpenD1, HIGH);
      } // end of if
      digitalWrite(OpenD2, HIGH);
      delay(PulseDuration);// not sure I need this
      if(USEMIDSTEM ==true){
      digitalWrite(OpenD1, HIGH);
      } // end of if
      digitalWrite(OpenD2, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of TTL_OpenMidstemAndRight

void TTL_CloseMidstemAndOpenRight(){
  if(digitalRead(sig_CloseVertexOpenRight)){
    Serial.print("sig_CloseVertexOpenRight detected, closing midstem and opening right\n");
    for(int z = 0; z <NumPulses; z++){
      if(USEMIDSTEM ==true){
      digitalWrite(CloseD1, LOW);
      } // end of if
      digitalWrite(OpenD2, HIGH);
// also open right startbox
        digitalWrite(OpenD5, HIGH);    
      delay(PulseDuration);// not sure I need this
      if(USEMIDSTEM ==true){
      digitalWrite(CloseD1, LOW);
      } // end of if
      digitalWrite(OpenD2, HIGH);
      digitalWrite(OpenD5, HIGH); 
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of TTL_CloseMidstemAndRight


void TTL_OpenMidstemAndLeft(){
  if(digitalRead(sig_openD1andD3)){
    for(int z = 0; z <NumPulses; z++){
      if(USEMIDSTEM ==true){
      digitalWrite(OpenD1, HIGH);
      } // end of if
      digitalWrite(OpenD3, HIGH);
      delay(PulseDuration);// not sure I need this
      if(USEMIDSTEM ==true){
      digitalWrite(OpenD1, HIGH);
      } // end of if
      digitalWrite(OpenD3, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of TTL_OpenMidstemAndLeft

void TTL_CloseVertexOpenLeft(){
  if(digitalRead(sig_CloseVertexOpenLeft)){
    for(int z = 0; z <NumPulses; z++){
      if(USEMIDSTEM ==true){
      digitalWrite(CloseD1, LOW);
      } // end of if
      digitalWrite(CloseD3, LOW);
      digitalWrite(CloseD2, LOW);
      // also open left startbox
      digitalWrite(OpenD5, HIGH);
      delay(PulseDuration); // not sure I need this
      if(USEMIDSTEM ==true){
      digitalWrite(CloseD1, LOW);
      } // end of if
      digitalWrite(CloseD2, LOW);
      digitalWrite(CloseD3, LOW);
      digitalWrite(OpenD5, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of TTL_CloseVertexOpenLeft



void TTL_OpenMidstemAndVertex(){
  if(digitalRead(sig_openD1andD3)){
    for(int z = 0; z <NumPulses; z++){
      if(USEMIDSTEM ==true){
      digitalWrite(OpenD1, HIGH);
      } // end of if
      digitalWrite(OpenD2, HIGH);
      digitalWrite(OpenD3, HIGH);
      delay(PulseDuration);// not sure I need this
      if(USEMIDSTEM ==true){
      digitalWrite(OpenD1, HIGH);
      } // end of if
      digitalWrite(OpenD2, HIGH);
      digitalWrite(OpenD3, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of TTL_OpenMidstemAndVertex


void TTL_CloseRightStartbox(){
  if(digitalRead(sig_closeD4)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD4, LOW);
      delay(PulseDuration);
      digitalWrite(CloseD4, LOW);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of TTL_OpenRightStartbox



void TTL_CloseLeftStartbox(){
  if(digitalRead(sig_closeD5)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD5, LOW);
      delay(PulseDuration);// not sure I need this
      digitalWrite(CloseD5, LOW);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of TTL_CloseLeftStartbox



//---------------------------------------------
//---------------------------------------------
// start of button functions
// - drive each door with a button

void button_OpenMidstem(){
  if(!digitalRead(button_openD1)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(OpenD1, HIGH);
      delay(PulseDuration);// not sure I need this
      digitalWrite(OpenD1, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_OpenMidstem

void button_CloseMidstem(){
  if(!digitalRead(button_closeD1)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD1, LOW);
      delay(PulseDuration);// not sure I need this
      digitalWrite(CloseD1, LOW);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_CloseMidstem

void button_OpenRight(){
  if(!digitalRead(button_openD2)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(OpenD2, HIGH);
      delay(PulseDuration);// not sure I need this
      digitalWrite(OpenD2, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_OpenRight

void button_CloseRight(){
  if(!digitalRead(button_closeD2)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD2, LOW);
      delay(PulseDuration);// not sure I need this
      digitalWrite(CloseD2, LOW);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_CloseRight

void button_OpenLeft(){
  if(!digitalRead(button_openD3)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(OpenD3, HIGH);
      delay(PulseDuration);// not sure I need this
      digitalWrite(OpenD3, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_OpenLeft

void button_CloseLeft(){
  if(!digitalRead(button_closeD3)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD3, LOW);
      delay(PulseDuration);
      digitalWrite(CloseD3, LOW);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_CloseLeft


void button_OpenRightStartBox(){
  if(!digitalRead(button_openD4)){
    Serial.print("button_openD4 detected, opening right start box\n");
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(OpenD4, HIGH);
      delay(PulseDuration);// not sure I need this
      digitalWrite(OpenD4, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_OpenRightStartBox

void button_CloseRightStartBox(){
  if(!digitalRead(button_closeD4)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD4, LOW);
      delay(PulseDuration);// not sure I need this
      digitalWrite(CloseD4, LOW);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_CloseRightStartBox

void button_OpenLeftStartBox(){
  if(!digitalRead(button_openD5)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(OpenD5, HIGH);
      delay(PulseDuration);// not sure I need this
      digitalWrite(OpenD5, HIGH);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_OpenLeftStartBox

void button_CloseLeftStartBox(){
  if(!digitalRead(button_closeD5)){
    for(int z = 0; z <NumPulses; z++){
      digitalWrite(CloseD5, LOW);
      delay(PulseDuration);// not sure I need this
      digitalWrite(CloseD5, LOW);
      delay(20);
      } // end of for-loop   
  } // of if statement
} // end of button_CloseLeftStartBox
