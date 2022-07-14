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

Serial.begin(9600);
  
pinMode(IR_Start,INPUT_PULLUP);
pinMode(IR_Midstem,INPUT_PULLUP);
pinMode(IR_RightPostVertex,INPUT_PULLUP);
pinMode(IR_RightPostBarrier,INPUT_PULLUP);
pinMode(IR_LeftPostVertex,INPUT_PULLUP);
pinMode(IR_LeftPostBarrier,INPUT_PULLUP);
pinMode(IR_RightStartBox,INPUT_PULLUP);
pinMode(IR_LeftStartBox,INPUT_PULLUP);

Serial.print("Setup complete\n");
Serial.print("IR_Start state: ");
Serial.print(digitalRead(IR_Start));
Serial.print("\n");

} // end of setup

void loop() {
  // put your main code here, to run repeatedly:
  CheckIRStart();
  /*CheckIRMidstem();
  CheckIRRightPostVertex();
  CheckIRRightPostBarrier();
  CheckIRLeftPostVertex();
  CheckIRLeftPostBarrier();
  CheckIRRightStartBox();
  CheckIRLeftStartBox();
  */
} // of declaring the functions written below

void CheckIRStart(){
  if(!digitalRead(IR_Start)){
    Serial.print("IR Start Active\n");
  }
}

/*void CheckIRMidstem(){
  if(digitalRead(IR_Midstem==LOW)){
    Serial.print("IR Midstem Active");
  }
}

void CheckIRRightPostVertex(){
  if(digitalRead(IR_RightPostVertex == LOW)){
    Serial.print("IR Right Post Vertex Active/n");
  }
}
 */
