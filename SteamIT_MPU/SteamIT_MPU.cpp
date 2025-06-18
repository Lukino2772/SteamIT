#include "SteamIT_MPU.h"

SteamIT_MPU::SteamIT_MPU() : rawAccelX(0), rawAccelY(0), rawAccelZ(0), rawGyroX(0), rawGyroY(0), rawGyroZ(0), accelX(0), accelY(0), accelZ(0),
      gyroX(0), gyroY(0), gyroZ(0), accelOffsetX(0), accelOffsetY(0), accelOffsetZ(0), gyroOffsetX(0), gyroOffsetY(0), gyroOffsetZ(0),
      pitch(0), roll(0), yaw(0), lastReadTime(0), isCalibrated(false) {}

SteamIT_MPU::SteamIT_MPU(){}

void SteamIT_MPU::begin() {
      Wire.begin();
      Wire.setClock(400000);

      Wire.beginTransmission(address);
      Wire.write(0x6B);
      Wire.write(0);
      Wire.endTransmission(true);

      EEPROM.get(tart_address, accelOffsetX);
      EEPROM.get(tart_address + sizeof(int16_t), accelOffsetY);
      EEPROM.get(tart_address + 2 * sizeof(int16_t), accelOffsetZ);
      EEPROM.get(tart_address + 3 * sizeof(int16_t), gyroOffsetX);
      EEPROM.get(tart_address + 4 * sizeof(int16_t), gyroOffsetY);
      EEPROM.get(tart_address + 5 * sizeof(int16_t), gyroOffsetZ);

      if (abs(accelOffsetX) < 1000 && abs(accelOffsetY) < 1000 && abs(accelOffsetZ) < 1000 &&
          abs(gyroOffsetX) < 1000 && abs(gyroOffsetY) < 1000 && abs(gyroOffsetZ) < 1000) {
            isCalibrated = true;
          } else {
                calibrate();
          }
      lastReadTime = millis();
}
void SteamIT_MPU::calibrate() {
    const int samples = 250;
    long sumAccelX = 0, sumAccelY = 0, sumAccelZ = 0;
    long sumGyroX = 0, sumGyroY = 0, sumGyroZ = 0;

    for (int i = 0; i < samples; i++) {
        Wire.beginTransmission(address);
        Wire.write(0x3B);
        if (Wire.endTransmission(false) != 0 || Wire.requestFrom(address, 14, true) != 14) {
            recoverSensor();
            i--;
            delay(2);
            continue;
        }

        int16_t ax = (Wire.read() << 8) | Wire.read();
        int16_t ay = (Wire.read() << 8) | Wire.read();
        int16_t az = (Wire.read() << 8) | Wire.read();
        Wire.read(); Wire.read();
        int16_t gx = (Wire.read() << 8) | Wire.read();
        int16_t gy = (Wire.read() << 8) | Wire.read();
        int16_t gz = (Wire.read() << 8) | Wire.read();

        sumAccelX += ax;
        sumAccelY += ay;
        sumAccelZ += az;
        sumGyroX += gx;
        sumGyroY += gy;
        sumGyroZ += gz;

        delay(2);
    }

    accelOffsetX = sumAccelX / samples;
    accelOffsetY = sumAccelY / samples;
    accelOffsetZ = (sumAccelZ / samples) - 16384;

    gyroOffsetX = sumGyroX / samples;
    gyroOffsetY = sumGyroY / samples;
    gyroOffsetZ = sumGyroZ / samples;

    EEPROM.put(tart_address, accelOffsetX);
    EEPROM.put(tart_address + sizeof(int16_t), accelOffsetY);
    EEPROM.put(tart_address + 2 * sizeof(int16_t), accelOffsetZ);
    EEPROM.put(tart_address + 3 * sizeof(int16_t), gyroOffsetX);
    EEPROM.put(tart_address + 4 * sizeof(int16_t), gyroOffsetY);
    EEPROM.put(tart_address + 5 * sizeof(int16_t), gyroOffsetZ);

#if defined(ESP32) || defined(ESP8266)
    EEPROM.commit();
#endif

    isCalibrated = true;
}

void SteamIT_MPU::recoverSensor() {
    Wire.end();
    delay(10);
    Wire.begin();
    begin();
}

void SteamIT_MPU::readSensorData() {
    if (!isCalibrated) calibrate();

    Wire.beginTransmission(address);
    Wire.write(0x3B);
    if (Wire.endTransmission(false) != 0 || Wire.requestFrom(address, 14, true) != 14) {
        recoverSensor();
        return;
    }

    rawAccelX = (Wire.read() << 8) | Wire.read();
    rawAccelY = (Wire.read() << 8) | Wire.read();
    rawAccelZ = (Wire.read() << 8) | Wire.read();
    Wire.read(); Wire.read();
    rawGyroX = (Wire.read() << 8) | Wire.read();
    rawGyroY = (Wire.read() << 8) | Wire.read();
    rawGyroZ = (Wire.read() << 8) | Wire.read();

    applyCalibration();

    unsigned long now = millis();
    float dt = (lastReadTime == 0) ? 0.01f : (now - lastReadTime) / 1000.0f;
    lastReadTime = now;

    float gyroXrate = gyroX;
    float gyroYrate = gyroY;
    float gyroZrate = gyroZ;

    pitch += gyroXrate * dt * DEG_TO_RAD;
    roll  += gyroYrate * dt * DEG_TO_RAD;
    yaw   += gyroZrate * dt * DEG_TO_RAD;

    computeAngles();
}

void SteamIT_MPU::applyCalibration() {
    int16_t calibratedAccelX = rawAccelX - accelOffsetX;
    int16_t calibratedAccelY = rawAccelY - accelOffsetY;
    int16_t calibratedAccelZ = rawAccelZ - accelOffsetZ;

    int16_t calibratedGyroX = rawGyroX - gyroOffsetX;
    int16_t calibratedGyroY = rawGyroY - gyroOffsetY;
    int16_t calibratedGyroZ = rawGyroZ - gyroOffsetZ;

    accelX = (float)calibratedAccelX / 16384.0f;
    accelY = (float)calibratedAccelY / 16384.0f;
    accelZ = (float)calibratedAccelZ / 16384.0f;

    gyroX = (float)calibratedGyroX / 131.0f;
    gyroY = (float)calibratedGyroY / 131.0f;
    gyroZ = (float)calibratedGyroZ / 131.0f;
}

void SteamIT_MPU::computeAngles() {
    float pitchAcc = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ));
    float rollAcc  = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ));

    pitch = alpha * pitch + (1.0f - alpha) * pitchAcc;
    roll  = alpha * roll + (1.0f - alpha) * rollAcc;
}

float SteamIT_MPU::getPitch() const {
    return pitch * RAD_TO_DEG;
}

float SteamIT_MPU::getRoll() const {
    return roll * RAD_TO_DEG;
}

float SteamIT_MPU::getYaw() const {
    return yaw * RAD_TO_DEG;
}
