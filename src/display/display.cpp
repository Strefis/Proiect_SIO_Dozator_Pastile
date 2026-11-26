#include "display.h"

#include <Arduino.h>
#include <GyverOLED.h>

namespace {
  GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> display;
  bool fingerprintStateActive = false;
  bool fingerprintStatePersistent = false;
  unsigned long fingerprintStateStartedAt = 0;
  const unsigned long fingerprintStateDuration = 2500;

  void renderFingerprintState(const char* title, const char* message) {
    display.clear();
    display.setScale(1);
    display.setCursor(0, 0);   // coloana in pixeli, rand in linii de 8 px
    display.print(title);
    display.setCursor(0, 2);   // = y 16 px
    display.print(message);
  }
}

namespace displayModule {
  void begin() {
    display.init();
    display.clear();
  }

  void showStartupMessage() {
    display.clear();
    display.setScale(1);
    display.setCursor(0, 0);
    display.print(F("OLED OK"));
  }

  void showClock(bool rtcReadOk, const tmElements_t& time) {
    if (fingerprintStateActive) {
      if (fingerprintStatePersistent || ((millis() - fingerprintStateStartedAt) < fingerprintStateDuration)) {
        return;
      }
      fingerprintStateActive = false;
    }

    display.clear();
    display.setScale(1);
    display.setCursor(0, 0);
    display.print(rtcReadOk ? F("RTC OK") : F("RTC FAIL"));
    if (rtcReadOk) {
      display.setScale(2);
      display.setCursor(0, 2);
      display.print(time.Hour);
      display.print(':');
      display.print(time.Minute);
      display.print(':');
      display.print(time.Second);
    }
  }

  void showFingerprintRecognized(unsigned int id) {
    display.clear();
    display.setScale(1);
    display.setCursor(0, 0);
    display.print(F("FINGERPRINT OK"));
    display.setCursor(0, 2);
    display.print(F("ID: "));
    display.print(id);

    fingerprintStateActive = true;
    fingerprintStatePersistent = false;
    fingerprintStateStartedAt = millis();
  }

  void showFingerprintUnrecognized() {
    renderFingerprintState("FINGERPRINT", "Not recognized");
    fingerprintStateActive = true;
    fingerprintStatePersistent = false;
    fingerprintStateStartedAt = millis();
  }

  void showFingerprintEnrollment(const char* message) {
    renderFingerprintState("ENROLLMENT", message);
    fingerprintStateActive = true;
    fingerprintStatePersistent = true;
  }

  void clearFingerprintState() {
    fingerprintStateActive = false;
    fingerprintStatePersistent = false;
  }

  void update() {
    // fara buffer, textul apare direct pe ecran; nu mai e nimic de trimis
  }
}