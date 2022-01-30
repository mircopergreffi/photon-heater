
#ifndef HISTORY_H
#define HISTORY_H

#include <String.h>
#include <ArduinoJson.h>
#include "CircularBuffer.h"

template<typename T, size_t N>
class HistoryEntry
{
	public:
		unsigned long timestamp;
		T values[N];
};

template<typename T, size_t N, size_t size>
class History
{
	public:
		History(const char * names[N])
		{
			for (int i=0; i<N; i++)
				mNames[i] = names[i];
		}

		void push(const HistoryEntry<T, N> & entry)
		{
			mBuffer.push(entry);
		}

		void run(Status &status)
		{
			if (status.currentTimestamp - mLastTimestampHistory >= 1000)
			{
				HistoryEntry<float, 3> entry;
				entry.timestamp = status.currentTimestamp;
				entry.values[0] = status.temperatureHeater;
				entry.values[1] = status.temperatureAir;
				// entry.values[2] = status.temperatureResin;
				entry.values[2] = status.fanSpeed;
				/* Start of critical section */
				push(entry);
				/* End of critical section */
				mLastTimestampHistory = status.currentTimestamp;
			}
		}

		void populateJson(JsonDocument & doc, unsigned long fromTimestamp) const
		{
			try
			{
				size_t len = mBuffer.length();
				for (size_t i = 0; i<len; i++)
				{
					const HistoryEntry<T, N> & entry = mBuffer.get(i);
					if (entry.timestamp < fromTimestamp)
						continue;
					doc["timestamps"][i] = entry.timestamp;
					for (size_t j = 0; j<N; j++)
						doc[mNames[j]][i] = entry.values[j];
				}
			}
			catch (OutOfBoundsException& e)
			{
				Serial.println(e.what());
			}
		}

		void printTo(Print &stream, unsigned long fromTimestamp, char separator) const
		{
			stream.print("Timestamp");
			for (size_t j = 0; j<N; j++)
			{
				stream.print(separator);
				stream.print(mNames[j]);
			}
			stream.println();
			try
			{
				size_t len = mBuffer.length();
				for (size_t i = 0; i<len; i++)
				{
					const HistoryEntry<T, N> & entry = mBuffer.get(i);
					if (entry.timestamp < fromTimestamp)
						continue;
					stream.print(entry.timestamp);
					for (size_t j = 0; j<N; j++)
					{
						stream.print(separator);
						stream.print(entry.values[j]);
					}
					stream.println();
				}
			}
			catch (OutOfBoundsException& e)
			{
				Serial.println(e.what());
			}
		}
		
	private:
		unsigned long mLastTimestampHistory;
		CircularBuffer<HistoryEntry<T, N>, size> mBuffer;
		const char * mNames[N];
};

#endif /* HISTORY_H */
