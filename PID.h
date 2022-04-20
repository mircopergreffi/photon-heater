
#ifndef PID_H
#define PID_H

#include <ArduinoJson.h>
#include "Controller.h"

enum PID_direction
{
	DIRECT = 0,
	REVERSE = 1
};
enum PID_proportional_on
{
	P_ON_M = 0,
	P_ON_E = 1
};

class PID : public Controller
{
	public:
		PID()
		{
			setDirection(DIRECT);
			setProportionalOn(P_ON_E);
			mLastInput = 0;
			mOutputSum = 0;
		}

		// Loads values and parameters from json object
		void loadFromJson(JsonObject& json)
		{
			Controller::loadFromJson(json);
			
			float p = 0, i = 0, d = 0;
			if (json.containsKey("p"))
				p = json["p"].as<float>();
			if (json.containsKey("i"))
				i = json["i"].as<float>();
			if (json.containsKey("d"))
				d = json["d"].as<float>();
			setTunings(p, i, d);

			if (json.containsKey("direction"))
			{
				String dir = json["direction"].as<String>();
				if (dir.compareTo("direct") == 0)
					setDirection(DIRECT);
				else if (dir.compareTo("reverse") == 0)
					setDirection(REVERSE);
			}

			if (json.containsKey("proportional"))
			{
				String prop = json["proportional"].as<String>();
				if (prop.compareTo("error") == 0)
					setProportionalOn(P_ON_E);
				else if (prop.compareTo("measurement") == 0)
					setProportionalOn(P_ON_M);
			}
		}

		float compute(float input, float setpoint, float dt)
		{
			float error = setpoint - input;
			float dinput = input - mLastInput;
			
			mOutputSum += mKI * error * dt;
			if (mProportionalOn == P_ON_M)
				mOutputSum -= mKP * dinput;
			boundOutput(mOutputSum);
			
			float output;
			if (mProportionalOn == P_ON_E)
				output = mKP * error;
			else
				output = 0;
			
			output += mOutputSum - mKD * dinput / dt;
			boundOutput(output);
			
			mLastInput = input;

			return output;
		}

		void setTunings(float kp, float ki, float kd)
		{
			mKP = kp;
			mKI = ki;
			mKD = kd;
		}
		void setDirection(PID_direction direction)
		{
			mDirection = direction;
		}
		void setProportionalOn(PID_proportional_on proportional)
		{
			mProportionalOn = proportional;
		}
	private:
		float mOutputSum, mLastInput;
		float mKP, mKI, mKD;
		PID_direction mDirection;
		PID_proportional_on mProportionalOn;
};

#endif /* PID_H */