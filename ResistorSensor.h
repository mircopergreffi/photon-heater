

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
			mRefVoltage = 3.3;
		}
		void loadFromJson(JsonObject json)
		{
			mPullUp = json["pullup"];
			if (json.containsKey("ref_voltage"))
				mRefVoltage = json["ref_voltage"].as<float>();
			AnalogSensor::loadFromJson(json);
		}
		float readValue(float dt)
		{
			float v = AnalogSensor::readValue(dt) / mRefVoltage;
			return (v/(1.0-v)) * mPullUp;
		}
	private:
		float mRefVoltage;
		float mPullUp;
};

#endif /* RESISTOR_SENSOR_H */
