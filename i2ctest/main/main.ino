#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

int c = 1;
void loop() {
  Serial.println(c);
  Wire.beginTransmission(0x55);
  Wire.write(c);
  c = c + 1;
  Wire.endTransmission();

  delay(1000);
}
