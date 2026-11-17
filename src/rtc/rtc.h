#ifndef RTC_MODULE_H
#define RTC_MODULE_H

#include <TimeLib.h>

namespace rtcModule {
bool read(tmElements_t& time);
bool chipPresent();
}

#endif
