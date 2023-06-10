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
    void configureParams(int HRTime, int LRTime, int barrierRHeight, int barrierLHeight, float prob, char HRside);
    void begin();
    void resetBarriers();
    void configureBarriers();
    void startTimer();
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
    void activatePump(char rew, char pump, long prob);
    void PIRStartOpenD1D2D3();
    void PIRMidstemCloseD1();
    void PIRLPBCloseD2D3D5OpenD4();
    void PIRRPBCloseD3D2D4OpenD5();
    void PIRLSBCloseD4();
    void PIRRSBCloseD5();
    void playWithBarriers();

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
    
    // timestamp variables
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
    
    // reward
    int pump1Output = 5;
    int pump2Output = 2;
    
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
    
    // choosing which side is the HR
    char _HRside;
    char _leftReward;
    char _rightReward;
    
    // choosing higher probability
    float _prob;
};

#endif