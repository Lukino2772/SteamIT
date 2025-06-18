#include "SteamIT_MPU.h"
#include "SteamIT_Sensors.h"
#include "SteamIT_Emotions.h"

SteamIT_MPU mpu;
SteamIT_Sensors sensors;
SteamIT_Emotions emotions;

const int sensorPins[] = {15, 2, 0, 4, 16, 17, 5, 18};

void setup() {
    Serial.begin(115200);
    EEPROM.begin(64);
    sensors.begin(sensorPins, 8);
    mpu.begin();
    Serial.println("MPU6050 Initialized and Calibrated.");
}

void loop() {
    string em = "praise";
    emotions.updateEmotions(em);
    mpu.readSensorData();

    Serial.print("Pitch: ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(" | Roll: ");
    Serial.print(mpu.getRoll(), 2);
    Serial.print(" | Yaw: ");
    Serial.println(mpu.getYaw(), 2);

    sensors.state_update();

    if (sensors.all_touched()) {
        Serial.println("🐢 All sensors touched. Turtle is happy!");
        sensors.reset();
    }
    if (sensors.left_touched()) {
        Serial.println("🐢 Side1 sensors touched!");
        sensors.reset();
    }
    if (sensors.right_touched()) {
        Serial.println("🐢 Side2 sensors touched!");
        sensors.reset();
    }
    if (sensors.middle_touched()) {
        Serial.println("🐢 Middle sensors touched!");
        sensors.reset();
    }
    if (sensors.timed_out()) {
        Serial.println("Stroke timed out. Start again 🐢");
        sensors.reset();
    }
    delay(1);
}
