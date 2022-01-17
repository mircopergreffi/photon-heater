

#IFNDEF RESISTOR_SENSOR_H
#DEFINE RESISTOR_SENSOR_H

#include "AnalogSensor.h"

class ResistorSensor : public AnalogSensor
{
    public:
        ResistorSensor(int pin, String * name, float pullup)
        : AnalogSensor(pin, name)
        {
            mPullUp = pullup;
        }
        float readValue()
        {
            float v = AnalogSensor::readValue();
            return (v/(1.0-v)) * mPullUp;
        }
    private:
        float mPullUp;
};

#ENDIF /* RESISTOR_SENSOR_H */