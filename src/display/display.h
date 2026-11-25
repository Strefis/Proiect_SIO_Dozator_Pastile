#ifndef DISPLAY_H
#define DISPLAY_H

#include <TimeLib.h>

namespace displayModule {
void begin();
void showStartupMessage();
void showClock(bool rtcReadOk, const tmElements_t& time);
void showFingerprintRecognized(unsigned int id);
void showFingerprintUnrecognized();
void showFingerprintEnrollment(const char* message);
void clearFingerprintState();
void update();
}

#endif
