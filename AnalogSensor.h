

#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <String.h>
#include <ArduinoJson.h>

class AnalogSensor
{
    public:
        AnalogSensor(JsonObject json)
        {
            mPin = json["pin"];
            mName = json["name"];
        }
        virtual float readValue()
        {
            return ((float)analogRead(mPin)) / 4095.0;
        }
        int getPin()
        {
            return mPin;
        }
        String* getName()
        {
            return mName;
        }

    private:
        int mPin;
        String * mName;
};

#endif /* ANALOG_SENSOR_H */
