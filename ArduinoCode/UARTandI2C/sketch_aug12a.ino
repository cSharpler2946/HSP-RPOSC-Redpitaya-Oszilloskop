#include <Wire.h>

char * testMessage = "testMessage ";
int msgLen = 12;

void setup() {
  Serial.begin(4800, SERIAL_8E1);
  //Wire.begin();
}

void loop() {
  //Serial.write(0x0);
  Serial.write(testMessage);
  //Wire.beginTransmission(0x14);
  //Wire.write(testMessage, msgLen);
  //Wire.endTransmission();
}
