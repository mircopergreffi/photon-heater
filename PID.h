
#ifndef PID_H
#define PID_H

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

class PID
{
	public:
		PID()
		{
			setOutputLimits(0.0, 1.0);
			setDirection(DIRECT);
			setProportionalOn(P_ON_M);
			mLastInput = 0;
			mOutputSum = 0;
		}

		float Compute(float input, float setpoint, float dt)
		{
			float error = setpoint - input;
			float dinput = input - mLastInput;
			
			mOutputSum += mKI * error * dt;
			if (mProportionalOn == P_ON_M)
				mOutputSum -= mKP * dinput;
			if (mOutputSum > mOutputMax)
				mOutputSum = mOutputMax;
			else if (mOutputSum < mOutputMin)
				mOutputSum = mOutputMin;
			
			float output;
			if (mProportionalOn == P_ON_E)
				output = mKP * error;
			else
				output = 0;
			
			output += mOutputSum - mKD * dinput / dt;
			
			if (output > mOutputMax)
				output = mOutputMax;
			else if (output < mOutputMin)
				output = mOutputMin;
			
			mLastInput = input;

			return output;
		}

		void setTunings(float kp, float ki, float kd)
		{
			mKP = kp;
			mKI = ki;
			mKD = kd;
		}
		void setOutputLimits(float min, float max)
		{
			mOutputMin = min;
			mOutputMax = max;
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
		float mOutputMin, mOutputMax;
		PID_direction mDirection;
		PID_proportional_on mProportionalOn;
};

#endif /* PID_H */