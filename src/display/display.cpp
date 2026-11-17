#include "display.h"

#include <SSD1306.h>

namespace {
SSD1306 display;
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

void update() {
  display.update();
}
}
