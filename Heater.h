
#IFNDEF HEATER_H
#DEFINE HEATER_H

#include "TemperatureSensor.h"

class Heater
{
    public:

    private:
        int mPin;
        float mTempMax, mTempMin, mTempCritical;
        float mPowerMax;
        float mDutyCycleMax, mDutyCycleMin;
        TemperatureSensor * mSensor;
};

#ENDIF /* HEATER_H */