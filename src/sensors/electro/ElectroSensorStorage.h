#ifndef ELECTRO_SENSOR_STORAGE_H
#define ELECTRO_SENSOR_STORAGE_H

#include "LittleFS.h"
#include "sensors/electro/EmonLibSensor.h"

class ElectroSensorStorage {
   private:
    bool isInited = false;
    EmonLibSensor* emon;
    MeteoLog* log;

    float powerSpent = -1;
    float lastPower = 0;
    long lastTs = 0;

    float getFromFs() {
        if (LittleFS.exists("/electro.txt")) {
            File file = LittleFS.open("/electro.txt", "r");
            float value = file.parseFloat();
            file.close();
            return value;
        } else {
            Serial.println("electro.txt not exists");
            return -20000;
        }
    }

    float saveToFs(float value) {
        File dataFile = LittleFS.open("/electro.txt", "w");
        if (!dataFile) {
            Serial.println("Failed to open config file electro.txt for writing");
            return -10;
        }
        dataFile.println(value);
        dataFile.flush();
        dataFile.close();

        return value;
    }

   public:
    ElectroSensorStorage() {}

    void init(EmonLibSensor* _emon, MeteoLog* _log) {
        if (isInited) {
            return;
        }
        emon = _emon;
        log = _log;
        powerSpent = getFromFs();
        isInited = true;
    }

    float getPowerSpent() { return powerSpent; }

    void setPowerSpent(float value) {
        powerSpent = value;
        saveToFs(powerSpent);
    }

    void processInterval() {
        long tsNow = millis();
        float curPower = emon->power();

        if (tsNow - lastTs < 6000) {
            float avgWatt = (curPower + lastPower) / 2;
            float wattSpent = avgWatt / 60 / 60 * ((tsNow - lastTs) / 1000.0);

            powerSpent += wattSpent;
            saveToFs(powerSpent);

            log->add("IRMSsum avgWatt: " + String(avgWatt));
            log->add("IRMSsum wattSpent: " + String(wattSpent));
        }

        lastTs = tsNow;
        lastPower = curPower;
    }
};

#endif