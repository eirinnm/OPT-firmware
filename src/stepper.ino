#include <arduino.h>
#define STEP_PIN 2
#define EN_PIN 8
#define DIR_PIN 5

const byte stepping = 16;
int steps_per_rev = 400 * stepping;
float ms_per_step = 12000.0 / steps_per_rev;

void setup(){
  pinMode(STEP_PIN,OUTPUT);
  pinMode(DIR_PIN,OUTPUT);
  Serial.begin(115200);
}

void startRotation(){
  pinMode(EN_PIN, OUTPUT); //enable the pin to pull it down
}
void stopRotation(){
  pinMode(EN_PIN, INPUT); //back to open-drain
}
void reverseDirection(){
  digitalWrite(DIR_PIN, !digitalRead(DIR_PIN));
}

void loop(){
  digitalWrite(STEP_PIN,HIGH);
  delayMicroseconds(10);
  digitalWrite(STEP_PIN,LOW);
  delayMicroseconds(ms_per_step*1000-10);
  while (Serial.available() > 0) {
    char inChar = Serial.read(); // Read single available character, there may be more waiting
    switch(inChar){
      case '0': stopRotation();
                break;
      case '1': startRotation();
                break;
      case '2': reverseDirection();
                break;
      }
    }
}
