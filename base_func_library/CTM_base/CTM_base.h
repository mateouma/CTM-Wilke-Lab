/*
 CTM_base.h - Library for continuous T-maze base functions.
 Created by Mateo Umagung, May 31, 2023.
 Authorized for Wilke Lab use only.
 */
#ifndef CTM_base_h
#define CTM_base_h

#include "Arduino.h"

class CTM_base {
  public:
    CTM_base();
    void configureParams(int HRTime, int LRTime,
                         int barrierRHeight, int barrierLHeight,
                         float prob_HR, float prob_LR,
                         int HRside, int ITI, int delayTime,
                         int totalSamples,
                         int nForceTrials, int ftSide,
                         int laserSelected, int laserMode,
                         int laserPulseType, int laserSide,
                         int laserOnSensor, float laserOnDelay,
                         int laserOffSensor, float laserOffDelay,
                         float laserProb);
    void begin();

    // general functions
    void printConfigParams();
    void resetBarriers();
    void configureBarriers();
    void startTimer();

    // sensor functions
    void ActivatePIRStart();
    void ActivatePIRMidstem();
    void ActivatePIREndstem();
    void ActivatePIRLeftPostVertex();
    void ActivatePIRRightPostVertex();
    void ActivatePIRLeftPreBarrier();
    void ActivatePIRRightPreBarrier();
    void ActivatePIRLeftStartBox();
    void ActivatePIRRightStartBox();
    void checkSensors();
    void resetFlags();

    // pump function
    void activatePump(int rew, int pump, float prob_HR, float prob_LR);

    // sensor - door activation
    void PIRStartOpenD1D2D3();
    void PIRMidstemCloseD1();
    void PIRLPBCloseD2D3D5OpenD4();
    void PIRRPBCloseD3D2D4OpenD5();
    void PIRLSBCloseD4();
    void PIRRSBCloseD5();

    // force trial function
    void enactForceTrials();

    // Laser functions
    void checkLaser();

    // Button Functions
    void buttonD1Pressed();
    void buttonD2Pressed();
    void buttonD3Pressed();
    void buttonD4Pressed();
    void buttonD5Pressed();

    void buttonRBarrierPressed();
    void buttonLBarrierPressed();

    void buttonRPumpPressed();
    void buttonLPumpPressed();

    void checkButtons();

    // testing
    void playWithBarriers();
    void testPump();
    void testSensors();
    void printFlags();
    void calibrateBarriers();

    // Trial Counter
    int currTrial = 0;

  private:
    // flags
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

    // laser flags
    bool laserPrimed = true;
    bool laserActive = false;
    
    // timestamp variables
    unsigned long startTime;
    unsigned long currentTime;

    // Trial Counter
    int currFt = 1;

    // checking if force trials are active
    bool ftActive = false;

    // Door Status for Buttons
    bool isD1Open = false;
    bool isD2Open = false;
    bool isD3Open = false;
    bool isD4Open = false;
    bool isD5Open = false;
  
    // =============================
    // PINS
    // edit these variables if the pins change
    // =============================
    
    // Pressure Sensor
    #define FORCE_SENSOR_PIN A0 // the FSR and 10K pulldown are connected to A0

    // door outputs
    int outputD1 = 32;
    int outputD2 = 33;
    int outputD3 = 34;
    int outputD4 = 35;
    int outputD5 = 41;
    
    // barrier outputs
    int barrierR = 9;
    int barrierRpwm = 14; // control speed of right barrier
    int barrierL = 8;
    int barrierLpwm = 17; // control speed of left barrier
    
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

    int sensorArray[9] = {PIR_Start,
                         PIR_Midstem,
                         PIR_Endstem,
                         PIR_LeftPostVertex,
                         PIR_RightPostVertex,
                         PIR_LeftPreBarrier,
                         PIR_RightPreBarrier,
                         PIR_LeftStartBox,
                         PIR_RightStartBox};

    // reward
    int pump1Output = 5;
    int pump2Output = 2;

    // laser
    int laserOutput = 49; // Add correct pin in here.
    // int laserOnCondPin1;
    // int laserOnCondPin2;
    // int laserOffCondPin1;
    // int laserOffCondPin2;

    #define MAX_ULONG 4294967295
    bool laserOn = false; //0 means the laser is currently off, 1 means it is currently on.
    unsigned long laserOnTime = MAX_ULONG;
    unsigned long laserOffTime = MAX_ULONG;

    // Input Buttons
    int rightBarrierButton = 50;
    int leftBarrierButton = 51;

    int startDoorButton = 38;
    int backRightDoorButton = 46;
    int backLeftDoorButton = 42;
    int frontRightDoorButton = 6;
    int frontLeftDoorButton = 7;

    int rightPumpButton = 4;
    int leftPumpButton = 3;

    // ===================
    // VARIABLE PARAMETERS
    // ===================
  
    // type of trial
    // choose from ["Acclimation", "1v1", "3v1", "1v1B", "3v1B"]
    //char _trialType[]; 
    
    // amount of time to dispense rewards
    int _HRTime;
    int _LRTime;
    
    // barrier heights
    int _bRH;
    int _bLH;
    int _converted_bRH;
    int _converted_bLH;
    #define MAX_RIGHT_BARRIER_HEIGHT 2630
    #define MAX_LEFT_BARRIER_HEIGHT 2170
    
    // choosing which side is the HR
    int _HRside;
    int _leftReward;
    int _rightReward;
    
    // choosing higher probability
    float _prob_HR;
    float _prob_LR;

    // inter-trial interval
    int _ITI;

    // Time delay between when a sensor is tripped and when the door closes.
    int _delayTime;

    int _totalSamples;

    int _laserSelected; // 1 is yes, 0 is no
    int _laserMode; // 1 is sensor, 0 is manual
    int _laserPulseType;
    int _laserSide; // Both: 0; HR: 1; LR: -1
    int _laserOnSensor;
    float _laserOnTimeDelay;
    int _laserOffSensor;
    float _laserOffTimeDelay;
    float _laserProb;


    // number of force trials
    int _nForceTrials;

    // force trial side
    int _ftSide;

    // Button States
    int startDoorButtonState = 0;
    int backRightDoorButtonState = 0;
    int backLeftDoorButtonState = 0;
    int frontRightDoorButtonState = 0;
    int frontLeftDoorButtonState = 0;

    int rightBarrierButtonState = 0;
    int leftBarrierButtonState = 0;

    int rightPumpButtonState = 0;
    int leftPumpButtonState = 0;

    int _laserTrials[10] = {1,1,1,1,1,1,1,1,1,1};
};

#endif