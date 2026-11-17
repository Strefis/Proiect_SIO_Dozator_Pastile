#include "sensor.h"

#include <Arduino.h>

namespace {
const int sensorPin = A0;
}

namespace sensorModule {
int detectionThreshold = 700;

void begin() {
  pinMode(sensorPin, INPUT);
}

int read() {
  return analogRead(sensorPin);
}
}
