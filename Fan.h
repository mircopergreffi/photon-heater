
#ifndef FAN_H
#define FAN_H

#define PWM_Fan_Ch   1
#define PWM_Fan_Freq 100
#define PWM_Fan_Rs   8
#define PWM_Fan_Max  255
#define PWM_Fan_Duty_Cycle_Max 0.95
#define PWM_Fan_Duty_Cycle_Min 0.05

#include "JsonLoadable.h"
#include "Status.h"

class Fan : public JsonLoadable
{
	public:
		Fan()
		{
			mRequestedSpeed = 0;
			mActualSpeed = 0;
			mStarted = false;
			mBoostTime = 1000;
			mInverted = false;
		}

		// Loads values and parameters from json object
		void loadFromJson(JsonObject const &json) throw (InvalidJsonException)
		{
			mPin = json["pin"];
			mMaxSpeed = json["speed_max"];
			mMinSpeed = json["speed_min"];
			if (json.containsKey("boost_time"))
				mBoostTime = json["boost_time"].as<float>() * 1000;
			if (json.containsKey("inverted"))
				mInverted = json["inverted"].as<bool>();
			setupPWM();
		}

		float setSpeed(float speed)
		{
			mRequestedSpeed = speed;
		}

		float getSpeed() const
		{
			return mActualSpeed;
		}

		// Runs the fan logic
		void run(float dt)
		{
			// Set the fan speed
			if (mRequestedSpeed > 0 && !mStarted)
			{
				applySpeed(1);
				mStarted = true;
				mDelayedTime = 0;
			}
			else
			{
				mDelayedTime += dt;
				if (mDelayedTime > mBoostTime)
					applySpeed(mRequestedSpeed);
			}

			if (mStarted && mActualSpeed == 0)
			{
				mStarted = false;
			}
		}
	
	protected:
		// Writes the digital output for controlling the fan speed
		void applySpeed(float speed)
		{
			// Clip the fan speed
			if (speed > mMaxSpeed)
				speed = mMaxSpeed;
			if (speed < mMinSpeed && speed > 0)
				speed = mMinSpeed;
			
			if (mActualSpeed != speed)
			{
				mActualSpeed = speed;
				// Calculate the duty cycle
				float duty_cycle;
				if (mInverted)
					duty_cycle = 1 - speed;
				else
					duty_cycle = speed;
				// Clip the duty cycle
				if (duty_cycle < PWM_Fan_Duty_Cycle_Min)
					duty_cycle = 0;
				else if (duty_cycle > PWM_Fan_Duty_Cycle_Max)
					duty_cycle = 1;

				// If the new speed is different from the current speed
				if (duty_cycle != mDutyCycle)
				{
					// Memorize the fan speed
					mDutyCycle = duty_cycle;
					// Write the digital output
					ledcWrite(PWM_Fan_Ch, mActualSpeed*PWM_Fan_Max);
				}
			}
		}

	private:
		void setupPWM()
		{
			ledcAttachPin(mPin, PWM_Fan_Ch);
			ledcSetup(PWM_Fan_Ch, PWM_Fan_Freq, PWM_Fan_Rs);
			ledcWrite(PWM_Fan_Ch, 0);
		}
		
		int mPin;
		float mMaxSpeed;
		float mMinSpeed;
		unsigned long mBoostTime;
		bool mInverted;

		float mRequestedSpeed;
		float mActualSpeed;
		float mDutyCycle;
		bool mStarted;
		float mDelayedTime;
};

#endif /* FAN_H */