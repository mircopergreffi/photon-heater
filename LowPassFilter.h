
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
            mLastTimestamp = 0;
            setFrequency(0);
        }
        void loadFromJson(JsonObject json)
        {
            setFrequency(json["frequency"].as<float>());
        }
        float filter(float value)
        {
            unsigned long timestamp = millis();
            float dt = (timestamp - mLastTimestamp) / 1000.0;
            if (dt > 0.5)
                return value;
            mValue += (value - mValue) * mMultiplier * dt;
            mLastTimestamp = timestamp;
            return mValue;
        }
        void setFrequency(float frequency)
        {
            mFrequency = frequency;
            mMultiplier = TWOPI * mFrequency;
        }
    private:
        float mFrequency, mMultiplier;
        unsigned long mLastTimestamp;
        float mValue;
};

#endif /* LOW_PASS_FILTER_H */