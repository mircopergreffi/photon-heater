

#ifndef RESISTOR_SENSOR_H
#define RESISTOR_SENSOR_H

#include <Arduino.h>
#include "AnalogSensor.h"

class ResistorSensor : public AnalogSensor
{
	public:
		ResistorSensor()
		: AnalogSensor()
		{

		}
		ResistorSensor(JsonObject json)
		: AnalogSensor(json)
		{
			loadFromJson(json);
			AnalogSensor::loadFromJson(json);
		}
		void loadFromJson(JsonObject json)
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
