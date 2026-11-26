#include "controls.h"
#include <Arduino.h>

#define NOT_SETTING 0
#define SETTING_HOUR 1
#define SETTING_MINUTES 2
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
bool set_button_toggle = false;
unsigned long set_button_pressed_at = 0;

uint8_t set_state = NOT_SETTING;
uint8_t min_per_pill = 0;
uint8_t hour_per_pill = 0;
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
      // logica pentru setarea cooldown-ului pentru pastila
      if(set_button_pressed && !set_long_press_pending){
        switch(set_button){
          case NOT_SETTING: SETTING_HOUR;
          case SETTING_HOUR: SETTING_MINUTES;
          case SETTING_MINUTES: NOT_SETTING;
        }
      set_button_pressed = false;
      set_long_press_triggered = false;
      }
    }

  void setPillCooldown(){
    if(set_state == SETTING_HOUR){
      if(hour_per_pill < 12){
        hour_per_pill +=1;
      }else{
        hour_per_pill = 0; //daca a trecut de 12 ore se reseteaza la 0
      }
    }else if (set_state == SETTING_MINUTES){
      if(min_per_pill < 60){
        min_per_pill += 5;
      }
      else{ //daca a trecut de 60 de min se reseteaza la 0;
        min_per_pill = 0; 
      }
    }
  }

    //aici e momentan logica pentru setarea cooldown-ului la pastile
    if(panic_button && set_state){ //daca butonul de urgenta e apasat
      setPillCooldown()
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
