
#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#define TWOPI 6.28318530718

#include "Filter.h"

class LowPassFilter : public Filter
{
	public:
		LowPassFilter()
		{
			mValue = 0;
			setFrequency(0);
		}
		// Loads values and parameters from json object
		void loadFromJson(JsonObject json)
		{
			setFrequency(json["frequency"].as<float>());
		}
		float filter(float value, float dt)
		{
			// If the time difference is too large
			// do not apply the filter
			if (dt > 0.5)
				mValue = value;
			else
				mValue += (value - mValue) * mMultiplier * dt;
			return mValue;
		}
		// Set the frequency
		void setFrequency(float frequency)
		{
			mFrequency = frequency;
			mMultiplier = TWOPI * mFrequency;
		}
	private:
		float mFrequency, mMultiplier;
		float mValue;
};

#endif /* LOW_PASS_FILTER_H */