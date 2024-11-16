#include <Arduino.h>

uint16_t* analogValues; 
uint8_t POTIENTIOMETER_PINS[] = {0,1,2,3};

void setup() {
  Serial.begin(9600);
  analogValues = new uint16_t[4];
  for (int i = 0; i < 4; i++) {
    analogValues[i] = 0;
  }
  // set the ADC attenuation to 11 dB (up to ~3.3V input)
  Serial.println("Setting ADC attenuation to 11 dB");
  analogSetAttenuation(ADC_11db);
}

void sendAnalogValues() {
  for(int i = 0; i < 4; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(analogValues[i]);
  }
}

// the loop routine runs over and over again forever:
void loop() {
  for(int i = 0; i < 4; i++) {
    uint16_t value = analogRead(POTIENTIOMETER_PINS[i]);
    if(value != analogValues[i]) {
      analogValues[i] = value;
      sendAnalogValues();
    }
  }
  delay(100);
}