

#IFNDEF NTC_SENSOR_H
#DEFINE NTC_SENSOR_H

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
    private:
        float mBeta, mR0, mT0Inverse;
};

#ENDIF /* NTC_SENSOR_H */