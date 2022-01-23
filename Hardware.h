
#ifndef HARDWARE_H
#define HARDWARE_H

#define HISTORY_SIZE

#include <ArduinoJson.h>
#include <PID_v1.h>

#include "Status.h"
#include "History.h"
#include "Heater.h"
#include "Fan.h"
#include "NTCSensor.h"

const char * sensorNames[] = {"Heater","Air","Fan" /*, "Resin" */};

class Hardware
{
	public:
		Hardware()
		: mHistory({"Heater","Air","Fan" /*, "Resin" */}),
		  mController(&mStatus.temperatureHeater, &mStatus.powerHeater, &mStatus.temperatureSetpoint, 0.0, 0.0, 0.0, P_ON_E, DIRECT)
		{

		}

		void run()
		{
			unsigned long timestamp = millis();
			if (timestamp - mLastTimestamp < 100)
				return;

			mStatus.temperatureHeater = mSensorHeater.readValue();
			mStatus.temperatureAir = mSensorAir.readValue();
			// mStatus.temperatureResin = mSensorResin.readValue();

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

			mController.Compute();
			mHeater.setPower(mStatus.powerHeater);

			if (timestamp - mLastTimestamp >= 1000)
			{
				float values[] = {mStatus.temperatureHeater, mStatus.temperatureAir, mFan.getSpeed()};
				mHistory.push(timestamp, values);
			}

			mLastTimestamp = timestamp;
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
		void loadFromJson(JsonObject & json)
		{
			mHeater.loadFromJson(doc["heater"]);
			mFan.loadFromJson(doc["fan"]);
			mSensorHeater.loadFromJson(doc["heater"]["sensor"]);
			mSensorAir.loadFromJson(doc["air_sensor"]);
			// mSensorResin.loadFromJson(doc["resin_sensor"]);
			mController.SetTunings(doc["control"]["p"].as<float>(), doc["control"]["i"].as<float>(), doc["control"]["d"].as<float>());
			mController.SetOutputLimits(0.0, 1.0);
			mController.SetSampleTime(100);
			mController.SetMode(AUTOMATIC);
		}

		const Status & getStatus() const
		{
			return mStatus;
		}
		const History<float, 3, HISTORY_SIZE> & getHistory() const
		{
			return mHistory;
		}
	private:
		unsigned long mLastTimestamp;
		volatile Status mStatus;
		volatile History<float, 3, HISTORY_SIZE> mHistory;
		Heater mHeater;
		Fan mFan;
		NTCSensor mSensorHeater;
		NTCSensor mSensorAir;
		// NTCSensor mSensorResin;
		PID mController;
}

#endif /* HARDWARE_H */