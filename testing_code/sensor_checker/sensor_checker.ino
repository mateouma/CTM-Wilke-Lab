#include <CTM_base.h>

CTM_base mazeProtocol;

char *strings[8];
char *ptr = NULL;

bool setupComplete = false;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);
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
      setupComplete = true;
      delay(2000);
      if (nForceTrials > 0) {
        mazeProtocol.enactForceTrials();
      }
    }
  } else if (setupComplete){
//      mazeProtocol.checkSensors();
      mazeProtocol.checkButtons();
      mazeProtocol.testSensors();
  }
}
