
#ifndef HARDWARE_H
#define HARDWARE_H

#define HISTORY_SIZE 120

#include <ArduinoJson.h>
#include "Air.h"
#include "Fan.h"
#include "Heater.h"
#include "History.h"
#include "Runnable.h"
#include "Sensor.h"
#include "Status.h"

const char * sensorNames[] = {"Heater","Air","Fan", "Power"/*, "Resin" */};

class Hardware : public Runnable
{
	public:
		Hardware()
		: mHistory(sensorNames)
		{
			mStatus.lastTimestamp = 0;
			mLastTimestampHistory = 0;
		}

		void readSensors(float dt) throw(SensorOutOfRange)
		{
			mAir.readSensors(dt);
			mHeater.readSensors(dt);
			mFan.readSensors(dt);
		}

		void run(float dt)
		{
			// Read the current time
			unsigned long currentTimestamp = millis();
			unsigned long dt_long = currentTimestamp - mStatus.lastTimestamp;
			// Returns if the time elapsed from the last execution is under 100ms
			if (dt_long < 100)
				return;
			
			// Calculate the time difference in seconds
			float dt = ((float)(dt_long))/1000.0;
			
			mAir.compute(dt)
			mHeater.run(, dt);
				
			// Calculate the fan speed given its current mode
			switch (status.fanMode)
			{
				case FAN_AUTO:
					// In automatic mode the fan turns on at full speed when the
					// temperature difference between the heater core and air sensors
					// is greater than 5 degrees.
					if (mHeater.getSensorValue() - mAir.getSensorValue() >= 5)
						mFan.setSpeed(1.0);
					else
						mFan.setSpeed(0.0);
					break;
				case FAN_MANUAL:
					// In manual mode the fan turns at the given speed.
					status.fanSpeed = status.fanManualSpeed;
			}
			
			mAir.run(dt);
			mHeater.run(dt);
			mFan.run(dt);

			// Log sensors
			if (currentTimestamp - mLastTimestampHistory >= 1000)
			{
				HistoryEntry<float, 4> entry;
				entry.timestamp = currentTimestamp;
				entry.values[0] = mHeater.getSensorValue();
				entry.values[1] = mAir.getSensorValue();
				entry.values[2] = mFan.getSpeed();
				entry.values[3] = mHeater.getPower();
				/* Start of critical section */
				mHistory.push(entry);
				/* End of critical section */
				mLastTimestampHistory = currentTimestamp;
			}

			// Store the current timestamp as the timestamp of the last execution
			mStatus.lastTimestamp = currentTimestamp;
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
			mAir.loadFromJson(json["air"]);
			mHeater.loadFromJson(json["heater"]);
			mFan.loadFromJson(json["fan"]);
			mSensorHeater = Sensor::fromJson(json["heater"]["sensor"]);
			mSensorAir = Sensor::fromJson(json["air"]["sensor"]);
		}

		void populateHistoryJson(JsonDocument &doc, unsigned long fromTimestamp)
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
		unsigned long mLastTimestampHistory;
		Status mStatus;
		Fan mFan;
		Air mAir;
		Heater mHeater;
		History<float, 4, HISTORY_SIZE> mHistory;
		Sensor * mSensorHeater;
		Sensor * mSensorAir;
};

#endif /* HARDWARE_H */