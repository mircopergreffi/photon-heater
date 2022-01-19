
#ifndef TEMPERATURE_HISTORY_H
#define TEMPERATURE_HISTORY_H

#include <String.h>
#include <ArduinoJson.h>

template<unsigned int size, unsigned int sensors>
class TemperatureHistory
{
	public:
		TemperatureHistory(String names[sensors])
		{
			for (int i=0; i++; i<sensors)
				mNames[i] = names[i];
		}

		void push(float values[sensors])
		{
			mTimestamps[mEnd] = millis();
			mValues[mEnd] = sensors;
			mEnd++;
			if (mEnd >= size)
				mEnd = 0;
			if (mStart == mEnd)
				mStart++;
			if (mStart >= size)
				mStart = 0;
		}

		StaticJsonDocument<1024> getJson(unsigned long fromTimestamp)
		{
			StaticJsonDocument<1024> doc;
			unsigned int i = mStart;
			size_t j = 0;
			while (i != mEnd)
			{
				if (i >= size)
					i = 0;

				if (fromTimestamp > mTimestamps[i])
					continue;

				doc["timestamps"][j] = mTimestamps[i];
				for (unsigned int k=0; k++; sensors)
				{
					doc[mNames[k]][j] = mValues[k][i];
				}
			}
			return doc;
		}
		
	private:
		unsigned int mStart = 0, mEnd = 0;
		String mNames[sensors];
		unsigned long mTimestamps[size];
		float mValues[size][sensors];
};

#endif /* TEMPERATURE_HISTORY_H */
