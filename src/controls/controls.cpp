#include "controls.h"

#include <Arduino.h>

namespace {
const int set_button_pin = 2;
const int panic_button_pin = 3;
const int reset_button_pin = 4;

const int red_led_pin = 5;
const int green_led_pin = 6;
const unsigned long set_long_press_duration = 2000; // 2 secunde apasare

bool set_button_pressed = false;
bool set_long_press_triggered = false;
bool set_long_press_pending = false;
unsigned long set_button_pressed_at = 0;
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

    set_button_pressed = false;
    set_long_press_triggered = false;
    set_long_press_pending = false;
  }

  void update() {
    bool set_button = digitalRead(set_button_pin);
    bool panic_button = digitalRead(panic_button_pin);
    bool reset_button = digitalRead(reset_button_pin);
    unsigned long now = millis();

    if (set_button) { // daca butonul de setare este apasat
      if (!set_button_pressed) { // daca nu a fost apasat anterior
        set_button_pressed = true;
        set_button_pressed_at = now;
        set_long_press_triggered = false;
      } else if (!set_long_press_triggered && ((now - set_button_pressed_at) >= set_long_press_duration)) { // daca a fost apasat suficient de mult timp
        set_long_press_pending = true; // marcheaza ca o apasare lunga a fost detectata
        set_long_press_triggered = true; // marcheaza ca apasarea lunga a fost deja procesata
      }
    } else {
      set_button_pressed = false;
      set_long_press_triggered = false;
    }

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

  bool consumeSetLongPress() {
    if (!set_long_press_pending) {
      return false;
    }
    set_long_press_pending = false;
    return true;
  }
}
