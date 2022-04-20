
#ifndef AIR_H
#define AIR_H

#include "Controller.h"
#include "Hardware.h"
#include "Sensor.h"

class Air : public Hardware
{
	public:
		Air()
		{
			mTempMax = -1;
			mTempMin = -1;
			mTempMaxCritical = -1;
			mTempMinCritical = -1;
			exceededMaxThreshold = 1;
			exceededMaxCounter = 0;
			mSensor = nullptr;
			mController = nullptr;
		}

		// Loads values and parameters from json object
		void loadFromJson(JsonObject const &json) throw(InvalidJsonException)
		{
			if (json.containsKey("temp_max"))
				mTempMax = json["temp_max"];
			if (json.containsKey("temp_min"))
				mTempMin = json["temp_min"];
			if (json.containsKey("temp_max_critical"))
				mTempMaxCritical = json["temp_max_critical"];
			if (json.containsKey("temp_min_critical"))
				mTempMinCritical = json["temp_min_critical"];
			if (json.containsKey("critical_threshold"))
				exceededMaxThreshold = json["critical_threshold"];
			
			if (mController != nullptr)
				delete mController;
			if (json.containsKey("control"))
				mController = Controller::fromJson(json["control"]);
			else
			{
				mController = nullptr;
				throw InvalidJsonException("Air module requires a control");
			}
			
			if (mSensor != nullptr)
				delete mSensor;
			if (json.containsKey("sensor"))
				mSensor = Sensor::fromJson(json["sensor"]);
			else
			{
				mSensor = nullptr;
				throw InvalidJsonException("Air module requires a sensor");
			}
		}

		void readSensors(float dt) throw(SensorOutOfRange)
		{
			mSensorValue = mSensor->readValue(dt);
			// Shutdown heater in case it execeds the critical temperatures
			if (mSensorValue >= mTempMaxCritical || mSensorValue <= mTempMinCritical)
				exceededMaxCounter++;
			else
				exceededMaxCounter = 0;

			if(exceededMaxCounter >= exceededMaxThreshold)
				throw SensorOutOfRange();
		}

		// Read sensor and returns an action to control the temperature
		void run(float dt)
		{
		}

		float compute(float dt)
		{
			return mController->compute(mSensorValue, mSetPoint, dt);
		}

		void setSetPoint(float setPoint)
		{
			if (setPoint > mTempMax)
				mSetPoint = mTempMax;
			if (setPoint < mTempMin)
				mSetPoint = mTempMin;
			else
				mSetPoint = setPoint;
		}
		
		float getTemperature() const
		{
			return mSensorValue;
		}

	private:
		float mTempMax, mTempMin;
		float mTempMaxCritical, mTempMinCritical;
		int exceededMaxThreshold, exceededMaxCounter;
		Sensor * mSensor;
		Controller * mController;
		
		float mSensorValue;
		float mSetPoint;

};

#endif /* AIR_H */