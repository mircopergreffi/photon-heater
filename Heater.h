
#ifndef HEATER_H
#define HEATER_H

#include <ArduinoJson.h>

class Heater
{
	public:
		Heater(JsonObject json)
		{
			mPin = json["heater_pin"];
			mTempMax = json["heater_temp_max"];
			mTempMin = json["heater_temp_min"];
			mTempCritical = json["heater_temp_critical"];
			mPowerMax = json["heater_power_max"];
			mDutyCycleMax = json["heater_duty_cycle_max"];
			mDutyCycleMin = json["heater_duty_cycle_min"];
		}
		void setPower(float power)
		{
			if (power >= mPowerMax)
				power = mPowerMax;
			if (power < mDutyCycleMin)
				power = 0;
			else if (power > mDutyCycleMax)
				power = 1;
			analogWrite(mPin, power*255);
		}
	private:
		int mPin;
		float mTempMax, mTempMin, mTempCritical;
		float mPowerMax;
		float mDutyCycleMax, mDutyCycleMin;
};

#endif /* HEATER_H */
