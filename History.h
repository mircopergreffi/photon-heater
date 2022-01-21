
#ifndef HISTORY_H
#define HISTORY_H

#include <String.h>
#include <ArduinoJson.h>

template<unsigned int size, unsigned int entries>
class History
{
	public:
		History(const char * names[entries])
		{
			for (int i=0; i<entries; i++)
				mNames[i] = names[i];
		}

		void push(float values[entries])
		{
			if (wait)
				Serial.println("waiting");
			while(wait);
			mTimestamps[mEnd] = millis();
			for (int i=0; i<entries; i++)
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

		StaticJsonDocument<12288> getJson(unsigned long fromTimestamp)
		{
			wait = true;
			StaticJsonDocument<12288> doc;
			unsigned int i = mStart;
			size_t j = 0;
			while (i != mEnd)
			{
				if (i >= size)
					i = 0;

				if (mTimestamps[i] > fromTimestamp)
				{
					doc["timestamps"][j] = mTimestamps[i];
					for (int k=0; k<entries; k++)
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
		volatile bool wait = false;
		unsigned int mStart = 0, mEnd = 0;
		const char * mNames[entries];
		unsigned long mTimestamps[size];
		float mValues[size][entries];
};

#endif /* HISTORY_H */
