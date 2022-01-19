

#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <Arduino.h>
#include <String.h>
#include <ArduinoJson.h>

class AnalogSensor
{
    public:
        AnalogSensor()
        {

        }
        AnalogSensor(JsonObject json)
        {
            loadFromJson(json);
        }
        void loadFromJson(JsonObject json)
        {
            mPin = json["pin"].as<int>();
            mName = json["name"].as<String>();
        }
        virtual float readValue()
        {
            return ((float)analogRead(mPin)) / 4095.0;
        }
        int getPin()
        {
            return mPin;
        }
        String getName()
        {
            return mName;
        }

    private:
        int mPin;
        String mName;
};

#endif /* ANALOG_SENSOR_H */
