#ifndef DISPLAY_H
#define DISPLAY_H

#include <TimeLib.h>

namespace displayModule {
void begin();
void showStartupMessage();
void showClock(bool rtcReadOk, const tmElements_t& time);
void update();
}

#endif
