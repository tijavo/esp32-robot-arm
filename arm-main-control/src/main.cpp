#include <Arduino.h>
#include <ESP32Servo.h>
#include <list>

u_int8_t servoPins[] = {4,5,6,7};

Servo servos[4];
std::array<u_int16_t, 4> analogValues;
std::list<std::array<u_int16_t, 4>> savedPoints;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");
  for(int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
  }
}

u_int8_t mapValue(u_int16_t value) {
  return map(value, 0, 4095, 0, 180);
}

void savePoint() {
  savedPoints.push_back(analogValues);
}

void goThroughPoints() {
  for(auto point : savedPoints) {
    analogValues = point;
    moveServos();
    delay(1000);
  }
}

void moveServos() {
  for(int i = 0; i < 4; i++) {
    Serial.print("Servo ");
    Serial.print(i);
    Serial.print(" => ");
    Serial.print(analogValues[i]);
    Serial.print("(");
    Serial.print(mapValue(analogValues[i]));
    Serial.print("°), ");
    servos[i].write(mapValue(analogValues[i]));
  }
  Serial.println(" moved");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() > 0) {
    for(int i = 0; i < 4; i++) {
      Serial.print("Enter Number for Servo ");
      Serial.println(i);
      analogValues[i] = Serial.parseInt();
    }
    moveServos();
  }
}
