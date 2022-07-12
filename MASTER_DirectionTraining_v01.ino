//---------------------------------------------------
//   CODE FOR WILKE MAZE MASTER ARDUINO CONTROLLER
//---------------------------------------------------
//                   GOALS
// - Train the mice to run in a single direction through the maze
// - provide a base template which declares all pins and core
//   functions. Use this to build up more complicated
//   protocols
//---------------------------------------------------

//      *********** IMPORTANT DETAIL
// this code is meant to run on an arduino MEGA / MEGA 2560
// In order to successfully upload this code, you will need
// to change the "board" settings in the arduino IDE (i.e. 
// the program you're using to view this code). To do this,
// look at the toolbar at the top of the screen and click:
// Tools > Board > Arduino Mega

//---------------------------------------------------
//                    DEVELOPMENT LOG
//---------------------------------------------------
// v01    by Thomas Elston; telston@nurhopsi.org
//---------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                    HIGH LEVEL SETTINGS
//--------------------------------------------------------------------------------------------------
int LapsBeforeReversal    = 3; // forces animals to run L/R for X consecutive trials before reversing
int LapCounter            = 0; // initialize to 0 and increment on each trial
int LapRewardSize         = 2; // 1 = small, 2 = medium, 3 = large rewards
                               // for directional training, constant reward size is good
int ForcedLeft            = 1; // first block of LapsBeforeReversal will force animal to the left maze arm
                               // --> will reverse (to right maze arm) after LapsBeforeReversal trials.   
                               // set to -1 if you want it to start by forcing the animals rightward                                      

boolean StartTrialsWithTouchScreen = false; // use beam break on IR_start to initiate trials
                                            // useful for first stages of training... 
                                            // set to "true" to use TS
//--------------------------------------------------------------------------------------------------


// BOOLEAN SWITCHES (logical variables)
boolean AllowTrial2Start = true; // this is a boolean approach to sensor de-bouncing
boolean TrialStarted = false;

boolean CheckLeftIRPostVertexSensor   = false;
boolean CheckRightIRPostVertexSensor  = false;
boolean CheckLeftIRPostBarrierSensor  = false;
boolean CheckRightIRPostBarrierSensor = false;
boolean CheckRightStartBoxSensor      = false;
boolean CheckLeftStartBoxSensor       = false;
boolean StartITI                      = false;

int SwitchSign = -1; // this drives the reversal - causes ForcedLeft to oscillate between 1 and -1

                                            
//--------------------------------------------------------------------------------------------------
//                                    DEFINE PIN NAMES
//--------------------------------------------------------------------------------------------------
// IR PHOTOBEAMS
// ** NOTE: these are PULLUP sensors, so, you detect them by looking
//          for their pins going LOW (rather than HIGH)
int IR_Start             = 22; // backup for touchscreen
int IR_Midstem           = 23; //  could be used to trigger LEDs, etc
int IR_RightPostVertex   = 24;                        
int IR_RightPostBarrier  = 25;
int IR_LeftPostVertex    = 26;
int IR_LeftPostBarrier   = 27;
int IR_RightStartBox     = 28;
int IR_LeftStartBox      = 29;

// MOTOR COMMANDS - wires connected to the MOTOR arduino
int OpenMidstemAndRight  = 32; // connected to pin 36 on motor arduino
int OpenMidstemAndLeft   = 34; // connected to 34 on motor arduino
int OpenVertexAndMidstem = 10; // pin 10 on motor arduino - this would be for free choices in other paradigms
int CloseVertexOpenRight = 33; // connected to pin 37 on motor arduino - also opens RIGHT STARTBOX
int CloseVertexOpenLeft  = 35; // connected to 35 on motor arduino - also opens LEFT STARTBOX
int CloseRightStartBox   = 39; // pin 31 on motor arduino
int CloseLeftStartBox    = 41; // pin 29 on motor arduino

// REWARD COMMANDS - wires connected to the REWARD arduino
int REW_SmallLeft        = 2; // pin 8 on reward arduino
int REW_MediumLeft       = 3; // pin 9 on reward arduino
int REW_LargeLeft        = 4; // pin 10 on reward arduino
int REW_SmallRight       = 5; // pin 11 on reward arduino
int REW_MediumRight      = 6; // pin 12 on reward arduino
int REW_LargeRight       = 7; // pin 13 on reward arduino

// BARRIER CONTROL STUFF - these wires connect to an L298N
// used to control speed of right barrier movement
int EngageRightMotor     = 9; // connected to EnA 
int EngageLeftMotor      = 8; // connected to EnB

int PushRightBarrier     = 14; // connected to In1
int PullRightBarrier     = 15; // connected to In2
int PushLeftBarrier      = 16; // connected to In3
int PullLeftBarrier      = 17; // connected to In4

// LEDs at vertex
int LeftLED1             = A1;
int LeftLED2             = A2;
int LeftLED3             = A3;
int RightLED1            = A4;
int RightLED2            = A5;
int RightLED3            = A6;

// CAPACATIVE TOUCH SENSOR - guarantees touchscreen functionality
int TouchSensor          = 12; 

// TOUCHSCREEN STUFF

//--------------------------------------------------------------------------------------------------


void setup() {
  // put your setup code here, to run once:
pinMode(IR_Start,INPUT_PULLUP);
pinMode(IR_Midstem,INPUT_PULLUP);
pinMode(IR_RightPostVertex,INPUT_PULLUP);
pinMode(IR_RightPostBarrier,INPUT_PULLUP);
pinMode(IR_LeftPostVertex,INPUT_PULLUP);
pinMode(IR_LeftPostBarrier,INPUT_PULLUP);
pinMode(IR_RightStartBox,INPUT_PULLUP);
pinMode(IR_LeftStartBox,INPUT_PULLUP);


pinMode(OpenMidstemAndRight,OUTPUT);
pinMode(OpenMidstemAndLeft,OUTPUT);
pinMode(OpenVertexAndMidstem,OUTPUT);

pinMode(CloseVertexOpenRight,OUTPUT);
pinMode(CloseVertexOpenLeft,OUTPUT);
pinMode(CloseRightStartBox,OUTPUT);
pinMode(CloseLeftStartBox,OUTPUT);

pinMode(REW_SmallLeft,OUTPUT);
pinMode(REW_MediumLeft,OUTPUT);
pinMode(REW_LargeLeft,OUTPUT);
pinMode(REW_SmallRight,OUTPUT);
pinMode(REW_MediumRight,OUTPUT);
pinMode(REW_LargeRight,OUTPUT);

pinMode(EngageRightMotor,OUTPUT);
pinMode(EngageLeftMotor,OUTPUT);
pinMode(PushRightBarrier,OUTPUT);
pinMode(PullRightBarrier,OUTPUT);
pinMode(PushLeftBarrier,OUTPUT);
pinMode(PullLeftBarrier,OUTPUT);

pinMode(LeftLED1,OUTPUT);
pinMode(LeftLED2,OUTPUT);
pinMode(LeftLED3,OUTPUT);
pinMode(RightLED1,OUTPUT);
pinMode(RightLED2,OUTPUT);
pinMode(RightLED3,OUTPUT);

pinMode(TouchSensor,INPUT);



} // end of setup

void loop() {
  // put your main code here, to run repeatedly:
   CheckForTrialStart();
   StartTrial();
   CheckLeftPostVertex(); // closes doors and delivers rewards
   CheckRightPostVertex();
   CloseRightStartBox_fxn();
   CloseLeftStartBox_fxn();
   ITI();
} // of declaring the functions written below



//---------------------------------------------------
//          Functions go below here
//---------------------------------------------------

void CheckForTrialStart(){
if(AllowTrial2Start == true){

// first find out whether to check the touchscreen or the IR beam
// check the touchscreen
if(StartTrialsWithTouchScreen == true){
  if(digitalRead(TouchSensor==HIGH)){
    TrialStarted = true;
    AllowTrial2Start = false;
  } // end of checking touch screen   
  } // end of whether we can check the touch screen

if(StartTrialsWithTouchScreen == false){
if(digitalRead(IR_Start==LOW)){
  TrialStarted = true;
  AllowTrial2Start = false;
} // end of checking starbox IR sensor 
} // end of checking whether to use the sensor

} // end of what to do if we are allowed to check the sensors 
} // end of CheckForTrialStart
//--------------------------------------------------------------------------------------------------



// NOW THAT A TRIAL START HAS BEEN DETECTED, FIGURE OUT WHICH DOORS TO OPEN
void StartTrial(){
if(TrialStarted == true){
   TrialStarted = false;
 
LapCounter = LapCounter+1; // increment the lap counter

// force the animal to the left or right maze arm?
if(ForcedLeft == 1){
digitalWrite(OpenMidstemAndLeft,HIGH);
delay(20); // leave the pin high for 20ms
digitalWrite(OpenMidstemAndLeft,LOW);
CheckLeftIRPostVertexSensor = true; // keep an eye on the Left post-vertex IR sensor
   
} // of forcing the animal to left maze arm
else { // the animal must be forced to the right

digitalWrite(OpenMidstemAndRight,HIGH);
delay(20); // leave the pin high for 20ms
digitalWrite(OpenMidstemAndRight,LOW);
CheckRightIRPostVertexSensor = true; // keep an eye on the Right post-vertex IR sensor
  
} // end of forcing the animal right 
}// end of checking if trial started
} // end of StartTrial
//--------------------------------------------------------------------------------------------------



// NOW CHECK THE POST-VERTEX SENSORS to open/close doors and deliver reward
void CheckLeftPostVertex(){
     if(CheckLeftIRPostVertexSensor == true){
       if(digitalRead(IR_LeftPostVertex == LOW)){
         CheckLeftIRPostVertexSensor = false;
         
         // close the doors and deliver reward
         digitalWrite(CloseVertexOpenLeft,HIGH);
         delay(20);
         
         // administer reward
         if(LapRewardSize == 1){
          digitalWrite(REW_SmallLeft,HIGH);
          delay(20);    
          digitalWrite(REW_SmallLeft,LOW);     
         }

         if(LapRewardSize == 2){
          digitalWrite(REW_MediumLeft,HIGH);
          delay(20);   
          digitalWrite(REW_MediumLeft,LOW);      
         }

         if(LapRewardSize == 3){
          digitalWrite(REW_LargeLeft,HIGH);
          delay(20);  
          digitalWrite(REW_LargeLeft,LOW);       
         } 

         CheckLeftStartBoxSensor = true;
   
       } // end of actually checking the sensor   
     } // end of knowing whether to check this sensor 
} // end of CheckLeftPostVertex


// RIGHT post-vertex
void CheckRightPostVertex(){
     if(CheckRightIRPostVertexSensor == true){
       if(digitalRead(IR_RightPostVertex == LOW)){
         CheckRightIRPostVertexSensor = false;
         
         // close the doors and deliver reward
         digitalWrite(CloseVertexOpenRight,HIGH);
         delay(20);
         
         // administer reward
         if(LapRewardSize == 1){
          digitalWrite(REW_SmallRight,HIGH);
          delay(20); 
          digitalWrite(REW_SmallRight,LOW);       
         }

         if(LapRewardSize == 2){
          digitalWrite(REW_MediumRight,HIGH);
          delay(20);
          digitalWrite(REW_MediumRight,LOW);         
         }

         if(LapRewardSize == 3){
          digitalWrite(REW_LargeRight,HIGH);
          delay(20);       
          digitalWrite(REW_LargeRight,LOW);  
         }

         CheckRightStartBoxSensor = true;
              
       } // end of actually checking the sensor   
     } // end of knowing whether to check this sensor 
} // end of CheckRightPostVertex
//--------------------------------------------------------------------------------------------------

// NOW CLOSE THE STARTBOX AND START AN INTERTRIAL-INTERVAL
//-- having an ITI will give you a defined period in which to carry out reversals and move the barriers

void CloseRightStartBox_fxn(){
if(CheckRightStartBoxSensor == true){
  if(digitalRead(IR_RightStartBox==LOW)){
    CheckRightStartBoxSensor = false;
    digitalWrite(CloseRightStartBox,HIGH);
    delay(20);
    digitalWrite(CloseRightStartBox,LOW);
    StartITI = true;   
  } // end of actually checking the sensor 
}// end of knowing whether to check this sensor
} // end of CloseRightStartBox_fxn

void CloseLeftStartBox_fxn(){
if(CheckLeftStartBoxSensor == true){
  if(digitalRead(IR_LeftStartBox==LOW)){
    CheckLeftStartBoxSensor = false;
    digitalWrite(CloseLeftStartBox,HIGH);
    delay(20);
    digitalWrite(CloseLeftStartBox,LOW);
    StartITI = true;   
  } // end of actually checking the sensor 
}// end of knowing whether to check this sensor
} // end of CloseLeftStartBox


void ITI(){
if(StartITI == true){
StartITI = false;
 // CHECK FOR REVERSAL
// if LapCounter has reached LapsBeforeReversal, reset and carry out reversal
if(LapCounter == LapsBeforeReversal){
LapCounter = 0; // reset the counter

ForcedLeft = ForcedLeft*SwitchSign; // this causes ForcedLeft to oscillate between -1 and 1 across blocks
                            // if you will only ever have 2 configurations/session, this is an elegant way to do it
                            // othewise, you will need a series of if statements
                            
  // THIS IS ALSO WHERE YOU WILL DO STUFF WITH THE BARRIERS (e.g. moving them up/down)                                                                               
} // end of checking for reversal
} // end of checking whether it's OK to do the ITI 
} // end of ITI function
