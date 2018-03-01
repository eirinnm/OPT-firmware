#include <arduino.h>
#include <keyboard.h>
#define DIR_PIN 2
#define STEP_PIN 3
#define EN_PIN 6
#define MS3 4
#define MS2 5
#define MS1 7
#define BUTTON1 A5
#define BUTTON2 A4

const byte seconds_per_rev = 12;
const int full_steps_per_rev = 200;
int microstepping = 16; 
int steps_per_second = microstepping * full_steps_per_rev / seconds_per_rev;
unsigned long button1_debounce = 0;
unsigned long button2_debounce = 0;
bool running = false;

void setup(){
  Serial.begin(115200);
  Keyboard.begin();
  pinMode(STEP_PIN,OUTPUT);
  pinMode(DIR_PIN,OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(MS1,OUTPUT);
  pinMode(MS2,OUTPUT);
  pinMode(MS3,OUTPUT);
  digitalWrite(MS1,HIGH);
  digitalWrite(MS2,HIGH);
  digitalWrite(MS3,HIGH);
}

void startRotation(){
  pinMode(EN_PIN, OUTPUT); //enable the pin to pull it down
  running = true;
  tone(STEP_PIN,steps_per_second);
  //acceleration profile
  // int maxSpeed = steps_per_second; //steps per second (Hz)
  // //get to max speed in 80ms
  // for(int i=1;i<=8;i++){
  //   int currentSpeed = maxSpeed * i / 8;
  //   NewTone(STEP_PIN,currentSpeed);
  //   delay(10);
  // }
}
void stopRotation(){
  pinMode(EN_PIN, INPUT); //back to open-drain
  running=false;
  noTone(STEP_PIN);
}
void reverseDirection(){
  digitalWrite(DIR_PIN, !digitalRead(DIR_PIN));
}
void loop(){
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
  //poll the buttons
  if(!digitalRead(BUTTON1) && millis()>button1_debounce+200){
    button1_debounce=millis();
    if(!running){
      startRotation();
    }else{
      stopRotation();
    }
  }
  if(!digitalRead(BUTTON2) && millis()>button2_debounce+500){
    button2_debounce=millis();
    Keyboard.write(KEY_F10);
  }
}
