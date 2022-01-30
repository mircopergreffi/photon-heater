
#ifndef AIR_H
#define AIR_H

#include <ArduinoJson.h>
#include "PID.h"
#include "Status.h"

class Air
{
	public:
		Air()
		{

		}
		// Loads values and parameters from json object
		void loadFromJson(JsonObject json)
		{
			mTempMax = json["temp_max"];
			mTempMin = json["temp_min"];
			mTempMaxCritical = json["temp_max_critical"];
			mTempMinCritical = json["temp_min_critical"];
			exceededMaxThreshold = json["critical_threshold"];
			mController.loadFromJson(json["control"]);
		}
		void run(Status &status)
		{
			// Shutdown heater in case it execeds the critical temperatures
			if (status.temperatureAir >= mTempMaxCritical
				|| status.temperatureAir <= mTempMinCritical)
					exceededMaxCounter++;
			else
				exceededMaxCounter = 0;

			if(exceededMaxCounter >= exceededMaxThreshold)
				status.heater = false;
			// If the heater is on calculate the heater temperature setpoint
			if (status.heater)
			{
				if (status.airSetpoint > mTempMax)
					status.airSetpoint = mTempMax;
				if (status.airSetpoint < mTempMin)
					status.airSetpoint = mTempMin;
				status.heaterSetpoint = mController.compute(status.temperatureAir, status.airSetpoint, status.dt);
			}
		}
	private:
		float mTempMax, mTempMin;
		float mTempMaxCritical, mTempMinCritical;
		int exceededMaxThreshold, exceededMaxCounter = 0;
		PID mController;
};

#endif /* AIR_H */