#include "i2c_scanner.h"

#include <Arduino.h>
#include <Wire.h>

namespace i2cScannerModule {
void scan() {
  Serial.println("I2C scan:");
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("  found 0x");
      Serial.println(address, HEX);
    }
  }
  Serial.println("scan done");
}
}
