#include "display.h"

#include <Arduino.h>
#include <SSD1306.h>

namespace {
  SSD1306 display;
  bool fingerprintStateActive = false;
  bool fingerprintStatePersistent = false;
  unsigned long fingerprintStateStartedAt = 0;
  const unsigned long fingerprintStateDuration = 2500;

  void renderFingerprintState(const char* title, const char* message) {
    display.clear();
    display.setScale(1);
    display.setCaret(0, 0);
    display.print(title);
    display.setCaret(0, 16);
    display.print(message);
    display.update();
  }
}

namespace displayModule {
  void begin() {
    display.init();
}

  void showStartupMessage() {
    display.clear();
    display.setScale(1);
    display.setCaret(0, 0);
    display.print("OLED OK");
    display.update();
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
    display.setCaret(0, 0);
    display.print(rtcReadOk ? "RTC OK" : "RTC FAIL");
    if (rtcReadOk) {
      display.setScale(2);
      display.setCaret(0, 15);
      display.print(time.Hour);
      display.print(":");
      display.print(time.Minute);
      display.print(":");
      display.print(time.Second);
    }
  }

  void showFingerprintRecognized(unsigned int id) {
    display.clear();
    display.setScale(1);
    display.setCaret(0, 0);
    display.print("FINGERPRINT OK");
    display.setCaret(0, 16);
    display.print("ID: ");
    display.print(id);
    display.update();

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
    display.update();
  }
}
