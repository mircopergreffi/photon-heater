

#ifndef NTC_SENSOR_H
#define NTC_SENSOR_H

#include <math.h>
#include <ArduinoJson.h>
#include "ResistorSensor.h"

class NTCSensor : public ResistorSensor
{
	public:
		NTCSensor()
		: ResistorSensor()
		{
			
		}
		NTCSensor(JsonObject json)
		: ResistorSensor(json)
		{
			loadFromJson(json);
		}
		void loadFromJson(JsonObject json)
		{
			mBetaInverse = 1.0/json["beta"].as<float>();
			mR0 = json["R0"];
			mT0Inverse = 1.0/(json["T0"].as<float>()+273.15);
			ResistorSensor::loadFromJson(json);
		}
		float readValue()
		{
			float R = ResistorSensor::readValue();
			// Steinhart–Hart equation:
			return 1.0/(mT0Inverse + mBetaInverse*log(R/mR0))-273.15;
		}
	private:
		float mBetaInverse, mR0, mT0Inverse;
};

#endif /* NTC_SENSOR_H */
