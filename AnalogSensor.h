

#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <Arduino.h>
#include <String.h>
#include <ArduinoJson.h>
#include <driver/adc.h>

#include "Filter.h"
#include "LowPassFilter.h"

class AnalogSensor
{
	public:
		AnalogSensor()
		{
			mFilter = nullptr;
		}
		void loadFromJson(JsonObject json)
		{
			mPin = json["pin"].as<int>();
			mName = json["name"].as<String>();
			if (mFilter != nullptr)
			{
				delete mFilter;
				mFilter = nullptr;
			}
			if (json.containsKey("filter"))
			{
				if (json["filter"]["type"].as<String>().compareTo("lowpass") == 0)
					mFilter = new LowPassFilter();
				if (mFilter != nullptr)
					mFilter->loadFromJson(json["filter"]);
			}
		}
		virtual float readValue(float dt) const
		{
			float value = ((float)analogRead(mPin)) / 4095.0; //+ 0.17;
			if (mFilter != nullptr)
				value = mFilter->filter(value, dt);
			return value;
		}

	private:
		int mPin;
		String mName;
		Filter * mFilter;
};

#endif /* ANALOG_SENSOR_H */
