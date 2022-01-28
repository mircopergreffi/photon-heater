

#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <Arduino.h>
#include <String.h>
#include <ArduinoJson.h>

#include <driver/adc.h>

class AnalogSensor
{
	public:
		AnalogSensor()
		{

		}
		AnalogSensor(JsonObject json)
		{
			loadFromJson(json);
		}
		void loadFromJson(JsonObject json)
		{
			mPin = json["pin"].as<int>();
			mName = json["name"].as<String>();
			// adc2_config_width(ADC_WIDTH_12Bit);
			// adc2_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11)
			Serial.print("Sensor ");
			Serial.print(mName);
			Serial.print(" on pin ");
			Serial.println(mPin);
		}
		virtual float readValue()
		{
			return ((float)(analogRead(mPin) - 235)) / 4095.0;
		}
		int getPin()
		{
			return mPin;
		}
		String getName()
		{
			return mName;
		}

	private:
		int mPin;
		String mName;
};

#endif /* ANALOG_SENSOR_H */
