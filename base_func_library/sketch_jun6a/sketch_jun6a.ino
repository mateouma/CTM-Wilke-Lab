#include <CTM_base.h>

CTM_base mazeProtocol;

//String rawInput;

//char *conditions[7];
//char *ptr = NULL;

int hr_pump_time = 3000;
int lr_pump_time = 1000;
int l_bar_height = 0;
int r_bar_height = 0;
float prob = 0.8;
char hr_side = 'L';

//bool gui_input_received = false;
//bool configured = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mazeProtocol.configureParams(hr_pump_time, lr_pump_time, r_bar_height, l_bar_height, prob, hr_side);
  mazeProtocol.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //setupConditions();
  mazeProtocol.checkSensors();
}

/*
void setupConditions() {
  if (!gui_input_received) {
    while (Serial.available() == 0) {}   
    
    rawInput = Serial.readString();
    int str_len = rawInput.length() + 1; 
    char char_array[str_len];
    rawInput.toCharArray(char_array, str_len);

    byte index = 0;
    ptr = strtok(char_array, ",");

    while (ptr != NULL) {
      conditions[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");
    }
    
    hr_side = conditions[0];
    hr_pump_time = atoi(conditions[1]);
    lr_pump_time = atoi(conditions[2]);
    l_bar_height = atoi(conditions[3]);
    r_bar_height = atoi(conditions[4]);
    prob = atof(conditions[5]);
    Serial.println(hr_pump_time);

    // configure setup
    mazeProtocol.configureParams(hr_pump_time, lr_pump_time, r_bar_height, l_bar_height, prob, hr_side);
    mazeProtocol.begin();
    
    configured = true;
    gui_input_received = true;
  }
}
*/


