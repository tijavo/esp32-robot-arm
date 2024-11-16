#include <Arduino.h>
#include <ESP32Servo.h>

u_int8_t servoPins[] = {4,5,6,7};

Servo servos[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(90);
  }
  

}

void loop() {
  // put your main code here, to run repeatedly:
}
