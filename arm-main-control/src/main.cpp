#include <Arduino.h>
#include <ESP32Servo.h>

u_int8_t servoPins[] = {4,5,6,7};

Servo servos[4];
u_int16_t analogValues[4];

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

void moveServos() {
  for(int i = 0; i < 4; i++) {
    Serial.print(analogValues[i]);
    Serial.print(" => ");
    Serial.print(mapValue(analogValues[i]));
    Serial.print(", ");
    servos[i].write(mapValue(analogValues[i]));
  }
  Serial.println(" moved");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() > 0) {
    for(int i = 0; i < 4; i++) {
      analogValues[i] = Serial.parseInt();
      Serial.print("Analog value read ");
      Serial.println(analogValues[i]);
    }
    moveServos();
  }
}
