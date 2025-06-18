#include "SteamIT_Sensors.h"

SteamIT_Sensors::SteamIT_Sensors() {
  num_sensors = 0;
  ltt = 0;
  timeout = 3000;
  current = 0;
  for (int i = 0; i < max_sensors; i++) {
    lastState[i] = LOW;
    touched[i] = false;
    pins[i] = -1;
  }
}

void SteamIT_Sensors::begin(const int sensorPins[], int numPins, unsigned long timeoutMillis) {
  num_sensors = numPins;
  timeout = timeoutMillis;

  for (int i = 0; i < num_sensors; i++) {
    pins[i] = sensorPins[i];
    pinMode(pins[i], INPUT);
    lastState[i] = LOW;
    touched[i] = false;
  }
}

void SteamIT_Sensors::state_update() {
  current = millis();
  for (int i = 0; i < num_sensors; i++) {
    bool currentState = digitalRead(pins[i]);
    if (currentState == HIGH && lastState[i] == LOW) {
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.println(" touched");
      touched[i] = true;
      ltt = current;
    }
    lastState[i] = currentState;
  }
}

bool SteamIT_Sensors::all_touched() {
  for (int i = 0; i < num_sensors; i++) {
    if (!touched[i]) return false;
  }
  return true;
}

bool SteamIT_Sensors::left_touched() {
  return touched[0] && touched[6] &&
         !touched[1] && !touched[2] && !touched[3] && !touched[4] && !touched[5];
}

bool SteamIT_Sensors::right_touched() {
  return touched[1] && touched[5] &&
         !touched[0] && !touched[2] && !touched[3] && !touched[4] && !touched[6];
}

bool SteamIT_Sensors::middle_touched() {
  return touched[6] && touched[5] && touched[3] && touched[4] &&
         !touched[1] && !touched[7] && !touched[0] && !touched[2];
}

bool SteamIT_Sensors::timed_out() {
  return (ltt > 0 && (millis() - ltt > timeout));
}

void SteamIT_Sensors::reset() {
  for (int i = 0; i < num_sensors; i++) {
    touched[i] = false;
    lastState[i] = LOW;
  }
  ltt = 0;
}
