
#ifndef HEATER_H
#define HEATER_H

#define PWM_Heater_Ch   2
#define PWM_Heater_Freq 10
#define PWM_Heater_Rs   8
#define PWM_Heater_Max  255
#define PWM_Heater_Duty_Cycle_Max 0.95
#define PWM_Heater_Duty_Cycle_Min 0.05

#include "Arduino.h"
#include <ArduinoJson.h>
#include "PID.h"
#include "Status.h"

class Heater
{
	public:
		Heater()
		{

		}
		// Loads values and parameters from json object
		void loadFromJson(JsonObject json)
		{
			mPin = json["pin"];
			mTempMax = json["temp_max"];
			mTempMin = json["temp_min"];
			mTempMaxCritical = json["temp_max_critical"];
			mTempMinCritical = json["temp_min_critical"];
			mPowerMax = json["power_max"];
			mController.loadFromJson(json["control"]);
			setup();
		}
		// Sets power output
		void setPower(float power) const
		{
			// Clips the power to the given maximum power
			if (power > mPowerMax)
				power = mPowerMax;
			// Clips the power according to minimum and maximum duty cycles
			if (power < PWM_Heater_Duty_Cycle_Min)
				power = 0;
			else if (power > PWM_Heater_Duty_Cycle_Max)
				power = 1;
			// Write the digital output
			ledcWrite(PWM_Heater_Ch, power*PWM_Heater_Max);
		}
		// Runs the heater logic
		void run(Status &status)
		{
			// Shutdown heater in case it execeds the critical temperatures
			if (status.temperatureHeater >= mTempMaxCritical
				|| status.temperatureHeater <= mTempMinCritical)
				status.heater = false;
			// Calculate power output according to the heater status
			if (status.heater)
			{
				if (status.heaterSetpoint > mTempMax)
					status.heaterSetpoint = mTempMax;
				if (status.heaterSetpoint < mTempMin)
					status.heaterSetpoint = mTempMin;
				// Calculate the power output according to the controller scheme
				status.powerHeater = mController.compute(status.temperatureHeater, status.heaterSetpoint, status.dt);
			}
			else
			{
				// Power off
				status.powerHeater = 0;
			}
			// Set power output
			setPower(status.powerHeater);
		}
	private:
		// Setup the PWM channel
		void setup() const
		{
			// Attach pin to the PWM channel
			ledcAttachPin(mPin, PWM_Heater_Ch);
			// Setup the PWM channel with proper frequency and resolution
			ledcSetup(PWM_Heater_Ch, PWM_Heater_Freq, PWM_Heater_Rs);
			// Write digital output zero for initialization
			ledcWrite(PWM_Heater_Ch, 0);
		}

		int mPin;
		float mTempMax, mTempMin;
		float mTempMaxCritical, mTempMinCritical;
		float mPowerMax;
		PID mController;
};

#endif /* HEATER_H */
