#include "controls.h"

#include <Arduino.h>

namespace {
const int set_button_pin = 2;
const int panic_button_pin = 3;
const int reset_button_pin = 4;

const int red_led_pin = 5;
const int green_led_pin = 6;
}

namespace controlsModule {
void begin() {
  pinMode(set_button_pin, INPUT);
  pinMode(panic_button_pin, INPUT);
  pinMode(reset_button_pin, INPUT);

  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);

  digitalWrite(red_led_pin, LOW);
  digitalWrite(green_led_pin, LOW);
}

void update() {
  bool set_button = digitalRead(set_button_pin);
  bool panic_button = digitalRead(panic_button_pin);
  bool reset_button = digitalRead(reset_button_pin);

  if (set_button) {
    digitalWrite(red_led_pin, HIGH);
  } else if (panic_button) {
    digitalWrite(green_led_pin, HIGH);
  } else if (reset_button) {
    digitalWrite(red_led_pin, HIGH);
    digitalWrite(green_led_pin, HIGH);
  } else {
    digitalWrite(red_led_pin, LOW);
    digitalWrite(green_led_pin, LOW);
  }
}
}
