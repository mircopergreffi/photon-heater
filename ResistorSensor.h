

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
		void loadFromJson(JsonObject json)
		{
			mPullUp = json["pullup"];
			AnalogSensor::loadFromJson(json);
		}
		float readValue(float dt)
		{
			float v = AnalogSensor::readValue(dt);
			return (v/(1.0-v)) * mPullUp;
		}
	private:
		float mPullUp;
};

#endif /* RESISTOR_SENSOR_H */
