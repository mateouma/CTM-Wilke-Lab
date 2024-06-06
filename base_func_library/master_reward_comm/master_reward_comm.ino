int pump1Input = 40;
int pump2Input = 46; 

int pump1Output = 32;
int pump2Output = 26; 


void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);

pinMode(pump1Input, INPUT);
pinMode(pump2Input, INPUT);

pinMode(pump1Output, OUTPUT);
pinMode(pump2Output, OUTPUT);

// turn on internal pull-up resistors?
digitalWrite(pump1Input, HIGH);
digitalWrite(pump2Input, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  
pump1check();
pump2check();

}

void pump1check() {
  Serial.println(digitalRead(pump1Input));

  if (digitalRead(pump1Input) == 0) { // input reading as LOW
    digitalWrite(pump1Output, HIGH); // output written as HIGH, which turns the pump OFF
  } else {
    digitalWrite(pump1Output, LOW); // output written as LOW, which turns the pump ON
  }
}

void pump2check() {
  Serial.println(digitalRead(pump2Input));

  if (digitalRead(pump2Input) == 0) { // input reading as LOW
    digitalWrite(pump2Output, HIGH); // output written as HIGH, which turns the pump OFF
  } else {
    digitalWrite(pump2Output, LOW); // output written as LOW, which turns the pump ON
  }
}

void testPump() {
  
}
