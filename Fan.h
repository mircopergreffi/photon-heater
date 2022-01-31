
#ifndef FAN_H
#define FAN_H

#define PWM_Fan_Ch   1
#define PWM_Fan_Freq 100
#define PWM_Fan_Rs   8
#define PWM_Fan_Max  255
#define PWM_Fan_Duty_Cycle_Max 0.95
#define PWM_Fan_Duty_Cycle_Min 0.05

#include <Arduino.h>
#include <ArduinoJson.h>

class Fan
{
	public:
		Fan()
		{
			mSpeed = 0;
			mStarted = false;
			mStartedTimestamp = 0;
			mBoostTime = 1000;
		}
		// Loads values and parameters from json object
		void loadFromJson(JsonObject json)
		{
			mPin = json["pin"];
			mMaxSpeed = json["speed_max"];
			mMinSpeed = json["speed_min"];
			if (json.containsKey("boost_time"))
				mBoostTime = json["boost_time"].as<float>() * 1000;
			setup();
		}
		// Writes the digital output for controlling the fan speed
		void setSpeed(float speed)
		{
			// Clip the fan speed between its maximum and minimum
			// values allowed.
			if (speed > mMaxSpeed)
				speed = mMaxSpeed;
			if (speed < mMinSpeed && speed > 0)
				speed = mMinSpeed;
			// The output for the fan is inverted: the fan turns
			// off when the output is high; the fan turns on when
			// the output is low.
			speed = 1 - speed;
			// Clips the output according to the minimum and maximum
			// duty cycles
			if (speed < PWM_Fan_Duty_Cycle_Min)
				speed = 0;
			else if (speed > PWM_Fan_Duty_Cycle_Max)
				speed = 1;
			// If the new speed is different from the current speed
			if (speed != mSpeed)
			{
				// Memorize the fan speed
				mSpeed = speed;
				// Write the digital output
				ledcWrite(PWM_Fan_Ch, mSpeed*PWM_Fan_Max);
			}
		}
		// Runs the fan logic
		void run(Status &status)
		{
			// Calculate the fan speed given its current mode
			switch (status.fanMode)
			{
				case FAN_AUTO:
					// In automatic mode the fan turns on at full speed when the
					// temperature difference between the heater core and air sensors
					// is greater than 5 degrees.
					if (status.temperatureHeater - status.temperatureAir >= 5)
						status.fanSpeed = 1.0;
					else
						status.fanSpeed = 0.0;
					break;
				case FAN_MANUAL:
					// In manual mode the fan turns at the given speed.
					status.fanSpeed = status.fanManualSpeed;
			}

			// Set the fan speed
			if (status.fanSpeed > 0 && !mStarted)
			{
				setSpeed(1);
				mStarted = true;
				mStartedTimestamp = status.currentTimestamp;
			}
			else
			{
				if (status.currentTimestamp - mStartedTimestamp > mBoostTime)
					setSpeed(status.fanSpeed);
			}

			if (mStarted && status.fanSpeed == 0)
			{
				mStarted = false;
			}
		}
	private:
		void setup()
		{
			ledcAttachPin(mPin, PWM_Fan_Ch);
			ledcSetup(PWM_Fan_Ch, PWM_Fan_Freq, PWM_Fan_Rs);
			ledcWrite(PWM_Fan_Ch, 0);
		}
		int mPin;
		float mSpeed, mMaxSpeed, mMinSpeed;
		bool mStarted;
		unsigned long mBoostTime;
		unsigned long mStartedTimestamp;
};

#endif /* FAN_H */