#include <Arduino.h>
#include <ESP32Servo.h>
#include <list>
#include <array>
#include <iterator>
#define SAVE_BTN 8
#define GO_BTN 9

u_int8_t servoPins[] = {1,4,3,2};


Servo servos[4];
std::array<u_int16_t, 4> analogValues;
std::list<std::array<u_int16_t, 4>> savedPoints;

bool isSaveBtnPressed = false;
bool isGoBtnPressed = false;


enum Mode {
  MANUAL,
  AUTO
};

Mode mode = MANUAL;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

void IRAM_ATTR onSaveBtnPressed() {
  button_time = millis();
  if(button_time - last_button_time > 250) {
    isSaveBtnPressed = true;
    last_button_time = button_time;
  }
}

void IRAM_ATTR onGoBtnPressed() {
  button_time = millis();
  if(button_time - last_button_time > 250) {
    isGoBtnPressed = true;
    last_button_time = button_time;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");
  for(int i = 0; i < 4; i++) {
    
    servos[i].attach(servoPins[i]);
  }
  analogSetAttenuation(ADC_11db);
  pinMode(SAVE_BTN, INPUT_PULLUP);
  pinMode(GO_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SAVE_BTN), onSaveBtnPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(GO_BTN), onGoBtnPressed, RISING);
}

u_int8_t mapValue(u_int16_t value) {
  return map(value, 0, 4095, 0, 180);
}

void savePoint() {
  Serial.println("Saving point...");
  savedPoints.push_back(analogValues);
}


void moveServos() {
  for(int i = 0; i < 4; i++) {
    Serial.print(" ");
    Serial.print(mapValue(analogValues[i]));
    servos[i].write(mapValue(analogValues[i]));
  }
  Serial.println(" moved");
}


std::list<std::array<u_int16_t, 4>>::iterator it;
void goThroughPoints() {
  it = savedPoints.begin();
  mode = AUTO;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mode == AUTO) {
    if(it != savedPoints.end()) {
      analogValues = *it;
      moveServos();
      it++;
      delay(1000);
    } else {
      it = savedPoints.begin();
    }

    if(isGoBtnPressed) {
      mode = MANUAL;
      isGoBtnPressed = false;
    }

  }else{
    for(int i = 0; i < 4; i++) {
      uint16_t value = analogRead(POTIENTIOMETER_PINS[i]);
      if(value != analogValues[i]) {
        analogValues[i] = value;
        Serial.print("Analog value ");
        Serial.print(value);
        moveServos();
      }
    }

    if (isSaveBtnPressed) {
      Serial.println("Save button pressed");
      savePoint();
      isSaveBtnPressed = false;
    } else if (isGoBtnPressed) {
      Serial.println("Go button pressed");
      goThroughPoints();
      isGoBtnPressed = false;
    }
    
    delay(100);
  }
  
}
