#include <Wire.h>
#include <TimeLib.h>

#include "controls/controls.h"
#include "display/display.h"
#include "i2c_scanner/i2c_scanner.h"
#include "rtc/rtc.h"
#include "sensor/sensor.h"

tmElements_t KT;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(100000);   // slow the I2C bus to 100 kHz
  Serial.println("1. setup start");

  displayModule::begin();
  Serial.println("2. display init done");

  // Show something immediately, before touching the RTC
  displayModule::showStartupMessage();
  Serial.println("3. OLED test text sent");

  i2cScannerModule::scan();

  sensorModule::begin();
  controlsModule::begin();
}

void loop() {
  bool ok = rtcModule::read(KT);
  Serial.print("RTC.read = ");
  Serial.print(ok ? "OK " : "FAIL ");
  if (ok) {
    Serial.print(KT.Hour); Serial.print(":");
    Serial.print(KT.Minute); Serial.print(":");
    Serial.println(KT.Second);
  } else {
    Serial.println(rtcModule::chipPresent() ? "(chip present but clock stopped)" : "(chip not found)");
  }

  displayModule::showClock(ok, KT);

  int value_A0 = sensorModule::read();
  Serial.println(value_A0);

  displayModule::update();
  delay(1000);

  controlsModule::update();
}
