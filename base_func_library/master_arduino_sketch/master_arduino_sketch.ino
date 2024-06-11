// LCD Setup:
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 47, d5 = 45, d6 = 43, d7 = 49;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int Trial_Count;
int last_displayed_trial = -1;

#include <CTM_base.h>

CTM_base mazeProtocol;

char *strings[19];
char *ptr = NULL;

bool setupComplete = false;
bool trialsComplete = false;
bool shutdownComplete = false;
int count = 0;
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);

  digitalWrite(52, LOW);
  
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print(" Welcome to the");
  lcd.setCursor(0, 1);
  lcd.print("Automated T-Maze");

//  Serial.println("SetUp Completed");

}

void loop() {
  if (!setupComplete) {
    if (Serial.available() > 0) {
      String msg = Serial.readString();
//      Serial.println("Here");
//      Serial.flush();
//
//      String msg = "100,100,0,0,1,0,1,100,100,10,1,1,0,0,0,2000,5,5000,0.6,EOF";
      Serial.println(msg);
      Serial.flush();
      msg.trim();
      int msg_len = msg.length() + 1; 
      char msg_array[msg_len];
      msg.toCharArray(msg_array, msg_len);
      byte index = 0;
      ptr = strtok(msg_array, ",");

     while (ptr != NULL) {
       strings[index] = ptr;
       index++;
       ptr = strtok(NULL, ",");
     }

// // Add everything except for the array into strings
//       while (ptr != NULL && index < 18) {
//         strings[index] = ptr;
//         index++;
//         ptr = strtok(NULL, ",");
//       }
      
      
      int hr_pump_time = atoi(strings[0]);
      int lr_pump_time = atoi(strings[1]);
      int l_bar_height = atoi(strings[2]);
      int r_bar_height = atoi(strings[3]);
      float prob_HR = atof(strings[4]);
      float prob_LR = atof(strings[5]);
      int hr_side = atoi(strings[6]);
      /*if (strings[5] == "R")
        hr_side = 1;
      else if (strings[5] == "L")
        hr_side = 0;*/
      //Serial.print(strings[5]);
      int ITI = atoi(strings[7]);
      int delayTime = atoi(strings[8]);
      int totalSamples = atoi(strings[9]);
      int nForceTrials = 0;
      int ftSide = 1;
      int laserSelected = atoi(strings[10]);
      int laserMode = atoi(strings[11]);
      int laserPulseType = atoi(strings[12]);
      int laserSide = atoi(strings[13]);
      int laserOnSensor = atoi(strings[14]);
      unsigned long laserOnDelay = atof(strings[15]);
      int laserOffSensor = atoi(strings[16]);
      unsigned long laserOffDelay = atof(strings[17]);
      float laserProb = atof(strings[18]);


      
//      String laserPatternString = strings[17];
      //Converts input array (Passed as a string) into an array of ints.

      // int laserOnArray[totalSamples] = {};
      // index = 0;
      // //Capture Laser Array:
      // while (ptr != NULL) {
      //   laserOnArray[index] = atoi(ptr);
      //   index++;
      //   ptr = strtok(NULL, ",");
      // }



//      int laserOnArrayptr = &laserOnArray[0];
//      int laserPattern[] = {};
//      for(int i = 17; i < strings.length(); i++){
//        laserPattern[i-17] = atoi(strings[i]);
//      }


      mazeProtocol.configureParams(hr_pump_time, lr_pump_time,
                                   r_bar_height, l_bar_height, 
                                   prob_HR, prob_LR, 
                                   hr_side, 
                                   ITI, 
                                   delayTime, totalSamples, 
                                   nForceTrials, ftSide,
                                   laserSelected, laserMode,
                                   laserPulseType, laserSide,
                                   laserOnSensor, laserOnDelay,
                                   laserOffSensor, laserOffDelay,
                                   laserProb);

//      mazeProtocol.configureParams(100, 100,
//                                   10, 0, 
//                                   1, 0, 
//                                   1, 
//                                   100, 
//                                   100, 10, 
//                                   nForceTrials, ftSide,
//                                   laserSelected, laserMode,
//                                   laserPulseType, laserSide,
//                                   laserOnSensor, laserOnDelay,
//                                   laserOffSensor, laserOffDelay,
//                                   laserOnArray);
      mazeProtocol.begin();
      mazeProtocol.printConfigParams();

//    Prepare LCD Display:
      lcd.setCursor(0, 0);
      lcd.print("Current Trial:  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);

      setupComplete = true;
      delay(2000);
      if (nForceTrials > 0) {
        mazeProtocol.enactForceTrials();
      }
    }
  } else if (setupComplete && !trialsComplete){
      mazeProtocol.checkSensors();
      mazeProtocol.checkButtons();
      mazeProtocol.checkLaser();
      count++;

//    Space out write to the screen by 10,000 iterations:
      if(count > 10000){
        lcd.setCursor(0, 1);
        lcd.print(String(mazeProtocol.currTrial));


//      Check to see if the maze has been informed that the trials are complted, and if so, set a flag:
        if (Serial.available() > 0) {
          trialsComplete = true;
        }
         
        count = 0;
      }
     
  }else if(trialsComplete && !shutdownComplete){
//    Print the final code, we can add additional information here that is relavent. This will only execute once.
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Testing Complete");
      lcd.setCursor(0, 1);
      lcd.print(" Reset Maze Now");

      shutdownComplete = true;
  }
}
