
#ifndef HARDWARE_H
#define HARDWARE_H

#define HISTORY_SIZE 60

#include <ArduinoJson.h>

#include "Status.h"
#include "History.h"
#include "Heater.h"
#include "Fan.h"
#include "NTCSensor.h"
#include "LowPassFilter.h"
#include "PID.h"

const char * sensorNames[] = {"Heater","Air","Fan" /*, "Resin" */};

class Hardware
{
	public:
		Hardware()
		: mHistory(sensorNames)
		{
			mLastTimestamp = 0;
		}

		void run()
		{
			unsigned long timestamp = millis();
			if (timestamp - mLastTimestamp < 100)
				return;

			mStatus.temperatureHeater = mFilterSensorHeater.filter(mSensorHeater.readValue());
			mStatus.temperatureAir = mFilterSensorAir.filter(mSensorAir.readValue());
			// mStatus.temperatureResin = mFilterSensorResin.filter(mSensorResin.readValue());

			switch (mStatus.fanMode)
			{
				case FAN_AUTO:
					if (mStatus.temperatureHeater - mStatus.temperatureAir >= 5)
						mFan.setSpeed(1.0);
					else
						mFan.setSpeed(0.0);
					break;
				case FAN_MANUAL:
					mFan.setSpeed(mStatus.fanManualSpeed);
			}

			if (mStatus.heater)
			{
				float dt = ((float)(timestamp - mLastTimestamp))/1000.0;
				mStatus.powerHeater = mController.Compute(mStatus.temperatureHeater, mStatus.temperatureSetpoint, dt);
			}
			else
			{
				mStatus.powerHeater = 0;
			}
			mHeater.setPower(mStatus.powerHeater);

			if (timestamp - mLastTimestampHistory >= 1000)
			{
				HistoryEntry<float, 3> entry;
				entry.timestamp = timestamp;
				entry.values[0] = mStatus.temperatureHeater;
				entry.values[1] = mStatus.temperatureAir;
				// entry.values[2] = mStatus.temperatureResin;
				entry.values[2] = mFan.getSpeed();
				/* Start of critical section */
				mHistory.push(entry);
				/* End of critical section */
				mLastTimestampHistory = timestamp;
			}

			mLastTimestamp = timestamp;
		}

		void setHeaterOn(bool heaterOn)
		{
			mStatus.heater = heaterOn;
		}
		void setFanMode(fan_mode_t mode)
		{
			mStatus.fanMode = mode;
		}
		void setFanManualSpeed(float speed)
		{
			mStatus.fanManualSpeed = speed;
		}
		void setTemperatureSetPoint(float temperature)
		{
			mStatus.temperatureSetpoint = temperature;
		}
		void loadFromJson(JsonDocument & json)
		{
			mHeater.loadFromJson(json["heater"]);
			mFan.loadFromJson(json["fan"]);
			mSensorHeater.loadFromJson(json["heater"]["sensor"]);
			mSensorAir.loadFromJson(json["air_sensor"]);
			mFilterSensorHeater.loadFromJson(json["heater"]["sensor"]["filter"]);
			mFilterSensorAir.loadFromJson(json["air_sensor"]["filter"]);
			// mSensorResin.loadFromJson(json["resin_sensor"]);
			// mFilterSensorAir.loadFromJson(json["resin_sensor"]["filter"]);
			mController.setTunings(json["control"]["p"].as<float>(), json["control"]["i"].as<float>(), json["control"]["d"].as<float>());
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
		unsigned long mLastTimestamp, mLastTimestampHistory;
		Status mStatus;
		History<float, 3, HISTORY_SIZE> mHistory;
		Heater mHeater;
		Fan mFan;
		NTCSensor mSensorHeater, mSensorAir;
		LowPassFilter mFilterSensorHeater, mFilterSensorAir;
		// NTCSensor mSensorResin;
		PID mController;
};

#endif /* HARDWARE_H */