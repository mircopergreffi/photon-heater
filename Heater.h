
#ifndef HEATER_H
#define HEATER_H

#define PWM_Heater_Ch   2
#define PWM_Heater_Freq 100
#define PWM_Heater_Rs   8
#define PWM_Heater_Max  255
#define PWM_Heater_Duty_Cycle_Max 0.95
#define PWM_Heater_Duty_Cycle_Min 0.05

#include "Arduino.h"
#include <ArduinoJson.h>

class Heater
{
	public:
		Heater()
		{

		}
		Heater(JsonObject json)
		{
			loadFromJson(json);
		}
		void loadFromJson(JsonObject json)
		{
			mPin = json["heater_pin"];
			mTempMax = json["heater_temp_max"];
			mTempMin = json["heater_temp_min"];
			mTempCritical = json["heater_temp_critical"];
			mPowerMax = json["heater_power_max"];
			setup();
		}
		void setPower(float power)
		{
			if (power > mPowerMax)
				power = mPowerMax;
			if (power < PWM_Heater_Duty_Cycle_Min)
				power = 0;
			else if (power > PWM_Heater_Duty_Cycle_Max)
				power = 1;
			ledcWrite(PWM_Heater_Ch, power*PWM_Heater_Max);
		}
	private:
		void setup()
		{
			ledcAttachPin(mPin, PWM_Heater_Ch);
			ledcSetup(PWM_Heater_Ch, PWM_Heater_Freq, PWM_Heater_Rs);
			ledcWrite(PWM_Heater_Ch, 0);
		}
		int mPin;
		float mTempMax, mTempMin, mTempCritical;
		float mPowerMax;
};

#endif /* HEATER_H */
