
#ifndef HARDWARE_H
#define HARDWARE_H

#define HISTORY_SIZE 60

#include <ArduinoJson.h>

#include "Air.h"
#include "Fan.h"
#include "Heater.h"
#include "History.h"
#include "NTCSensor.h"
#include "Status.h"

const char * sensorNames[] = {"Heater","Air","Fan" /*, "Resin" */};

class Hardware
{
	public:
		Hardware()
		: mHistory(sensorNames)
		{
			mStatus.lastTimestamp = 0;
		}

		void run()
		{
			// Read the current time
			mStatus.currentTimestamp = millis();
			// Returns if the time elapsed from the last execution is under 100ms
			if (mStatus.currentTimestamp - mStatus.lastTimestamp < 100)
				return;

			// Calculate the time difference in seconds
			mStatus.dt = ((float)(mStatus.currentTimestamp - mStatus.lastTimestamp))/1000.0;
			
			// Read values from the sensors
			mStatus.temperatureHeater = mSensorHeater.readValue(mStatus.dt);
			mStatus.temperatureAir = mSensorAir.readValue(mStatus.dt);
			// mStatus.temperatureResin = mSensorResin.readValue(mStatus.dt);

			// Run the various logics
			mFan.run(mStatus);
			mAir.run(mStatus);
			mHeater.run(mStatus);
			mHistory.run(mStatus);

			// Store the current timestamp as the timestamp of the last execution
			mStatus.lastTimestamp = mStatus.currentTimestamp;
		}

		void setHeaterOn(bool heater)
		{
			mStatus.heater = heater;
		}
		void setFanMode(fan_mode_t mode)
		{
			mStatus.fanMode = mode;
		}
		void setFanManualSpeed(float speed)
		{
			mStatus.fanManualSpeed = speed;
		}
		void setTemperature(float temperature)
		{
			mStatus.airSetpoint = temperature;
		}
		void loadFromJson(JsonDocument & json)
		{
			mHeater.loadFromJson(json["heater"]);
			mFan.loadFromJson(json["fan"]);
			mSensorHeater.loadFromJson(json["heater"]["sensor"]);
			mSensorAir.loadFromJson(json["air"]["sensor"]);
			// mSensorResin.loadFromJson(json["resin"]["sensor"]);
		}

		void populateHistoryJson(JsonDocument & doc, unsigned long fromTimestamp)
		{
			/* Start of critical section */
			mHistory.populateJson(doc, fromTimestamp);
			/* End of critical section */
		}

		void printHistoryTo(Print &stream, unsigned long fromTimestamp, char separator)
		{
			/* Start of critical section */
			mHistory.printTo(stream, fromTimestamp, separator);
			/* End of critical section */
		}

		const Status & getStatus() const
		{
			return mStatus;
		}
	private:
		Status mStatus;
		Fan mFan;
		Air mAir;
		Heater mHeater;
		History<float, 3, HISTORY_SIZE> mHistory;
		NTCSensor mSensorHeater, mSensorAir /*, mSensorResin */;
};

#endif /* HARDWARE_H */