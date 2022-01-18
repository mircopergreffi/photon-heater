

#ifndef RESISTOR_SENSOR_H
#define RESISTOR_SENSOR_H

#include "AnalogSensor.h"

class ResistorSensor : public AnalogSensor
{
    public:
        ResistorSensor(JsonObject json)
        : AnalogSensor(json)
        {
            mPullUp = json["pullup"];
        }
        float readValue()
        {
            float v = AnalogSensor::readValue();
            return (v/(1.0-v)) * mPullUp;
        }
    private:
        float mPullUp;
};

#endif /* RESISTOR_SENSOR_H */
