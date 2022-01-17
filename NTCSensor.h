

#ifndef NTC_SENSOR_H
#define NTC_SENSOR_H

#include <math.h>
#include "ResistorSensor.h"

class NTCSensor : public ResistorSensor
{
    public:
        NTCSensor(int pin, String * name, float pullup, float beta, float R0, float T0)
        : ResistorSensor(pin, name, pullup)
        {
            mBetaInverse = 1.0/beta;
            mR0 = R0;
            mT0Inverse = 1.0/T0;
        }
        float readValue()
        {
            float R = ResistorSensor::readValue();
            // Steinhart–Hart equation:
            return 1.0/(mT0Inverse + mBetaInverse*log(R/mR0));
        }
        static NTCSensor fromJson(JsonObject json)
        {
            return NTCSensor(
                json["pin"].toInt(),
                json["name"],
                json["pullup"],
                json["beta"],
                json["R0"],
                json["T0"],)
        }
    private:
        float mBetaInverse, mR0, mT0Inverse;
};

#endif /* NTC_SENSOR_H */
