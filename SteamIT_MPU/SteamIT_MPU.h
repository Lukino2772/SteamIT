#ifndef STEAMIT_MPU_H
#define STEAMIT_MPU_H
#include <Wire.h>
#include <EEPROM.h>
#include <Arduino.h>
#define address 0x68
#define start_address 0
#define alpha 0.96f

class SteamIT_MPU {
public:
    SteamIT_MPU();
    ~SteamIT_MPU();
    void begin();
    void calibrate();
    void readSensorData();
    float getPitch() const;
    float getRoll() const;
    float getYaw() const;

private:
    void recoverSensor();
    void applyCalibration();
    void computeAngles();

    int16_t rawAccelX, rawAccelY, rawAccelZ;
    int16_t rawGyroX, rawGyroY, rawGyroZ;

    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;

    int16_t accelOffsetX, accelOffsetY, accelOffsetZ;
    int16_t gyroOffsetX, gyroOffsetY, gyroOffsetZ;

    float pitch, roll, yaw;
    unsigned long lastReadTime;
    bool isCalibrated;
};

#endif
