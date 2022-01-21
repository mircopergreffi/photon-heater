
#ifndef HISTORY_H
#define HISTORY_H

#include <String.h>
#include <ArduinoJson.h>

volatile bool lock0 = false, lock1 = false;

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
			while(lock1);
			lock0 = true;
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
			lock0 = false;
		}

		StaticJsonDocument<12288> getJson(unsigned long fromTimestamp)
		{
			// while(lock0);
			lock1 = true;
			StaticJsonDocument<12288> doc;
			unsigned int i = mStart, end = mEnd;
			size_t j = 0;
			while (i != end)
			{
				if (lock0)
					break;
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
			lock1 = false;
			return doc;
		}
		
	private:
		volatile unsigned int mStart = 0, mEnd = 0;
		const char * mNames[entries];
		volatile unsigned long mTimestamps[size];
		volatile float mValues[size][entries];
};

#endif /* HISTORY_H */
