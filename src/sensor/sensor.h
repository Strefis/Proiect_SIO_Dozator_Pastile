#ifndef SENSOR_H
#define SENSOR_H

namespace sensorModule {
extern int detectionThreshold;

void begin();
int read();
}

#endif
