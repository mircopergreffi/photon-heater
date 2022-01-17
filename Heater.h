
#ifndef HEATER_H
#define HEATER_H

#include <ArduinoJson.h>
#include "NTCSensor.h"

class Heater
{
    public:
        Heater(JsonObject json)
        {
            mPin = json["heater_pin"];
            mTempMax = json["heater_temp_max"];
            mTempMin = json["heater_temp_min"];
            mTempCritical = json["heater_temp_critical"];
            mPowerMax = json["heater_power_max"];
            mDutyCycleMax = json["heater_duty_cycle_max"];
            mDutyCycleMin = json["heater_duty_cycle_min"];
            mSensor = NTCSensor::fromJson(json["heater_sensor"]);
        }
    private:
        int mPin;
        float mTempMax, mTempMin, mTempCritical;
        float mPowerMax;
        float mDutyCycleMax, mDutyCycleMin;
        NTCSensor mSensor;
};

#endif /* HEATER_H */