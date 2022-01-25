
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

		}
		Fan(JsonObject json)
		{
			loadFromJson(json);
		}
		void loadFromJson(JsonObject json)
		{
			mPin = json["pin"];
			mMaxSpeed = json["speed_max"];
			mMinSpeed = json["speed_min"];
		}
		void setSpeed(float speed)
		{
			speed = purgeSpeed(speed);
			if (speed != mSpeed)
			{
				mSpeed = speed;
				ledcWrite(PWM_Fan_Ch, mSpeed*PWM_Fan_Max);
			}
		}
		float getSpeed()
		{
			return mSpeed;
		}
	private:
		void setup()
		{
			ledcAttachPin(mPin, PWM_Fan_Ch);
			ledcSetup(PWM_Fan_Ch, PWM_Fan_Freq, PWM_Fan_Rs);
			ledcWrite(PWM_Fan_Ch, 0);
		}
		float purgeSpeed(float speed)
		{
			if (speed > mMaxSpeed)
				speed = mMaxSpeed;
			if (speed < mMinSpeed && speed > 0)
				speed = mMinSpeed;
			if (speed < PWM_Fan_Duty_Cycle_Min)
				speed = 0;
			else if (speed > PWM_Fan_Duty_Cycle_Max)
				speed = 1;
			return speed;
		}
		int mPin;
		float mSpeed = 0, mMaxSpeed, mMinSpeed;
};

#endif /* FAN_H */