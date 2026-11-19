#include "rtc.h"

#include <DS1307RTC.h>
#include <stdio.h>
#include <string.h>


namespace rtcModule {
bool read(tmElements_t& time) {
  return RTC.read(time);
}

bool chipPresent() {
  return RTC.chipPresent();
}
bool write(tmElements_t &time) {
  return RTC.write(time);
}

bool getCompileTime(tmElements_t &t) {
  const char *months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
  char mon[4];
  int d, y, h, m, s;
  if (sscanf(__DATE__, "%3s %d %d", mon, &d, &y) != 3) return false;
  if (sscanf(__TIME__, "%d:%d:%d", &h, &m, &s) != 3) return false;
  int mi = -1;
  for (int i = 0; i < 12; i++) if (strcmp(mon, months[i]) == 0) mi = i;
  if (mi < 0) return false;
  t.Day = d; t.Month = mi + 1; t.Year = CalendarYrToTm(y);
  t.Hour = h; t.Minute = m; t.Second = s;
  return true;
}

}

