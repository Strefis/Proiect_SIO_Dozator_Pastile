#include "rtc.h"

#include <DS1307RTC.h>

namespace rtcModule {
bool read(tmElements_t& time) {
  return RTC.read(time);
}

bool chipPresent() {
  return RTC.chipPresent();
}
}
