
#ifndef TEMPERATURE_HISTORY_H
#define TEMPERATURE_HISTORY_H

#include <String.h>
#include <ArduinoJson.h>

template<unsigned int size, unsigned int sensors>
class TemperatureHistory
{
	public:
		TemperatureHistory(const char * names[sensors])
		{
			for (int i=0; i<sensors; i++)
				mNames[i] = names[i];
		}

		void push(float values[sensors])
		{
			while(wait);
			mTimestamps[mEnd] = millis();
			for (int i=0; i<sensors; i++)
			{
				mValues[mEnd][i] = values[i];
			}
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
			wait = true;
			StaticJsonDocument<1024> doc;
			unsigned int i = mStart;
			size_t j = 0;
			while (i != mEnd)
			{
				if (i >= size)
					i = 0;

				if (mTimestamps[i] > fromTimestamp)
				{
					doc["timestamps"][j] = mTimestamps[i];
					for (int k=0; k<sensors; k++)
					{
						doc[mNames[k]][j] = mValues[i][k];
					}
					j++;
				}

				i++;
			}
			wait = false;
			return doc;
		}
		
	private:
		bool wait = false;
		unsigned int mStart = 0, mEnd = 0;
		const char * mNames[sensors];
		unsigned long mTimestamps[size];
		float mValues[size][sensors];
};

#endif /* TEMPERATURE_HISTORY_H */
