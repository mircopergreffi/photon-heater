

#ifndef NTC_SENSOR_H
#define NTC_SENSOR_H

#include <math.h>
#include <ArduinoJson.h>
#include "ResistorSensor.h"

class NTCSensor : public ResistorSensor
{
    public:
        NTCSensor(JsonObject json)
        : ResistorSensor(json)
        {
            mBetaInverse = 1.0/json["beta"].as<float>();
            mR0 = json["R0"];
            mT0Inverse = 1.0/json["T0"].as<float>();
        }
        float readValue()
        {
            float R = ResistorSensor::readValue();
            // Steinhart–Hart equation:
            return 1.0/(mT0Inverse + mBetaInverse*log(R/mR0));
        }
    private:
        float mBetaInverse, mR0, mT0Inverse;
};

#endif /* NTC_SENSOR_H */
