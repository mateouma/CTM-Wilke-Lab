// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 47, d5 = 45, d6 = 43, d7 = 49;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int Trial_Count;
int last_displayed_trial = -1;

#include <CTM_base.h>

CTM_base mazeProtocol;

char *strings[8];
char *ptr = NULL;

bool setupComplete = false;
int count = 0;
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);
  // Trial_Count = 1;

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
//  lcd.print("Current Trial:");
//   lcd.print("Hello World");
  lcd.print(" Welcome to the");
  lcd.setCursor(0, 1);
  lcd.print("Automated T-Maze");


  
}

void loop() {
  if (!setupComplete) {
    if (Serial.available() > 0) {
      String msg = Serial.readString();
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
      int hr_pump_time = atoi(strings[0]);
      int lr_pump_time = atoi(strings[1]);
      int l_bar_height = atoi(strings[2]);
      int r_bar_height = atoi(strings[3]);
      float prob = atof(strings[4]);
      int hr_side = atoi(strings[5]);
      /*if (strings[5] == "R")
        hr_side = 1;
      else if (strings[5] == "L")
        hr_side = 0;*/
      //Serial.print(strings[5]);
      int ITI = atoi(strings[6]);
      int delayTime = atoi(strings[7]);
      int nForceTrials = 0;
      int ftSide = 1;

      mazeProtocol.configureParams(hr_pump_time, lr_pump_time, r_bar_height, l_bar_height, prob, hr_side, ITI, delayTime, nForceTrials, ftSide);
      mazeProtocol.begin();
      mazeProtocol.printConfigParams();
//      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Current Trial:  ");
      lcd.setCursor(0, 1);
//      lcd.setCursor(0, 0);
//      lcd.print("                ");
//      lcd.setCursor(0, 1);
//      lcd.print("                ");
      setupComplete = true;
      delay(2000);
      if (nForceTrials > 0) {
        mazeProtocol.enactForceTrials();
      }
    }
  } else if (setupComplete){

      
//      lcd.setCursor(0, 1);
//      lcd.print(mazeProtocol.currTrial);
//      Serial.println(mazeProtocol.currTrial);
      mazeProtocol.checkSensors();
      mazeProtocol.checkButtons();
      count++;

      if(count > 10000){
//        lcd.clear();
//          lcd.setCursor(0, 1);
//         lcd.print("Hello");
        // lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        // lcd.print(millis() / 1000);
//        lcd.setCursor(0, 0);
//        lcd.print("                ");
//        lcd.setCursor(0, 1);
//        lcd.print("                ");
//        lcd.clear();
//        lcd.setCursor(0, 0);        
//        lcd.print("Current Trial:");
        lcd.setCursor(0, 1);
        lcd.print(String(mazeProtocol.currTrial));
//        Serial.println("Current Trial:");
//        Serial.println(mazeProtocol.currTrial);
        
        count = 0;
      }

//      if(last_displayed_trial != mazeProtocol.currTrial){
//        lcd.clear();
//        lcd.setCursor(0, 0);        
//        lcd.print("Current Trial:");
//        lcd.setCursor(0, 1);
//        lcd.print(mazeProtocol.currTrial);
//        last_displayed_trial = mazeProtocol.currTrial;
//      }

     
  }
}














//void setup() {
//  // set up the LCD's number of columns and rows:
//  lcd.begin(16, 2);
//  // Print a message to the LCD.
//  lcd.print("hello, world!");
//}
//
//void loop() {
//  // set the cursor to column 0, line 1
//  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(0, 1);
//  // print the number of seconds since reset:
//  lcd.print(millis() / 1000);
//}
