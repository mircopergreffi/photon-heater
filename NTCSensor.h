

#ifndef NTC_SENSOR_H
#define NTC_SENSOR_H

#include <math.h>
#include "ResistorSensor.h"

class NTCSensor : public ResistorSensor
{
	public:
		NTCSensor()
		: ResistorSensor()
		{
			
		}
		void loadFromJson(JsonObject const &json) throw(InvalidJsonException)
		{
			mBetaInverse = 1.0/json["B"].as<float>();
			mR0 = json["R0"];
			mT0Inverse = 1.0/(json["T0"].as<float>()+273.15);
			ResistorSensor::loadFromJson(json);
		}
		float readValue(float dt)
		{
			float R = ResistorSensor::readValue(dt);
			// Steinhart–Hart equation:
			return 1.0/(mT0Inverse + mBetaInverse*log(R/mR0)) - 273.15;
		}
	private:
		float mBetaInverse, mR0, mT0Inverse;
};

#endif /* NTC_SENSOR_H */
