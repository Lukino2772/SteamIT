#ifndef STEAMIT_SENSORS_H
#define STEAMIT_SENSORS_H
#include <Arduino.h>

class SteamIT_Sensors {
private:
    static const int max_sensors = 8;
    int pins[max_sensors];
    int num_sensors;
    bool lastState[max_sensors];
    bool touched[max_sensors];
    unsigned long ltt;
    unsigned long timeout;
    unsigned long current;

public:
    SteamIT_Sensors();
    void begin(const int sensorPins[], int numPins, unsigned long timeoutMillis = 3000);
    void state_update();
    bool all_touched();
    bool left_touched();
    bool right_touched();
    bool middle_touched();
    bool timed_out();
    void reset();
};

#endif
