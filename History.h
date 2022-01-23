
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

		void populateJson(JsonDocument & doc, unsigned long fromTimestamp) const
		{
			size_t len = mBuffer.length();
			for (size_t i = 0; i<len; i++)
			{
				try
				{
					const HistoryEntry<T, N> & entry = mBuffer.get(i);
					if (entry.timestamp < fromTimestamp)
						continue;
					doc["timestamps"][i] = entry.timestamp;
					for (size_t j = 0; j<N; j++)
						doc[mNames[j]][i] = entry.values[j];
				}
				catch (OutOfBoundsException& e)
				{
					Serial.println(e.what());
				}
			}
		}
		
	private:
		CircularBuffer<HistoryEntry<T, N>, size> mBuffer;
		const char * mNames[N];
};

#endif /* HISTORY_H */
